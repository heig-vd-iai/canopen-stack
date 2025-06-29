import pickle
import re
import warnings
from pathlib import Path
from typing import Dict, List, Optional

from ..validation.profile import Profiles
from .yaml_config import read_config_file

PROFILE_DIR = Path(__file__).parent.parent / "profiles"


class ProfileLoader:
    """
    Loads all CANopen profiles from a local directory, validates them,
    and caches the result using pickle if up-to-date.
    """

    def __init__(
        self, profile_dir: Optional[Path] = None, cache_path: Optional[Path] = None
    ):
        self.profile_dir = profile_dir or PROFILE_DIR
        self.cache_path = cache_path or self.profile_dir / ".cache_profiles.pkl"

        self.profiles: Dict[int, "Profiles"] = {}
        self.warnings: Dict[int, List[str]] = {}
        self.errors: Dict[int, str] = {}

    def load_all(self, use_cache: bool = True) -> None:
        """Load and validate all profiles. Use cache if possible."""
        if use_cache and self._load_from_cache():
            return

        yaml_pattern = re.compile(r"^(\d+)\.yaml$")
        self.profiles.clear()
        self.warnings.clear()
        self.errors.clear()

        for file in self.profile_dir.glob("*.yaml"):
            if not file.is_file():
                continue
            match = yaml_pattern.match(file.name)
            if not match:
                continue
            profile_id = int(match.group(1))
            print(profile_id, file.name)

            # try:
            raw = read_config_file(file)

            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                profile = Profiles.model_validate(raw)

            self.profiles[profile_id] = profile
            self.warnings[profile_id] = [str(warn.message) for warn in w]

            # except ValidationError as e:
            #     self.errors[profile_id] = f"Validation error: {e}"
            # except Exception as e:
            #     self.errors[profile_id] = f"Exception while loading: {e}"

        self._save_to_cache()

    def _load_from_cache(self) -> bool:
        """Try to load from cache if up-to-date."""
        if not self.cache_path.exists():
            return False

        cache_mtime = self.cache_path.stat().st_mtime
        for file in self.profile_dir.glob("*.yaml"):
            if file.stat().st_mtime > cache_mtime:
                return False  # A file is newer than cache

        try:
            with self.cache_path.open("rb") as f:
                cache = pickle.load(f)
            self.profiles = cache["profiles"]
            self.warnings = cache["warnings"]
            self.errors = cache["errors"]
            return True
        except Exception:
            return False

    def _save_to_cache(self) -> None:
        """Save validated data to cache."""
        data = {
            "profiles": self.profiles,
            "warnings": self.warnings,
            "errors": self.errors,
        }
        try:
            with self.cache_path.open("wb") as f:
                pickle.dump(data, f)
        except Exception as e:
            print(f"Warning: failed to write profile cache: {e}")

    def summary(self) -> str:
        """Return a textual summary of all loaded profiles."""
        lines = [f"Loaded {len(self.profiles)} profile(s)."]

        if self.warnings:
            lines.append("\n=== WARNINGS ===")
            for pid, warns in self.warnings.items():
                lines.append(f"- Profile {pid}:")
                for w in warns:
                    lines.append(f"    * {w}")

        if self.errors:
            lines.append("\n=== ERRORS ===")
            for pid, err in self.errors.items():
                lines.append(f"- Profile {pid}: {err}")

        return "\n".join(lines)
