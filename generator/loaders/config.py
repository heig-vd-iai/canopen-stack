"""Loading of a device configuration, completed by its CiA profiles."""

import copy
from pathlib import Path
from typing import Dict, Optional, Sequence, Union

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


def normalize_index(index: Union[int, str]) -> int:
    """Return an object index as an integer, accepting "0x1234" strings."""
    return index if isinstance(index, int) else int(str(index), 0)


def normalize_objects(objects: dict) -> Dict[int, dict]:
    """Return the objects keyed by integer index."""
    return {normalize_index(index): obj for index, obj in objects.items()}


def merge_device(base: dict, overlay: dict) -> dict:
    """Merge device mappings recursively, overlay leaves win."""
    merged = dict(base)
    for key, value in overlay.items():
        if isinstance(value, dict) and isinstance(merged.get(key), dict):
            merged[key] = merge_device(merged[key], value)
        else:
            merged[key] = copy.deepcopy(value)
    return merged


def enum_value(entry: Union[int, dict]) -> Optional[int]:
    """Return the numeric value of an enum entry, short or long form."""
    return entry.get("value") if isinstance(entry, dict) else entry


def enum_values(obj: dict) -> Optional[dict]:
    """Return the enum values of a raw object, if any."""
    enum = obj.get("enum")
    return enum.get("values") if isinstance(enum, dict) else None


def is_enum_extension(obj: dict) -> bool:
    """Tell whether an overlay object only adds enum values."""
    enum = obj.get("enum")
    return (
        set(obj) == {"enum"}
        and isinstance(enum, dict)
        and set(enum) == {"values"}
        and isinstance(enum["values"], dict)
    )


def merge_enum_values(base: dict, extra: dict, index: int, source: Path) -> dict:
    """Add enum values, refusing any name or numeric value already used."""
    merged = dict(base)
    used = {enum_value(entry): name for name, entry in base.items()}
    for name, entry in extra.items():
        if name in merged:
            raise ValueError(
                f"0x{index:04X} enum value {name} already defined ({source})"
            )
        value = enum_value(entry)
        if value in used:
            raise ValueError(
                f"0x{index:04X} enum value {name} = {value} already used by "
                f"{used[value]} ({source})"
            )
        merged[name] = copy.deepcopy(entry)
        used[value] = name
    return merged


def is_entry_extension(entry: object) -> bool:
    """Tell whether an overlay record entry only names an entry and adds enum values."""
    return (
        isinstance(entry, dict)
        and isinstance(entry.get("name"), str)
        and is_enum_extension(
            {key: value for key, value in entry.items() if key != "name"}
        )
    )


def is_record_extension(obj: dict) -> bool:
    """Tell whether an overlay object only adds enum values to record entries."""
    record = obj.get("record")
    return (
        set(obj) == {"record"}
        and isinstance(record, list)
        and len(record) > 0
        and all(is_entry_extension(entry) for entry in record)
    )


def extend_enum(base: dict, extra: dict, index: int, source: Path) -> dict:
    """Return a copy of an object whose enum holds the extra values."""
    extended = merge_enum_values(enum_values(base), extra, index, source)
    return {**base, "enum": {**base["enum"], "values": extended}}


def find_entry(record: list, name: str, index: int, source: Path) -> int:
    """Return the position of the record entry with this name."""
    for position, entry in enumerate(record):
        if isinstance(entry, dict) and entry.get("name") == name:
            return position
    raise ValueError(f"0x{index:04X} has no record entry named {name!r} ({source})")


def extend_entry(entry: dict, extension: dict, index: int, source: Path) -> dict:
    """Return a copy of a record entry whose enum holds the overlay values."""
    if enum_values(entry) is None:
        raise ValueError(
            f"0x{index:04X} record entry {extension['name']!r} has no enum "
            f"to extend ({source})"
        )
    return extend_enum(entry, extension["enum"]["values"], index, source)


def extend_record(base: dict, extensions: list, index: int, source: Path) -> dict:
    """Return a copy of a record object whose named entries hold the overlay values."""
    record = list(base["record"])
    for extension in extensions:
        position = find_entry(record, extension["name"], index, source)
        record[position] = extend_entry(record[position], extension, index, source)
    return {**base, "record": record}


def merge_overlay_object(base: dict, overlay: dict, index: int, source: Path) -> dict:
    """Extend an existing object, or its record entries, with overlay enum values."""
    base, overlay = base or {}, overlay or {}
    if is_enum_extension(overlay) and enum_values(base) is not None:
        return extend_enum(base, overlay["enum"]["values"], index, source)
    if is_record_extension(overlay) and isinstance(base.get("record"), list):
        return extend_record(base, overlay["record"], index, source)
    raise ValueError(
        f"0x{index:04X} already defined, an overlay may only add enum values "
        f"to an existing object ({source})"
    )


def apply_overlay(raw: dict, overlay: dict, source: Path) -> dict:
    """Return the raw configuration with one overlay merged in."""
    merged = dict(raw)
    if overlay.get("device"):
        merged["device"] = merge_device(raw.get("device") or {}, overlay["device"])
    objects = normalize_objects(raw.get("objects") or {})
    for index, obj in normalize_objects(overlay.get("objects") or {}).items():
        if index in objects:
            objects[index] = merge_overlay_object(objects[index], obj, index, source)
        else:
            objects[index] = copy.deepcopy(obj)
    merged["objects"] = objects
    return merged


def apply_overlays(raw: dict, overlays: Sequence[Path]) -> dict:
    """Return a copy of the raw configuration with every overlay merged in."""
    merged = copy.deepcopy(raw)
    for source in overlays:
        merged = apply_overlay(merged, read_yaml_file(source), Path(source))
    return merged


def load_raw_config(filename: Union[str, Path], overlays: Sequence[Path] = ()) -> dict:
    """Read a configuration file and merge its overlays, without validation."""
    return apply_overlays(read_yaml_file(filename), overlays)


def load_config(
    filename: Union[str, Path],
    profile_dir: Optional[Path] = None,
    overlays: Sequence[Path] = (),
) -> SchemaConfig:
    """Read and validate a configuration file, extended by its overlays."""
    return load_config_dict(load_raw_config(filename, overlays), profile_dir)
