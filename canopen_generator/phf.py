"""
Two-level Perfect Hash Builder with Displacement Search (C++ codegen ready).

This module implements a compact two-level perfect hash function (PHF) that maps
a finite set of 16/32-bit unsigned integer keys to unique slots without collisions.
The construction proceeds bucket-wise: keys are first partitioned by a coarse hash
(h0) into B buckets; within each bucket, a single displacement value d_b is searched
so that the displaced fine hash (h1 + d_b) yields non-overlapping slots in a
global table of size M (both B and M are powers of two).

Key properties
--------------
- Deterministic builds: seeds are derived in a stable manner; parallel builds
  remain reproducible.
- Size-aware code generation: the builder selects compact C types for the
  displacement table and value payloads and can render ready-to-use HPP/CPP files
  through Jinja2 templates.
- Multi-objective selection: among successful trials, the "best" configuration
  minimizes total flash footprint; ties are broken by the maximum displacement
  and then by build time.

Hash family
-----------
Let mix_w(x) = (x + (x >> 3)) mod 2^w for w ∈ {16, 32}.
For parameters (A0, A1), and power-of-two sizes B = 2^{B_BITS}, M = 2^{M_BITS}:

    h0(x) = ((mix_w(x) * A0) >> (w - B_BITS)) & (B - 1)
    h1(x) = (mix_w(x) * A1) & (M - 1)

The builder first tries cheap multipliers (e.g., A1 ≡ 1 mod M, A0 ∈ {1,3,5,...})
to enable constant-time index computation on constrained targets, then falls back
to random odd 16-bit pairs if necessary.

Usage overview
--------------
1) Instantiate `PHF(keys, values=..., target_load=0.6, seed=...)`.
2) Search for the best layout:
       best = phf.build_best_any(parallel=True, trials_per_B=..., ...)
3) Generate code:
       hpp = phf.get_hpp(namespace="phf")
       cpp = phf.get_cpp(header_file="phf_lookup.hpp")

Constraints and limitations
---------------------------
- Keys must be unique unsigned integers fitting in 16 or 32 bits.
- Values are treated as unsigned payloads; signed or structured payloads are not
  handled by default (extend `_ctype_for_values` if needed).
- The mixing function is not cryptographic; this implementation targets PHF
  construction and constant-time lookup on embedded/flash-constrained systems.

Performance notes
-----------------
- The displacement search is linear in M for each bucket; large buckets or very
  high load factors may increase build time. Consider tuning `target_load`,
  `trials_per_B`, or `max_attempts_per_trial`, and/or augmenting the search
  heuristic for demanding datasets.

"""

import math
import os
import random
import time
from concurrent.futures import ProcessPoolExecutor, as_completed
from dataclasses import dataclass
from pathlib import Path
from statistics import mean
from typing import (
    Any,
    Dict,
    Final,
    Iterable,
    Iterator,
    Literal,
    Optional,
    Sequence,
    Tuple,
    TypeAlias,
)

from jinja2 import (
    Environment,
    FileSystemLoader,
    StrictUndefined,
    TemplateError,
    TemplateNotFound,
)

BASE_DIR = Path(__file__).resolve().parent
TEMPLATE_DIR: Final[Path] = BASE_DIR / "templates"

DERIVED_SEED_BASE: Final = 1_234_567
MAX_ATTEMPTS_DEFAULT: Final = 10_000


@dataclass(frozen=True, slots=True)
class Params:
    """Hash parameters and derived constants."""

    A0: int
    A1: int
    B_BITS: int
    M_BITS: int
    B: int
    M: int
    KEY_BITS: int


@dataclass(slots=True, frozen=True)
class TrialArtifacts:
    """Arrays produced by a successful build."""

    P: Params
    D: tuple[int, ...]  # size B
    K: tuple[int, ...]  # size M (uint16_t values, 0xFFFF for empty)
    V: tuple[int, ...]  # size M
    dmax: int  # max displacement used across non-empty buckets
    dmean: float  # mean displacement across non-empty buckets


@dataclass(frozen=True, slots=True)
class BestConfig:
    """Selected best configuration among candidates."""

    artifacts: TrialArtifacts
    displace_ctype: str
    values_ctype: str
    bytes_total: int
    build_ms: float


def _ctype_for_keys(max_key: int) -> str:
    return "uint16_t" if max_key <= 0xFFFF else "uint32_t"


def _next_pow2(x: int) -> int:
    return 1 if x <= 1 else 1 << ((x - 1).bit_length())


def _flash_cost(
    M: int, B: int, displace_bytes: int, key_bytes: int, value_bytes: int
) -> int:
    """
    Total flash footprint of the tables:
    - g_displace[B]  (displacement per bucket)
    - g_keys[M]      (uint16_t per slot)
    - g_values[M]    (value payload per slot)
    """
    return displace_bytes * B + key_bytes * M + value_bytes * M


def _ctype_for_displace(dmax: int) -> str:
    return "uint8_t" if dmax <= 0xFF else "uint16_t"


def _ctype_for_values(vmin: int, vmax: int) -> str:
    """
    Select a compact unsigned C type for values.
    Extend if you need signed or custom structures.
    """
    if 0 <= vmin and vmax <= 0xFF:
        return "uint8_t"
    if 0 <= vmin and vmax <= 0xFFFF:
        return "uint16_t"
    return "uint32_t"


def _parallel_trial(args):
    """
    Worker d'un essai: construit une configuration pour (B, seed).
    Retourne (B, art, elapsed_ms). Lève RuntimeError si aucun placement.
    """
    (keys, values, M, B, seed, max_attempts) = args
    # Instanciation locale (l'env. Jinja ne sera pas utilisé)
    phf_local = PHF(keys, values=values, M=M, B=B, trials=1, seed=seed)
    rng = random.Random(seed)
    t0 = time.perf_counter()
    art = phf_local._build_search_fixed(phf_local.items, M, B, rng=rng, max_attempts=max_attempts)
    elapsed_ms = (time.perf_counter() - t0) * 1000.0
    return (B, art, elapsed_ms)


def _best_pick(curr: Optional[BestConfig], cand: BestConfig) -> BestConfig:
    """Return the better of two candidates using bytes_total, dmax, then build time."""
    if curr is None:
        return cand
    if cand.bytes_total != curr.bytes_total:
        return cand if cand.bytes_total < curr.bytes_total else curr
    if cand.artifacts.dmax != curr.artifacts.dmax:
        return cand if cand.artifacts.dmax < curr.artifacts.dmax else curr
    return cand if cand.build_ms < curr.build_ms else curr


def _candidate_powers(p_max: int) -> list[int]:
    """Last six powers up to and including p_max (at least 1)."""
    p_min = max(1, p_max - 5)
    return list(range(p_min, p_max + 1))


def _score_config(
    *,
    art: TrialArtifacts,
    B: int,
    key_bits: int,
    vmin: int,
    vmax: int,
) -> tuple[int, str, str]:
    """Compute flash footprint and chosen C types for a configuration."""
    displace_ctype = _ctype_for_displace(art.dmax)
    values_ctype = _ctype_for_values(vmin, vmax)
    keys_bytes = 4 if key_bits == 32 else 2
    displace_bytes = 1 if displace_ctype == "uint8_t" else 2
    value_bytes = (
        1 if values_ctype == "uint8_t" else (2 if values_ctype == "uint16_t" else 4)
    )
    bytes_total = _flash_cost(art.P.M, B, displace_bytes, keys_bytes, value_bytes)
    return bytes_total, displace_ctype, values_ctype


def _derive_seed(base: int, B: int, t: int) -> int:
    return (base ^ (B * 0x9E3779B1) ^ (t * 0x85EBCA6B)) & 0xFFFFFFFF


# --- Unified core ------------------------------------------------------------
Task: TypeAlias = tuple[Sequence[int], Sequence[int], int, int, int, int]


def _trial_args_iter(
    *,
    keys: Sequence[int],
    values: Sequence[int],
    M: int,
    candidate_B_powers: Sequence[int],
    trials_per_B: int,
    base_seed: int,
    max_attempts_per_trial: int,
) -> Iterable[Task]:
    """Yield (keys, values, M, B, seed, max_attempts) for each trial."""
    for p in reversed(candidate_B_powers):  # start with larger B (smaller buckets)
        B = 1 << p
        if B > M:
            continue
        for t in range(trials_per_B):
            # Stable derived seed per (B, trial index)
            seed = _derive_seed(base_seed, B, t)
            yield (keys, values, M, B, seed, max_attempts_per_trial)


def _run_trials_serial(
    *,
    self_ref: "PHF",
    tasks: Iterable[tuple[Sequence[int], Sequence[int], int, int, int, int]],
) -> Iterator[tuple[int, TrialArtifacts, float]]:
    """Run trials in-process (serial)."""
    for _, _, M, B, seed, max_attempts in tasks:
        rng = random.Random(int(seed))
        t0 = time.perf_counter()
        try:
            art = self_ref._build_search_fixed(
                self_ref.items, M, B, rng=rng, max_attempts=max_attempts
            )
        except RuntimeError:
            continue
        yield B, art, (time.perf_counter() - t0) * 1000.0


def _run_trials_parallel(
    *,
    tasks: Iterable[tuple[Sequence[int], Sequence[int], int, int, int, int]],
    max_workers: int,
) -> Iterator[tuple[int, TrialArtifacts, float]]:
    """Run trials in a process pool (CPU-bound)."""
    with ProcessPoolExecutor(max_workers=max_workers) as ex:
        futures = [ex.submit(_parallel_trial, args) for args in tasks]
        for fut in as_completed(futures):
            try:
                B, art, build_ms = fut.result()
            except RuntimeError:
                continue
            yield B, art, build_ms


class BuildNotFound(RuntimeError):
    pass


class PHF:
    """
    Two-level perfect hash using per-bucket displacements.
    Hash family:
      h0(x) = ((mix16(x) * A0) >> (16 - B_BITS)) & (B-1)
      h1(x) =  (mix16(x) * A1) & (M-1)
    with mix16(x) = x + (x >> 3) (mod 2^16).
    """

    def __init__(
        self,
        keys: Sequence[int],
        values: Optional[Sequence[int]] = None,
        *,
        trials: int = 30,
        M: Optional[int] = None,
        B: Optional[int] = None,
        target_load: float = 0.8,
        seed: Optional[int] = 43,
    ) -> None:
        if not (0.0 < target_load <= 1.0):
            raise ValueError("target_load must be in (0.0, 1.0].")

        self.keys, self.values = self._prepare_keys_values(keys, values)
        self.items = list(zip(self.keys, self.values))

        self.key_bits = 16 if (max(self.keys, default=0) <= 0xFFFF) else 32
        self.key_mask = (1 << self.key_bits) - 1

        N = len(self.keys)
        self.M, self.B = self._derive_sizes(N, target_load, M, B)

        self.trials: int = int(trials)
        self.seed: Optional[int] = seed

        # Build summaries
        self.artifacts: Dict[int, TrialArtifacts] = {}
        self._best: Optional[BestConfig] = None

        self.env = None

    def _prepare_keys_values(
        self, keys: Sequence[int], values: Optional[Sequence[int]]
    ) -> tuple[tuple[int, ...], tuple[int, ...]]:
        if len(keys) != len(set(keys)):
            raise ValueError("Duplicate keys detected.")
        if any(not (0 <= k <= 0xFFFFFFFF) for k in keys):
            raise ValueError("Keys must be 32-bit unsigned integers (0..2^32-1).")

        keys_t = tuple(keys)
        if values is None:
            values_t = tuple(((k ^ 0x55AA) & 0xFFFF) for k in keys_t)
        else:
            if len(values) != len(keys):
                raise ValueError("Values must have the same cardinality as keys.")
            if any(v < 0 for v in values):
                raise ValueError("Values must be unsigned (>= 0).")
            kv = dict(zip(keys, values))
            if set(kv.keys()) != set(keys_t):
                raise ValueError("Values must be provided for all unique keys.")
            values_t = tuple(kv[k] for k in keys_t)

        if any(v > 0xFFFFFFFF for v in values_t):
            raise ValueError("Values must fit into 32-bit unsigned.")
        return keys_t, values_t

    def _derive_sizes(
        self, N: int, target_load: float, M: Optional[int], B: Optional[int]
    ) -> tuple[int, int]:
        M_eff = _next_pow2(int(math.ceil(N / target_load))) if M is None else M
        if M_eff & (M_eff - 1):
            raise ValueError("M must be a power of two.")
        B_eff = B if B is not None else max(1, M_eff // 2)
        if B_eff & (B_eff - 1):
            raise ValueError("B must be a power of two.")
        if B_eff > M_eff:
            raise ValueError("B must not exceed M.")
        return M_eff, B_eff

    def _ensure_env(self) -> Environment:
        if self.env is None:
            self.env = Environment(
                loader=FileSystemLoader(TEMPLATE_DIR),
                undefined=StrictUndefined,
                trim_blocks=False,
                lstrip_blocks=True,
                autoescape=False,
            )
        return self.env

    @staticmethod
    def _mix_w(x: int, w: int) -> int:
        # Additive scramble modulo 2^w
        return (x + (x >> 3)) & ((1 << w) - 1)

    @staticmethod
    def _h0(x: int, P: Params) -> int:
        m = PHF._mix_w(x, P.KEY_BITS)
        prod = (m * P.A0) & ((1 << P.KEY_BITS) - 1)
        return (prod >> (P.KEY_BITS - P.B_BITS)) & (P.B - 1)

    @staticmethod
    def _h1(x: int, P: Params) -> int:
        m = PHF._mix_w(x, P.KEY_BITS)
        prod = (m * P.A1) & ((1 << P.KEY_BITS) - 1)
        return prod & (P.M - 1)

    def _precompute_base_h1(
        self, bucket: list[tuple[int, int]], P: Params
    ) -> Optional[list[int]]:
        base, seen = [], set()
        for k, _ in bucket:
            bi = self._h1(k, P)
            if bi in seen:
                return None
            seen.add(bi)
            base.append(bi)
        return base

    def _try_place_bucket(
        self,
        b: int,
        bucket: list[tuple[int, int]],
        base: list[int],
        P: Params,
        used: list[bool],
        K: list[int],
        V: list[int],
        D: list[int],
    ) -> bool:
        m_mask = P.M - 1
        for d in range(P.M):
            indices = [(bi + d) & m_mask for bi in base]
            if any(used[i] for i in indices):
                continue
            D[b] = d
            for (k, val), i in zip(bucket, indices):
                used[i] = True
                K[i] = k
                V[i] = val
            return True
        return False

    def _build_once_fixed(
        self, items: Sequence[Tuple[int, int]], A0: int, A1: int, M: int, B: int
    ) -> Optional[TrialArtifacts]:
        """Try to place all keys for fixed (A0, A1, M, B). Return artifacts on success, None otherwise."""
        M_BITS = M.bit_length() - 1
        B_BITS = B.bit_length() - 1
        P = Params(
            A0=A0, A1=A1, B_BITS=B_BITS, M_BITS=M_BITS, B=B, M=M, KEY_BITS=self.key_bits
        )

        # Prepare buckets
        buckets: list[list[tuple[int, int]]] = [[] for _ in range(B)]
        for k, v in items:
            buckets[self._h0(k, P)].append((k, v))

        # Place largest buckets first
        bucket_order = sorted(range(B), key=lambda b: len(buckets[b]), reverse=True)

        empty_key = self.key_mask

        D: list[int] = [0] * B
        K: list[int] = [empty_key] * M
        V: list[int] = [0] * M
        used = [False] * M

        for b in bucket_order:
            bucket = buckets[b]
            if not bucket:
                continue
            if len(bucket) == 1:
                k, val = bucket[0]
                i = self._h1(k, P)
                if used[i]:
                    # fall back to displacement search below (rare)
                    pass
                else:
                    D[b] = 0
                    used[i] = True
                    K[i] = k
                    V[i] = val
                    continue

            # Precompute the base h1 values (before displacement) for bucket b
            base = self._precompute_base_h1(bucket, P)
            if base is None:
                return None

            if not self._try_place_bucket(b, bucket, base, P, used, K, V, D):
                return None

        # Displacement statistics on non-empty buckets
        non_empty_disp = [D[b] for b in range(B) if buckets[b]]
        dmax = max(non_empty_disp, default=0)
        dmean = float(mean(non_empty_disp)) if non_empty_disp else 0.0

        return TrialArtifacts(
            P=P, D=tuple(D), K=tuple(K), V=tuple(V), dmax=dmax, dmean=dmean
        )

    def _gen_A1_mod_M(
        self, M_BITS: int, rng: random.Random, budget: int
    ) -> Iterable[int]:
        """
        Yield candidates A1 such that A1 ≡ 1 (mod 2^{M_BITS}).
        Emits 1 first (ultra-cheap case), then up to `budget` random candidates.
        """
        yield 1 & 0xFFFF
        for _ in range(budget):
            # k occupies bits above M_BITS within a 16-bit word, keeping A1 odd.
            k = rng.randrange(0, 1 << max(0, 16 - M_BITS))
            yield (1 + (k << M_BITS)) & 0xFFFF

    def _try_phase_modM(
        self,
        items: Sequence[tuple[int, int]],
        M: int,
        B: int,
        rng: random.Random,
        max_attempts: int,
        attempts: int,
    ) -> tuple[Optional[TrialArtifacts], int]:
        """
        Phase 1:
          (a) Fix A1 ≡ 1 (mod M) and try a set of cheap A0 multipliers to
              potentially eliminate the multiplication in h0.
          (b) If still failing, try a few random odd 16-bit A0 values for the
              same A1 candidate.

        Returns (artifacts_or_None, updated_attempts).
        May raise RuntimeError if `max_attempts` is reached.
        """
        M_BITS = M.bit_length() - 1
        # Keep a bounded budget to avoid consuming all attempts in this phase.
        a1_budget = max(1, min(256, max_attempts // 16))
        cheap_A0s = (1, 3, 5, 9, 17, 33, 65, 129)

        for A1 in self._gen_A1_mod_M(M_BITS, rng, a1_budget):
            # (a) Cheap A0 constants
            for A0 in cheap_A0s:
                attempts += 1
                art = self._build_once_fixed(items, A0 & 0xFFFF, A1, M, B)
                if art is not None:
                    return art, attempts
                if attempts >= max_attempts:
                    raise RuntimeError("Failed to find parameters within max_attempts.")

            # (b) A few random odd 16-bit A0 values
            rnd_budget = max(1, min(64, (max_attempts - attempts) // 8))
            for _ in range(rnd_budget):
                A0 = (rng.getrandbits(16) | 1) & 0xFFFF
                attempts += 1
                art = self._build_once_fixed(items, A0, A1, M, B)
                if art is not None:
                    return art, attempts
                if attempts >= max_attempts:
                    raise RuntimeError("Failed to find parameters within max_attempts.")

        return None, attempts

    def _try_phase_random(
        self,
        items: Sequence[tuple[int, int]],
        M: int,
        B: int,
        rng: random.Random,
        max_attempts: int,
        attempts: int,
    ) -> TrialArtifacts:
        """
        Phase 2: fully random sampling of odd 16-bit pairs (A0, A1).
        Returns artifacts on success.
        Raises RuntimeError once `max_attempts` is exceeded.
        """
        while attempts < max_attempts:
            A0 = (rng.getrandbits(16) | 1) & 0xFFFF
            A1 = (rng.getrandbits(16) | 1) & 0xFFFF
            attempts += 1
            art = self._build_once_fixed(items, A0, A1, M, B)
            if art is not None:
                return art
        raise RuntimeError("Failed to find parameters within max_attempts.")


    def _build_search_fixed(
        self,
        items: Sequence[tuple[int, int]],
        M: int,
        B: int,
        *,
        max_attempts: int = 100_000,
        rng: random.Random,
    ) -> TrialArtifacts:
        """
        Try to find a perfect placement by sampling pairs (A0, A1).

        Two-phase strategy:
          1) Constrain A1 ≡ 1 (mod M), first with cheap A0 values, then a few random A0.
          2) If unsuccessful, sample random odd (A0, A1) pairs.

        Stop on success or once `max_attempts` cumulative attempts have been made.
        """
        attempts = 0

        art, attempts = self._try_phase_modM(
            items=items,
            M=M,
            B=B,
            rng=rng,
            max_attempts=max_attempts,
            attempts=attempts,
        )
        if art is not None:
            return art

        return self._try_phase_random(
            items=items,
            M=M,
            B=B,
            rng=rng,
            max_attempts=max_attempts,
            attempts=attempts,
        )

    def _build_best_core(
        self,
        *,
        candidate_B_powers: Optional[Sequence[int]],
        trials_per_B: Optional[int],
        parallel: bool,
        max_workers: Optional[int],
        max_attempts_per_trial: int,
        start_with_larger_B: bool,
    ) -> BestConfig:
        """
        Unified implementation for (serial | parallel) best search.
        Minimizes flash footprint; ties broken by lower dmax then faster build time.
        """
        p_max = self.M.bit_length() - 1
        candidate_B_powers = list(candidate_B_powers or _candidate_powers(p_max))
        if start_with_larger_B:
            candidate_B_powers = list(reversed(candidate_B_powers))
        trials_per_B = int(trials_per_B or self.trials)

        vmin = min(self.values) if self.values else 0
        vmax = max(self.values) if self.values else 0

        base_seed = int(self.seed if self.seed is not None else DERIVED_SEED_BASE)
        tasks = list(
            _trial_args_iter(
                keys=self.keys,
                values=self.values,
                M=self.M,
                candidate_B_powers=candidate_B_powers,
                trials_per_B=trials_per_B,
                base_seed=base_seed,
                max_attempts_per_trial=max_attempts_per_trial,
            )
        )
        if not tasks:
            raise RuntimeError("No candidate tasks to run.")

        if parallel:
            if not max_workers or max_workers <= 0:
                max_workers = min(os.cpu_count() or 1, len(tasks))
            results_iter = _run_trials_parallel(tasks=tasks, max_workers=max_workers)
        else:
            results_iter = _run_trials_serial(self_ref=self, tasks=tasks)

        best: Optional[BestConfig] = None
        for B, art, build_ms in results_iter:
            bytes_total, displace_ctype, values_ctype = _score_config(
                art=art, B=B, key_bits=self.key_bits, vmin=vmin, vmax=vmax
            )
            cand = BestConfig(
                artifacts=art,
                displace_ctype=displace_ctype,
                values_ctype=values_ctype,
                bytes_total=bytes_total,
                build_ms=build_ms,
            )
            best = _best_pick(best, cand)

        if best is None:
            raise BuildNotFound("No successful build found.")

        self._best = best
        self.artifacts.clear()
        self.artifacts[1] = best.artifacts  # keep code-generation API stable
        return best

    def build_best(
        self,
        *,
        candidate_B_powers: Optional[Sequence[int]] = None,
        trials_per_B: Optional[int] = None,
    ) -> BestConfig:
        """
        Serial search for the best configuration.
        """
        return self._build_best_core(
            candidate_B_powers=candidate_B_powers,
            trials_per_B=trials_per_B,
            parallel=False,
            max_workers=None,
            max_attempts_per_trial=MAX_ATTEMPTS_DEFAULT,  # conservative to avoid long thrashing
            start_with_larger_B=False,  # matches your original serial order
        )

    def build_best_parallel(
        self,
        *,
        candidate_B_powers: Optional[Sequence[int]] = None,
        trials_per_B: Optional[int] = None,
        max_workers: Optional[int] = None,
        max_attempts_per_trial: int = MAX_ATTEMPTS_DEFAULT,
    ) -> BestConfig:
        """
        Parallel search for the best configuration (CPU-bound, deterministic w.r.t. seed).
        """
        return self._build_best_core(
            candidate_B_powers=candidate_B_powers,
            trials_per_B=trials_per_B,
            parallel=True,
            max_workers=max_workers,
            max_attempts_per_trial=max_attempts_per_trial,
            start_with_larger_B=True,  # your parallel version prefers larger B first
        )

    def build_best_any(
        self,
        *,
        parallel: bool = False,
        candidate_B_powers: Optional[Sequence[int]] = None,
        trials_per_B: Optional[int] = None,
        max_workers: Optional[int] = None,
        max_attempts_per_trial: int = MAX_ATTEMPTS_DEFAULT,
        start_with_larger_B: Optional[bool] = None,
    ) -> BestConfig:
        """
        Convenience wrapper that selects defaults matching the serial/parallel helpers.

        - If `parallel` is False: same defaults as `build_best`.
        - If `parallel` is True:  same defaults as `build_best_parallel`.
        - `start_with_larger_B` defaults to `parallel` (your parallel search prefers
          larger B first).
        """
        if start_with_larger_B is None:
            start_with_larger_B = parallel

        return self._build_best_core(
            candidate_B_powers=candidate_B_powers,
            trials_per_B=trials_per_B,
            parallel=parallel,
            max_workers=max_workers,
            max_attempts_per_trial=max_attempts_per_trial,
            start_with_larger_B=start_with_larger_B,
        )

    def load_factor(self) -> float:
        return len(self.keys) / self.M

    #
    # Code generation (C++)
    #
    @staticmethod
    def _values_zero_literal(ctype: str) -> str:
        """Return a neutral literal for the given C value type."""
        return "0.0" if ctype in ("float", "double") else "0"

    def _resolve_ctypes(
        self,
        art: TrialArtifacts,
        *,
        displace_ctype: Optional[str],
        values_ctype: Optional[str],
    ) -> tuple[str, str]:
        """Choose compact C types for displacements and values if not provided."""
        # Displacement type depends on the maximum displacement used.
        dc = displace_ctype or _ctype_for_displace(art.dmax)
        # Value type depends on observed min/max of the value array.
        vmin = min(self.values) if self.values else 0
        vmax = max(self.values) if self.values else 0
        vc = values_ctype or _ctype_for_values(vmin, vmax)
        return dc, vc

    def _render(self, template_str: str, context: Dict[str, Any]) -> str:
        """Render a Jinja template with standard options."""
        try:
            t = self._ensure_env().get_template(template_str)
        except TemplateNotFound as exc:
            raise FileNotFoundError(
                f"Template '{template_str}' not found in '{TEMPLATE_DIR}'."
            ) from exc
        except TemplateError as exc:
            raise RuntimeError(f"Template error in '{template_str}': {exc}") from exc
        return t.render(**context)

    def get_hpp(
        self,
        trial: int = 1,
        *,
        template_str: str = "phf.hpp.j2",
        namespace: str = "phf",
        displace_ctype: Optional[str] = None,
        values_ctype: Optional[str] = None,
        extra_includes: Optional[Sequence[str]] = None,
        header_comment: Optional[str] = None,
    ) -> str:
        """Render the C++ header with constants and declarations."""
        if trial not in self.artifacts:
            raise ValueError("Unknown trial. Build first.")
        art = self.artifacts[trial]
        P = art.P

        dc, vc = self._resolve_ctypes(
            art, displace_ctype=displace_ctype, values_ctype=values_ctype
        )
        includes = list(extra_includes or [])

        keys_ctype = _ctype_for_keys(max(self.keys) if self.keys else 0)

        A1_out = self._canonicalize_a1(P.A1, P.M)

        ctx: Dict[str, Any] = {
            "header_comment": header_comment
            or f"phf_lookup.hpp — generated for trial {trial}",
            "namespace": namespace,
            "A0": P.A0,
            "A1": A1_out,
            "B_BITS": P.B_BITS,
            "M_BITS": P.M_BITS,
            "displace_ctype": dc,
            "values_ctype": vc,
            "keys_ctype": keys_ctype,
            "key_bits": self.key_bits,
            "values_zero": self._values_zero_literal(vc),
            "extra_includes": includes,
        }
        return self._render(template_str, ctx)

    def get_cpp(
        self,
        trial: int = 1,
        *,
        template_str: str = "phf.cpp.j2",
        namespace: str = "phf",
        header_file: str = "phf_lookup.hpp",
        displace_ctype: Optional[str] = None,
        values_ctype: Optional[str] = None,
        cols: int = 16,
        number_format: Literal["dec", "hex16"] = "dec",
    ) -> str:
        """
        Render the C++ data tables and constants for the selected trial
        using a Jinja template that handles all formatting (hex/dec and line breaks).
        """
        if trial not in self.artifacts:
            raise ValueError("Unknown trial. Build first.")
        art = self.artifacts[trial]

        dc, vc = self._resolve_ctypes(
            art, displace_ctype=displace_ctype, values_ctype=values_ctype
        )

        keys_ctype = _ctype_for_keys(max(self.keys) if self.keys else 0)

        ctx: Dict[str, Any] = {
            "header_file": header_file,
            "namespace": namespace,
            "displace_ctype": dc,
            "values_ctype": vc,
            "D": art.D,
            "K": art.K,
            "V": art.V,
            "cols": cols,
            "keys_ctype": keys_ctype,
            "key_bits": self.key_bits,
            "number_format": number_format,
        }
        return self._render(template_str, ctx)

    @staticmethod
    def probe_index(key: int, art: TrialArtifacts) -> int:
        """Compute the slot index i for a given key, using the C++ layout semantics."""
        b = PHF._h0(key, art.P)
        return (PHF._h1(key, art.P) + art.D[b]) & (art.P.M - 1)

    @staticmethod
    def find_py(key: int, art: TrialArtifacts) -> tuple[bool, int]:
        """Python equivalent of the generated C++ find() for validation."""
        i = PHF.probe_index(key, art)
        k = art.K[i]
        if k == key:
            return True, art.V[i]
        return False, 0

    @staticmethod
    def _canonicalize_a1(A1: int, M: int) -> int:
        """Return a readable representative of A1 modulo M (map A1≡1 mod M to 1)."""
        return 1 if (A1 & (M - 1)) == 1 else A1
