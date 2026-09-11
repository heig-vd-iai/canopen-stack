"""Tests for the Access model and its methods."""

# pylint: disable=missing-function-docstring
from typing import cast

import pytest

from generator.validation import Datatype, Var
from generator.validation.models.config import Config, SchemaConfig


@pytest.fixture
def sample_config_valid():
    """Sample configuration for testing."""
    return {
        "device": {
            "vendor": {
                "name": "CiA",
                "number": 0x0001,
            },
            "product": {
                "name": "CiA Product",
                "number": 0x0002,
            },
            "revision": "1.0.0",
            "baudrate": [125, 250, 500],
        },
        "profiles": [300, 301, 302],
        "objects": {
            0x1000: {
                "name": "Var",
                "datatype": "int16",
                "default": 42,
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
                        "enum": {"typedef": "EnumType", "values": {"FOO": 0, "BAR": 1}},
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
    }


@pytest.fixture
def sample_config_invalid():
    """Sample configuration for testing."""
    return {
        "device": {
            "vendor": {
                "name": "C" * 500,
                "number": 0x0001,
            },
            "product": {
                "name": "CiA Product",
                "number": 0x0002,
            },
            "revision": "1.0.512",
            "baudrate": [125, 250, 819],
        },
        "profiles": [300, 301, 302, 999],
        "objects": {
            0x1000: {
                "name": "Var",
                "datatype": "int42",
                "default": 42,
                "limits": {"min": 100, "max": 0},
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
                        "default": 1.12,
                        "access": "r",
                        "enum": {"typedef": "EnumType", "values": {"FOO": 1, "BAR": 1}},
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
    }


def test_schema_config_validation(sample_config_valid):
    """Test that the SchemaConfig validates a correct configuration."""
    config = SchemaConfig.model_validate(sample_config_valid)
    assert config.device.vendor.name == "CiA"
    assert config.profiles == [300, 301, 302]
    assert len(config.objects) == 3
    assert config.objects[0x1000].name == "Var"
    assert config.objects[0x1000].type == "var"
    assert isinstance(config.objects[0x1000], Var)
    v = cast(Var, config.objects[0x1000])
    assert isinstance(v.datatype, Datatype)
    assert v.datatype.name == "int16"

    assert config.objects[0x2000].type == "array"
    assert config.objects[0x3000].type == "record"


def test_schema_config_validation_invalid(sample_config_invalid):
    """Test that the SchemaConfig validates a correct configuration."""
    config, errors = Config(sample_config_invalid)
    assert config is None
    assert len(errors) > 0

    def error_on(path, errors):
        """Helper to find the first error on a given path."""
        for error in errors:
            if error["loc"] == path:
                return error
        return {}

    assert error_on(("device", "vendor", "name"), errors)["type"] == "string_too_long"
    assert (
        error_on(("device", "revision", "patch"), errors)["type"] == "less_than_equal"
    )
    assert error_on(("device", "baudrate"), errors)["type"] == "value_error"
    assert error_on(("profiles", 3), errors)["type"] == "less_than_equal"
    assert (
        error_on(("objects", 12288, "record", "record", 0, "enum", "values"), errors)[
            "type"
        ]
        == "value_error"
    )
