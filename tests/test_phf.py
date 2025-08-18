# tests/test_phf.py
import sys
import platform
import types
import importlib
from pathlib import Path

import pytest


def import_fresh_phf(tmp_templates: Path) -> types.ModuleType:
    """
    Import the phf module freshly and redirect its TEMPLATE_DIR
    to `tmp_templates`. Returns the imported module object.
    """
    # Import module
    import canopen_generator.phf as phf_mod  # assumes your file is importable as "phf.py"
    # Monkeypatch TEMPLATE_DIR at module level and clear any existing envs
    phf_mod.TEMPLATE_DIR = tmp_templates
    return phf_mod


@pytest.fixture
def keys_small():
    # A tiny, collision-free set (fits 16-bit)
    return [1, 2, 3, 5, 8, 13, 21]


@pytest.fixture
def values_small(keys_small):
    # Simple mapping: value = key*2 (all fit in uint8)
    return [k * 2 for k in keys_small]


@pytest.fixture
def tmp_templates(tmp_path):
    """
    Create a temporary "templates" directory with minimal Jinja2 templates
    used by get_hpp/get_cpp. Return the folder Path.
    """
    tdir = tmp_path / "templates"
    tdir.mkdir()

    # Minimal header template: reference a few variables that must exist
    (tdir / "phf.hpp.j2").write_text(
        "// {{ header_comment }}\n"
        "namespace {{ namespace }} {\n"
        "  static constexpr unsigned A0 = {{ A0 }};\n"
        "  static constexpr unsigned A1 = {{ A1 }};\n"
        "  using displace_t = {{ displace_ctype }};\n"
        "  using value_t = {{ values_ctype }};\n"
        "}\n",
        encoding="utf-8",
    )

    # Minimal cpp template: emit sizes and a few entries
    (tdir / "phf.cpp.j2").write_text(
        "#include \"{{ header_file }}\"\n"
        "namespace {{ namespace }} {\n"
        "  // sizes: D={{ D|length }}, K={{ K|length }}, V={{ V|length }}\n"
        "  // first-D={{ D[0] if D|length>0 else 0 }}\n"
        "  // key-bits={{ key_bits }} number-format={{ number_format }}\n"
        "}\n",
        encoding="utf-8",
    )

    return tdir


def test_constructor_validation_duplicate_keys(tmp_templates):
    phf = import_fresh_phf(tmp_templates)
    with pytest.raises(ValueError, match="Duplicate keys detected"):
        phf.PHF(keys=[1, 1, 2])


def test_constructor_validation_key_range(tmp_templates):
    phf = import_fresh_phf(tmp_templates)
    with pytest.raises(ValueError, match="32-bit unsigned integers"):
        phf.PHF(keys=[-1, 2, 3])
    with pytest.raises(ValueError, match="32-bit unsigned integers"):
        phf.PHF(keys=[2**33])


def test_values_default_and_unsigned(tmp_templates, keys_small):
    phf = import_fresh_phf(tmp_templates)

    # Default values are derived from keys; all unsigned and <= 0xFFFF
    h = phf.PHF(keys_small, seed=7, trials=1)
    assert len(h.values) == len(keys_small)
    assert all(0 <= v <= 0xFFFFFFFF for v in h.values)

    # Negative values forbidden
    with pytest.raises(ValueError, match="Values must be unsigned"):
        phf.PHF(keys_small, values=[-1] * len(keys_small))


def test_power_of_two_M_B_checks(tmp_templates, keys_small, values_small):
    phf = import_fresh_phf(tmp_templates)

    # M not a power of two
    with pytest.raises(ValueError, match="power of two"):
        phf.PHF(keys_small, values_small, M=6)

    # B not a power of two
    with pytest.raises(ValueError, match="power of two"):
        phf.PHF(keys_small, values_small, B=3)

    # B > M
    with pytest.raises(ValueError, match="must not exceed M"):
        phf.PHF(keys_small, values_small, M=8, B=16)


@pytest.mark.parametrize("parallel", [False, True])
def test_build_best_any_basic(tmp_templates, keys_small, values_small, parallel):
    phf = import_fresh_phf(tmp_templates)
    h = phf.PHF(keys_small, values_small, seed=123, trials=2, target_load=0.6)
    best = h.build_best_any(parallel=parallel, trials_per_B=2)

    assert isinstance(best, phf.BestConfig)
    art = best.artifacts
    assert isinstance(art, phf.TrialArtifacts)
    # K should contain exactly our keys and empties (depending on M)
    assert set(k for k in art.K if k in keys_small) == set(keys_small)
    # Consistency of D size and M size
    assert len(art.D) == art.P.B
    assert len(art.K) == art.P.M
    assert len(art.V) == art.P.M


def test_determinism_serial_vs_parallel(tmp_templates, keys_small, values_small):
    phf = import_fresh_phf(tmp_templates)
    # Same seed & parameters should yield same BestConfig regardless of mode
    h1 = phf.PHF(keys_small, values_small, seed=999, trials=2, target_load=0.6)
    best1 = h1.build_best_any(parallel=False, trials_per_B=2)

    h2 = phf.PHF(keys_small, values_small, seed=999, trials=2, target_load=0.6)
    best2 = h2.build_best_any(parallel=True, trials_per_B=2)

    # Check a few immutable fields for equality
    a1, a2 = best1.artifacts, best2.artifacts
    assert a1.P.B == a2.P.B
    assert a1.P.M == a2.P.M
    assert a1.D == a2.D
    assert a1.K == a2.K
    assert a1.V == a2.V


@pytest.mark.parametrize("number_format", ["dec", "hex16"])
def test_code_generation_renders_with_minimal_templates(
    tmp_templates, keys_small, values_small, number_format
):
    phf = import_fresh_phf(tmp_templates)

    h = phf.PHF(keys_small, values_small, seed=42, trials=2, target_load=0.6)
    best = h.build_best_any(parallel=False, trials_per_B=2)

    # Ensure we use the new template dir for rendering
    # Reset env to force re-creation with new loader
    h.env = None

    hpp = h.get_hpp(namespace="phf_test")
    assert "namespace phf_test" in hpp
    assert "using displace_t" in hpp
    assert "using value_t" in hpp

    cpp = h.get_cpp(
        namespace="phf_test",
        header_file="phf_lookup.hpp",
        number_format=number_format,
    )
    assert "include \"phf_lookup.hpp\"" in cpp
    assert "sizes: D=" in cpp
    assert "key-bits=" in cpp


def test_probe_and_find_consistency(tmp_templates, keys_small, values_small):
    phf = import_fresh_phf(tmp_templates)
    h = phf.PHF(keys_small, values_small, seed=17, trials=2, target_load=0.6)
    best = h.build_best_any(parallel=False, trials_per_B=2)
    art = best.artifacts

    # Every key must be found and return its assigned value
    for k, v in zip(h.keys, h.values):
        i = phf.PHF.probe_index(k, art)
        assert 0 <= i < art.P.M
        ok, val = phf.PHF.find_py(k, art)
        assert ok is True
        assert val == v

    # A key not present should not be found
    ok, val = phf.PHF.find_py(123456, art)
    assert ok is False
    assert val == 0


def test_value_type_selection_uint8_uint16_uint32(tmp_templates):
    phf = import_fresh_phf(tmp_templates)

    keys = [10, 20, 30, 40]

    # Case 1: values fit in uint8
    vals1 = [0, 10, 200, 255]
    h1 = phf.PHF(keys, vals1, seed=1, trials=1)
    best1 = h1.build_best_any(parallel=False, trials_per_B=1)
    dc1, vc1 = h1._resolve_ctypes(best1.artifacts, displace_ctype=None, values_ctype=None)
    assert vc1 == "uint8_t"

    # Case 2: values fit in uint16 (but exceed 255)
    vals2 = [0, 256, 65535, 1000]
    h2 = phf.PHF(keys, vals2, seed=2, trials=1)
    best2 = h2.build_best_any(parallel=False, trials_per_B=1)
    dc2, vc2 = h2._resolve_ctypes(best2.artifacts, displace_ctype=None, values_ctype=None)
    assert vc2 == "uint16_t"

    # Case 3: values require uint32
    vals3 = [0, 65536, 70000, 2**31]
    h3 = phf.PHF(keys, vals3, seed=3, trials=1)
    best3 = h3.build_best_any(parallel=False, trials_per_B=1)
    dc3, vc3 = h3._resolve_ctypes(best3.artifacts, displace_ctype=None, values_ctype=None)
    assert vc3 == "uint32_t"


def test_build_not_found_raises_with_tiny_budget(tmp_templates, keys_small, values_small, monkeypatch):
    """
    Force an impossibly small max_attempts to trigger failure path in _build_search_fixed.
    This is artificial but validates the error handling.
    """
    phf = import_fresh_phf(tmp_templates)
    h = phf.PHF(keys_small, values_small, seed=123, trials=1, target_load=0.9)

    # Monkeypatch _build_best_core to call with absurdly small attempts
    with pytest.raises(RuntimeError, match="Failed to find parameters"):
        _ = h._build_search_fixed(
            items=h.items,
            M=h.M,
            B=h.B,
            rng=__import__("random").Random(0),
            max_attempts=1,  # deliberately too small
        )
