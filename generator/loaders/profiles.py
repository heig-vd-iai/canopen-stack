"""Loading of the CiA profile definitions shipped with the generator."""

import copy
from pathlib import Path
from typing import Dict, Optional

from ..validation.helpers import resolve_inheritance
from ..validation.models.profile import SchemaProfile, SchemaProfiles
from .yaml_config import read_yaml_file

PROFILE_DIR = Path(__file__).parent.parent / "profiles"

PROFILE_ONLY_KEYS = {"mandatory_conditions", "max_length"}
ENUM_PROFILE_ONLY_KEYS = {"allow_override", "override-ranges"}


def profile_path(profile: int, profile_dir: Optional[Path] = None) -> Path:
    path = (profile_dir or PROFILE_DIR) / f"{profile}.yaml"
    if not path.is_file():
        raise FileNotFoundError(f"Unknown CiA profile {profile}: {path} not found")
    return path


def read_profile(profile: int, profile_dir: Optional[Path] = None) -> dict:
    """Read a profile file and return the raw definition of that profile."""
    raw = read_yaml_file(profile_path(profile, profile_dir))
    profiles = raw.get("profiles", {})
    if profile not in profiles:
        raise ValueError(
            f"Profile file {profile}.yaml does not define profile {profile}"
        )
    return profiles[profile]


def load_profile(profile: int, profile_dir: Optional[Path] = None) -> SchemaProfile:
    """Read and validate a profile."""
    raw = read_yaml_file(profile_path(profile, profile_dir))
    return SchemaProfiles.model_validate(raw).profiles[profile]


def _strip_profile_only(obj: dict) -> dict:
    obj = {k: v for k, v in obj.items() if k not in PROFILE_ONLY_KEYS}
    if isinstance(obj.get("enum"), dict):
        obj["enum"] = {
            k: v for k, v in obj["enum"].items() if k not in ENUM_PROFILE_ONLY_KEYS
        }
    for key in ("record", "array"):
        if isinstance(obj.get(key), list):
            obj[key] = [_strip_profile_only(e) for e in obj[key]]
    return obj


def profile_objects(
    profile: int, profile_dir: Optional[Path] = None
) -> Dict[int, dict]:
    """Raw profile objects ready to be merged into a device configuration."""
    load_profile(profile, profile_dir)
    objects = copy.deepcopy(read_profile(profile, profile_dir).get("objects", {}))
    return {
        index: _strip_profile_only(obj)
        for index, obj in resolve_inheritance(objects).items()
    }
