"""Tests for the Access model and its methods."""

# pylint: disable=missing-function-docstring

import pytest

from generator.validation import VarProfile
from generator.validation.models.access import Access
from generator.validation.models.bitfields import Bitfield
from generator.validation.models.datatype import Datatype
from generator.validation.models.profile import SchemaProfiles


@pytest.fixture
def sample_config_valid():
    """Sample configuration for testing."""
    return {
        "profiles": {
            402: {
                "name": "CiA402",
                "description": "CiA 402 profile for drives and motion control",
                "objects": {
                    0x1000: {
                        "name": "Var",
                        "datatype": "int16",
                        "default": 42,
                        "bitfield": {
                            0: "Switch On",
                            1: "Enable Voltage",
                            2: "Quick Stop",
                            3: "Enable Operation",
                            "6..4": "Operation Mode Specific",
                            7: "Fault Reset",
                        },
                        "limits": {"min": 0, "max": 100},
                    },
                    0x2000: {
                        "name": "Array",
                        "description": "An array of integers",
                        "length": 5,
                        "datatype": "int32",
                        "array": [
                            {
                                "default": 0,
                                "limits": {"min": -1000, "max": 1000},
                                "access": "rw",
                            }
                        ],
                    },
                    0x3000: {
                        "name": "Record",
                        "description": "A record with variable fields",
                        "record": [
                            {
                                "name": "Field1",
                                "datatype": "uint8",
                                "default": 1,
                                "access": "r",
                                "enum": {
                                    "typedef": "EnumType",
                                    "values": {"FOO": 0, "BAR": 1},
                                },
                            },
                            {
                                "name": "Field2",
                                "datatype": "string",
                                "default": "default",
                                "access": "w",
                            },
                        ],
                    },
                },
            },
        },
    }


@pytest.fixture
def full_profile():
    return {
        "profiles": {
            402: {
                "name": "Profile for drives and motion control",
                "description": "Adjustable speed electrical power drive systems - Part 7-201:\nGeneric interface and use of profiles for power drive systems\n",
                "standards": ["EN 61800-7-201:2016", "IEC 61800-7-201:2015", "CiA 402"],
                "features": {
                    "vl": "Velocity Mode",
                    "cstca": "Current Mode (CST/CA)",
                    "pp": "Profile Position Mode",
                    "pv": "Profile Velocity Mode",
                    "ip": "Interpolated Position Mode",
                    "pc": "Position Mode",
                    "hm": "Homing Mode",
                    "csp": "Cyclic Synchronous Position Mode",
                    "tq": "Torque Mode",
                    "csv": "Cyclic Synchronous Velocity Mode",
                },
                "objects": {
                    1: {
                        "name": "Abort connection option code",
                        "description": "This object shall indicate what action shall be performed\nwhen one of the following events occurres:bus-off, heartbeat,\nlife guarding, NMT stopped state entered, reset application, and reset\ncommunication. Table 35 specifies the value definition,\nTable 36 specifies the object description,\nand Table 37 specifies the entry description.\n",
                        "datatype": "int16",
                        "access": "rw",
                        "default": 1,
                        "enum": {
                            "typedef": "AbortConnectionOptionCode",
                            "values": {
                                "ABORT_CONNECTION_OPTION_CODE__NO_ACTION": 0,
                                "ABORT_CONNECTION_OPTION_CODE__FAULT_SIGNAL": 1,
                                "ABORT_CONNECTION_OPTION_CODE__DISABLE_VOLTAGE_COMMAND": 2,
                                "ABORT_CONNECTION_OPTION_CODE__QUICK_STOP_COMMAND": 3,
                            },
                            "override-ranges": [[-32768, -1]],
                        },
                    },
                    2: {
                        "name": "Controlword",
                        "category": "mandatory",
                        "datatype": "uint16",
                        "access": "rw",
                        "pdo": True,
                        "default": 0,
                        "bitfield": {
                            0: "Switch On",
                            1: "Enable Voltage",
                            2: "Quick Stop",
                            3: "Enable Operation",
                            "6..4": "Operation Mode Specific",
                            7: "Fault Reset",
                            8: "Halt",
                            9: "Operation Manufacturer Specific",
                            "15..11": "Manufacturer Specific",
                        },
                    },
                    3: {
                        "name": "An array",
                        "description": "An array of integers",
                        "datatype": "int32",
                        "array": [
                            {
                                "name": "Array Entry",
                                "default": 42,
                                "limits": {"min": -1000, "max": 1000},
                            },
                            {
                                "name": "Another Entry",
                                "default": 100,
                                "limits": {"min": 0, "max": 200},
                            },
                        ],
                    },
                },
            }
        }
    }


def test_schema_config_validation(sample_config_valid):
    """Test that the SchemaConfig validates a correct configuration."""
    config = SchemaProfiles.model_validate(sample_config_valid)


def test_schema_config_validation2(full_profile):
    """Test that the SchemaConfig validates a correct configuration."""
    config = SchemaProfiles.model_validate(full_profile)
    assert isinstance(config, SchemaProfiles)
    obj_var = config.profiles[402].objects[2]

    assert isinstance(obj_var, VarProfile)
    assert obj_var.category == "mandatory"

    assert isinstance(obj_var.datatype, Datatype)
    assert obj_var.datatype.name == "uint16"

    assert isinstance(obj_var.access, Access)
    assert obj_var.access.read is True
    assert obj_var.access.write is True

    assert isinstance(obj_var.bitfield, Bitfield)
    assert obj_var.bitfield.entries[(1, 1)].name == "Enable Voltage"
