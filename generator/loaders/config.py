"""Loading of a device configuration, completed by its CiA profiles."""

import copy
from pathlib import Path
from typing import Dict, Optional, Union

from ..validation.models.config import SchemaConfig
from .profiles import profile_objects
from .yaml_config import read_yaml_file

ENTRY_LISTS = ("record", "array")
MERGED_MAPPINGS = ("sub0", "limits")


def merge_entries(base: list, override: list) -> list:
    """Merge entry lists position by position; extra override entries are kept."""
    merged = []
    for position in range(max(len(base), len(override))):
        base_entry = base[position] if position < len(base) else {}
        override_entry = override[position] if position < len(override) else {}
        merged.append({**base_entry, **override_entry})
    return merged


def merge_object(base: dict, override: dict) -> dict:
    """Complete a configuration object with its profile definition."""
    merged = {**base, **override}
    for key in ENTRY_LISTS:
        if key in base and key in override:
            merged[key] = merge_entries(base[key], override[key])
    for key in MERGED_MAPPINGS:
        if isinstance(base.get(key), dict) and isinstance(override.get(key), dict):
            merged[key] = {**base[key], **override[key]}
    return merged


def complete_with_profiles(
    raw: dict, profile_dir: Optional[Path] = None
) -> Dict[int, dict]:
    """Return the raw objects, each merged with its profile definition."""
    objects = copy.deepcopy(raw.get("objects", {}))
    for profile in raw.get("profiles", []):
        for index, definition in profile_objects(profile, profile_dir).items():
            if index not in objects:
                continue
            merged = merge_object(definition, objects[index] or {})
            merged.setdefault("module", f"cia{profile}")
            objects[index] = merged
    return objects


def load_config_dict(raw: dict, profile_dir: Optional[Path] = None) -> SchemaConfig:
    """Validate a raw configuration mapping."""
    data = {**raw, "objects": complete_with_profiles(raw, profile_dir)}
    return SchemaConfig.model_validate(data)


def load_config(
    filename: Union[str, Path], profile_dir: Optional[Path] = None
) -> SchemaConfig:
    """Read and validate a configuration file."""
    return load_config_dict(read_yaml_file(filename), profile_dir)
