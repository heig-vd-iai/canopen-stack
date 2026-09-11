"""Migration of schema v1 files (canopen_generator) to schema v2 (generator)."""

from datetime import datetime
from pathlib import Path
from typing import Dict, Optional

import yaml

ACCESS = {"ro": "r", "rw": "rw", "wo": "w", "const": "r"}
NONE = "none"
V1_INDEX_PLACEHOLDER = "?#"

SCALAR_KEYS = (
    "type",
    "access",
    "pdo_mapping",
    "default",
    "lowLimit",
    "highLimit",
    "length",
    "unit",
    "scale",
    "enum",
    "get",
    "set",
    "attribute",
)
SUB0_KEYS = ("name", "access", "default")
SUB0_DEFAULT_NAME = "Highest sub-index supported"


class HexInt(int):
    """Integer written in hexadecimal by the YAML dumper."""


class MultilineStr(str):
    """String written as a literal block by the YAML dumper."""


class Dumper(yaml.SafeDumper):
    def ignore_aliases(self, data):
        return True


Dumper.add_representer(
    HexInt,
    lambda dumper, data: dumper.represent_scalar(
        "tag:yaml.org,2002:int", f"0x{data:04X}"
    ),
)
Dumper.add_representer(
    MultilineStr,
    lambda dumper, data: dumper.represent_scalar(
        "tag:yaml.org,2002:str", str(data), style="|"
    ),
)


def dump(data: dict) -> str:
    return yaml.dump(
        data, Dumper=Dumper, sort_keys=False, allow_unicode=True, width=100
    )


def _given(mapping: dict, key: str):
    value = mapping.get(key)
    if value is None or value == NONE or value == "":
        return None
    return value


def _text(value: str):
    return MultilineStr(value) if "\n" in value else value


def migrate_enum(v1: dict) -> dict:
    if "class" in v1 and "data" in v1:
        typedef, data = v1["class"], v1["data"]
    else:
        raise ValueError(f"enum without 'class' cannot be migrated: {v1}")
    values = {}
    for key, item in data.items():
        if isinstance(item, int):
            values[key] = item
            continue
        entry = {"value": item["value"]}
        if _given(item, "name"):
            entry["name"] = item["name"]
        if _given(item, "documentation"):
            entry["description"] = item["documentation"]
        values[key] = entry
    return {"typedef": typedef, "values": values}


def migrate_accessors(v1: dict, remote: bool, access: Optional[str]) -> dict:
    """v1 get/set/attribute to v2 get/set.

    Local accessors stay function names. Remote getters become calls,
    remote setters take the value through '@', attributes become plain
    expressions read or assigned according to the access rights.
    """
    out = {}
    get, set_, attribute = _given(v1, "get"), _given(v1, "set"), _given(v1, "attribute")
    if get:
        out["get"] = f"{get}()" if remote else get
    if set_:
        out["set"] = f"{set_}(@)" if remote else set_
    if attribute:
        if access in (None, "ro", "rw", "const"):
            out["get"] = attribute
        if access in (None, "rw", "wo"):
            out["set"] = attribute
    return {k: v.replace(V1_INDEX_PLACEHOLDER, "0") for k, v in out.items()}


def migrate_scalar(data: dict, remote: bool) -> dict:
    """Fields of one v1 data entry, as v2 variable fields."""
    out = {}
    if "type" in data:
        out["datatype"] = data["type"]
    if _given(data, "access"):
        out["access"] = ACCESS[data["access"]]
    if "pdo_mapping" in data:
        out["pdo"] = bool(data["pdo_mapping"])
    if "default" in data:
        out["default"] = data["default"]
    if _given(data, "lowLimit") is not None or _given(data, "highLimit") is not None:
        out["limits"] = {"min": data.get("lowLimit"), "max": data.get("highLimit")}
    if data.get("type") == "string" and "length" in data:
        out["size"] = data["length"]
    if _given(data, "unit"):
        out["unit"] = data["unit"]
    if _given(data, "scale") not in (None, 1):
        out["scale"] = data["scale"]
    if "enum" in data:
        out["enum"] = migrate_enum(data["enum"])
    out.update(migrate_accessors(data, remote, data.get("access")))
    return out


def _entry(data: dict, remote: bool) -> dict:
    out = {}
    if _given(data, "name"):
        out["name"] = data["name"].replace(V1_INDEX_PLACEHOLDER, "#")
    out.update(migrate_scalar(data, remote))
    return out


def _sub0(data: dict, entry_count: int) -> dict:
    """Sub-index 0 fields that differ from what the v2 schema synthesizes."""
    unsupported = {
        k for k in data if k not in SUB0_KEYS and k not in ("type", "length")
    }
    if unsupported:
        raise ValueError(
            f"sub-index 0 only supports name/access/default, got {unsupported}"
        )
    out = {}
    if _given(data, "name") and data["name"] != SUB0_DEFAULT_NAME:
        out["name"] = data["name"]
    if _given(data, "access") and ACCESS[data["access"]] != "r":
        out["access"] = ACCESS[data["access"]]
    default = data.get("default", 0)
    if default != entry_count:
        out["default"] = default
    return out


def migrate_data(data: list, remote: bool, shape: Optional[list] = None) -> dict:
    """A v1 data list as v2 var/record/array fields.

    `shape` is the data list of the profile object when the object only
    overrides it: the layout comes from the profile, not from `data`.
    """
    layout = shape if shape is not None else data
    if not data:
        return {}
    if len(layout) == 1:
        return migrate_scalar(data[0], remote)

    out = {}
    expansion = layout[0].get("length", 1)
    entry_count = expansion if expansion > 1 else len(layout) - 1
    sub0 = _sub0(data[0], entry_count)
    if sub0:
        out["sub0"] = sub0
    entries = [_entry(d, remote) for d in data[1:]]
    if expansion > 1:
        out["length"] = expansion
        if shape is None:
            out["datatype"] = entries[0].pop("datatype")
        out["array"] = entries
    else:
        out["record"] = entries
    return out


def migrate_object(index: int, obj: dict, profile_obj: Optional[dict]) -> dict:
    """One v1 object dictionary entry as a v2 object."""
    if "logicalDevices" in obj:
        raise ValueError(f"0x{index:04X}: per-object logicalDevices are not supported")

    remote = bool(obj.get("remote", False))
    data = obj.get("data", [])
    shape = profile_obj["data"] if profile_obj else None
    first_access = (data[0] if data else (shape[0] if shape else {})).get("access")

    out = {}
    if _given(obj, "name"):
        out["name"] = obj["name"]
    if _given(obj, "description"):
        out["description"] = _text(obj["description"])
    if _given(obj, "category") and obj["category"] != "optional":
        out["category"] = obj["category"]
    profile = obj.get("profile", 0)
    module = _given(obj, "module")
    if module and module != f"cia{profile}":
        out["module"] = module
    if remote:
        out["remote"] = "cpu1"
    if _given(obj, "unit"):
        out["unit"] = obj["unit"]
    if _given(obj, "scale") not in (None, 1):
        out["scale"] = obj["scale"]
    if "enum" in obj:
        out["enum"] = migrate_enum(obj["enum"])
    out.update(migrate_accessors(obj, remote, first_access))
    out.update(migrate_data(data, remote, shape))
    return out


def migrate_device(info: dict, functionalities: Optional[dict]) -> dict:
    device = info.get("device", {})
    out = {
        "vendor": {
            "name": device.get("vendorName", ""),
            "number": device.get("vendorNumber", 0),
        },
        "product": {
            "name": device.get("productName", ""),
            "number": device.get("productNumber", 0),
        },
        "revision": device.get("revisionNumber", 0),
        "order_code": str(device.get("orderCode", "")),
        "node_id": device.get("nodeID", 1),
    }
    if functionalities:
        out["baudrate"] = [
            rate for rate, on in functionalities.get("baudrate", {}).items() if on
        ]
        out["lss_supported"] = bool(functionalities.get("LSS_Supported", False))
        out["simple_bootup_slave"] = bool(
            functionalities.get("simpleBootUpSlave", False)
        )
        out["simple_bootup_master"] = bool(
            functionalities.get("simpleBootUpMaster", False)
        )
        out["granularity"] = functionalities.get("granularity", 8)
        out["dynamic_channels_supported"] = bool(
            functionalities.get("dynamicChannelsSupported", False)
        )
        out["compact_pdo"] = bool(functionalities.get("compactPDO", False))
        out["group_messaging"] = bool(functionalities.get("groupMessaging", False))
    return out


def migrate_info(info: dict) -> dict:
    out = {}
    if _given(info, "description"):
        out["description"] = info["description"]
    if "fileVersion" in info:
        out["file_version"] = info["fileVersion"]
    if "fileRevision" in info:
        out["file_revision"] = info["fileRevision"]
    if _given(info, "createdBy"):
        out["created_by"] = info["createdBy"]
    if _given(info, "creationDate"):
        stamp = f"{info['creationDate']} {info.get('creationTime', '00:00')}"
        out["created_at"] = datetime.strptime(stamp, "%Y-%m-%d %H:%M")
    if _given(info, "modifiedBy"):
        out["modified_by"] = info["modifiedBy"]
    return out


def migrate_config(v1: dict, v1_profiles: dict) -> dict:
    """A v1 configuration mapping as a v2 configuration mapping."""
    info = v1.get("info", {})
    logical = v1.get("logicalDevices", {})
    if len(logical) > 1:
        raise ValueError("several logical devices are not supported")

    objects = v1.get("objectDictionary", {})
    profiles = {301} | set(logical.values())
    profiles |= {obj.get("profile", 0) for obj in objects.values()} - {0}

    v1_profile_objects = {
        pid: p.get("objects", {}) for pid, p in v1_profiles.get("profiles", {}).items()
    }

    out = {
        "device": migrate_device(info, v1_profiles.get("functionalities")),
        "info": migrate_info(info),
        "profiles": sorted(profiles),
        "objects": {},
    }
    for index, obj in objects.items():
        profile = obj.get("profile", 0)
        profile_obj = (
            v1_profile_objects.get(profile, {}).get(index) if profile else None
        )
        if profile and profile_obj is None:
            raise ValueError(f"0x{index:04X}: not found in v1 profile {profile}")
        out["objects"][HexInt(index)] = migrate_object(index, obj, profile_obj)
    return out


def migrate_profiles(v1_profiles: dict) -> Dict[int, dict]:
    """A v1 profiles mapping as one v2 profile mapping per profile number."""
    out = {}
    for pid, profile in v1_profiles.get("profiles", {}).items():
        objects = {
            HexInt(index): migrate_object(index, obj, None)
            for index, obj in profile.get("objects", {}).items()
        }
        out[pid] = {"profiles": {pid: {"name": profile["name"], "objects": objects}}}
    return out


def write_yaml(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(dump(data), encoding="utf-8")
