"""Mechanical migration of schema v1 files to schema v2."""

import pytest

from generator.migrate import (
    HexInt,
    dump,
    migrate_config,
    migrate_object,
    migrate_profiles,
)

V1_PROFILES = {
    "functionalities": {
        "baudrate": {125: True, 250: False, 1000: True},
        "simpleBootUpMaster": False,
        "simpleBootUpSlave": True,
        "granularity": 8,
        "dynamicChannelsSupported": False,
        "compactPDO": False,
        "groupMessaging": False,
        "LSS_Supported": False,
    },
    "profiles": {
        301: {
            "name": "Communication",
            "objects": {
                0x1000: {
                    "name": "Device type",
                    "category": "mandatory",
                    "data": [{"type": "uint32", "access": "ro"}],
                },
                0x1018: {
                    "name": "Identity object",
                    "get": "node.identity.getData",
                    "data": [
                        {
                            "type": "uint8",
                            "name": "Number of entries",
                            "access": "ro",
                            "default": 2,
                        },
                        {"type": "uint32", "name": "Vendor-ID", "access": "ro"},
                        {"type": "uint32", "name": "Product code", "access": "ro"},
                    ],
                },
                0x1600: {
                    "name": "RPDO mapping",
                    "data": [
                        {"type": "uint8", "name": "Number of entries", "access": "rw"},
                        {"type": "uint32", "name": "1st object", "access": "rw"},
                        {"type": "uint32", "name": "2nd object", "access": "rw"},
                    ],
                },
                0x60E3: {
                    "name": "Supported homing methods",
                    "data": [
                        {
                            "type": "uint8",
                            "name": "Highest sub-index supported",
                            "access": "ro",
                            "default": 1,
                            "length": 255,
                        },
                        {"type": "int16", "name": "?#st method", "access": "ro"},
                    ],
                },
            },
        }
    },
}

V1_CONFIG = {
    "info": {
        "fileVersion": 1,
        "fileRevision": 2,
        "description": "A device",
        "createdBy": "me",
        "creationTime": "10:00",
        "creationDate": "2026-09-11",
        "modifiedBy": "me",
        "device": {
            "vendorName": "HEIG-VD",
            "vendorNumber": 0,
            "productName": "minimal",
            "productNumber": 1,
            "revisionNumber": 1,
            "orderCode": 0,
            "nodeID": 1,
        },
    },
    "logicalDevices": {0: 402},
    "objectDictionary": {
        0x1000: {"profile": 301, "module": "cia301"},
        0x1018: {
            "profile": 301,
            "module": "cia301",
            "data": [{"default": 2}, {"default": 7}],
        },
        0x2005: {
            "name": "A remote value",
            "module": "fixture",
            "remote": True,
            "get": "remote.getValue",
            "set": "remote.setValue",
            "data": [{"type": "uint32", "access": "rw", "default": 0}],
        },
        0x2006: {
            "name": "A remote attribute",
            "module": "fixture",
            "remote": True,
            "attribute": "remote.attribute",
            "scale": 0.001,
            "data": [{"type": "float32", "access": "rw", "default": 0}],
        },
        0x2007: {
            "name": "An enumerated value",
            "module": "fixture",
            "enum": {"class": "Level", "data": {"LOW": 0, "HIGH": 1}},
            "data": [{"type": "uint8", "access": "rw", "default": 0}],
        },
        0x2001: {
            "name": "A string",
            "module": "fixture",
            "data": [{"type": "string", "access": "ro", "length": 16}],
        },
        0x2003: {
            "name": "Limited",
            "module": "fixture",
            "data": [
                {
                    "type": "uint16",
                    "access": "rw",
                    "default": 50,
                    "lowLimit": 10,
                    "highLimit": 100,
                }
            ],
        },
    },
}


def test_migrate_config_device_and_info():
    v2 = migrate_config(V1_CONFIG, V1_PROFILES)
    assert v2["device"]["vendor"] == {"name": "HEIG-VD", "number": 0}
    assert v2["device"]["revision"] == 1
    assert v2["device"]["node_id"] == 1
    assert v2["device"]["baudrate"] == [125, 1000]
    assert v2["device"]["simple_bootup_slave"] is True
    assert v2["info"]["file_revision"] == 2
    assert v2["info"]["created_at"].isoformat() == "2026-09-11T10:00:00"
    assert v2["profiles"] == [301, 402]


def test_migrate_config_objects():
    objects = migrate_config(V1_CONFIG, V1_PROFILES)["objects"]
    assert objects[0x1000] == {}
    assert objects[0x1018] == {"record": [{"default": 7}]}
    assert objects[0x2005] == {
        "name": "A remote value",
        "module": "fixture",
        "remote": "cpu1",
        "get": "remote.getValue()",
        "set": "remote.setValue(@)",
        "datatype": "uint32",
        "access": "rw",
        "default": 0,
    }
    assert objects[0x2006]["get"] == "remote.attribute"
    assert objects[0x2006]["set"] == "remote.attribute"
    assert objects[0x2006]["scale"] == 0.001
    assert objects[0x2007]["enum"] == {
        "typedef": "Level",
        "values": {"LOW": 0, "HIGH": 1},
    }
    assert objects[0x2001] == {
        "name": "A string",
        "module": "fixture",
        "datatype": "string",
        "access": "r",
        "size": 16,
    }
    assert objects[0x2003]["limits"] == {"min": 10, "max": 100}


def test_migrate_profiles():
    v2 = migrate_profiles(V1_PROFILES)[301]["profiles"][301]
    assert v2["name"] == "Communication"
    objects = v2["objects"]
    assert objects[0x1000] == {
        "name": "Device type",
        "category": "mandatory",
        "datatype": "uint32",
        "access": "r",
    }
    identity = objects[0x1018]
    assert identity["get"] == "node.identity.getData"
    assert identity["sub0"] == {"name": "Number of entries"}
    assert identity["record"][0] == {
        "name": "Vendor-ID",
        "datatype": "uint32",
        "access": "r",
    }
    mapping = objects[0x1600]
    assert mapping["sub0"] == {
        "name": "Number of entries",
        "access": "rw",
        "default": 0,
    }
    homing = objects[0x60E3]
    assert homing["length"] == 255
    assert homing["datatype"] == "int16"
    assert homing["sub0"] == {"default": 1}
    assert homing["array"] == [{"name": "#st method", "access": "r"}]


def test_migrate_object_rejects_logical_devices():
    with pytest.raises(ValueError, match="logicalDevices"):
        migrate_object(
            0x6040, {"name": "x", "logicalDevices": [0, 1], "data": []}, None
        )


def test_dump_writes_hex_indices():
    text = dump({"objects": {HexInt(0x1A00): {"name": "x"}}})
    assert "0x1A00:" in text
