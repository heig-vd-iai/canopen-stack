"""Loading a v2 configuration merges the profile definitions."""

from pathlib import Path

import pytest
from pydantic import ValidationError

from generator.loaders import load_config, load_config_dict, merge_object

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "examples" / "minimal.v2.yaml"


@pytest.fixture(scope="module")
def config():
    return load_config(CONFIG)


def test_profiles_and_device(config):
    assert config.profiles == [301, 402]
    assert config.device_profile == 402
    assert config.device.node_id == 1
    assert config.info.file_version == 1


def test_profile_object_is_completed(config):
    heartbeat = config.objects[0x1017]
    assert heartbeat.name == "Producer hearbeat time"
    assert heartbeat.module == "cia301"
    assert heartbeat.category == "mandatory"
    assert heartbeat.default == 1000
    assert heartbeat.get == "hbGetData"


def test_record_override_by_position(config):
    identity = config.objects[0x1018].subentries()
    assert [e.default for e in identity] == [4, 0, 1, 1, 0]
    assert identity[1].name == "Vendor-ID"


def test_manufacturer_objects(config):
    remote = config.objects[0x2005]
    assert remote.is_remote
    assert remote.get == "remote.getValue()"
    assert remote.set == "remote.setValue(@)"

    string = config.objects[0x2001]
    assert string.size == 16
    assert string.default == ""

    limited = config.objects[0x2003]
    assert limited.limited
    assert (limited.limits.min, limited.limits.max) == (10, 100)


def test_enum_override_on_profile_object(config):
    modes = config.objects[0x6060]
    assert modes.enum is not None
    assert modes.enum.typedef == "OperationModes"
    assert modes.datatype.name == "int16"


def test_merge_object_keeps_profile_entries():
    base = {
        "name": "Base",
        "record": [
            {"name": "a", "datatype": "uint8"},
            {"name": "b", "datatype": "uint8"},
        ],
        "sub0": {"access": "rw"},
    }
    override = {"record": [{"default": 3}], "sub0": {"default": 0}}
    merged = merge_object(base, override)
    assert merged["record"] == [
        {"name": "a", "datatype": "uint8", "default": 3},
        {"name": "b", "datatype": "uint8"},
    ]
    assert merged["sub0"] == {"access": "rw", "default": 0}


def test_unknown_profile_object_needs_full_definition():
    raw = {"device": {}, "profiles": [301], "objects": {0x2000: {"datatype": "uint8"}}}
    with pytest.raises(ValidationError):
        load_config_dict(raw)


def test_string_needs_size():
    raw = {
        "device": {},
        "profiles": [],
        "objects": {0x2000: {"name": "s", "datatype": "string"}},
    }
    with pytest.raises(ValidationError, match="size"):
        load_config_dict(raw)


def test_node_id_default_is_accepted():
    raw = {
        "device": {},
        "profiles": [],
        "objects": {
            0x2000: {"name": "c", "datatype": "uint32", "default": "$NODEID+0x180"}
        },
    }
    assert load_config_dict(raw).objects[0x2000].default == "$NODEID+0x180"
