import pytest
from pydantic import ValidationError

from generator.validation import (
    Access,
    Bitfield,
    Datatype,
    Enum,
    EnumEntry,
    Limits,
    Markdown,
    Var,
)


def test_var_minimal():
    """Test Var creation with minimal fields."""

    var = Var(name="myVar", datatype="uint16")

    assert var.type == "var"
    assert var.name == "myVar"
    assert isinstance(var.datatype, Datatype)
    assert var.datatype.name == "uint16"
    assert var.remote == "local"
    assert var.path == ""
    assert var.description == Markdown("")
    assert var.inherit is None
    assert var.limits.min is None
    assert var.limits.max is None
    assert var.pdo is False
    assert var.enum is None
    assert var.default == 0
    assert var.bitfield is None
    assert var.unit == ""
    assert var.scale is None
    assert isinstance(var.access, Access)
    assert var.access.read is False
    assert var.access.write is False
    assert var.get is None
    assert var.set is None


def test_var_full():
    """Test Var with all optional fields filled."""

    enum = Enum(
        **{
            "class": "TestEnum",
            "data": {
                "FIRST": {"name": "FIRST", "value": 1, "description": "First value"},
                "SECOND": {"name": "SECOND", "value": 2},
            },
        }
    )

    bitfield = Bitfield(
        {
            "31..16": "StatusHigh",
            "15..0": {"name": "StatusLow", "values": {0: "OFF", 1: "ON"}},
        }
    )

    var = Var(
        name="FullVar",
        remote="remote_device",
        path="SomePath.SubPath",
        description=Markdown("# Test description\n\nThis is a full variable test.\n"),
        inherit=123,
        datatype="int32",
        limits=Limits(min=-100, max=100),
        pdo=True,
        enum=enum,
        default=5,
        bitfield=bitfield,
        unit="V",
        scale=0.001,
        access="r",
        get="read_func",
        set="write_func",
    )

    assert var.name == "FullVar"
    assert var.remote == "remote_device"
    assert var.path == "SomePath.SubPath"
    assert isinstance(var.description, Markdown)
    assert var.inherit == 123
    assert isinstance(var.datatype, Datatype)
    assert var.datatype.name == "int32"
    assert var.limits.min == -100
    assert var.limits.max == 100
    assert var.pdo is True
    assert isinstance(var.enum, Enum)
    assert var.enum.data["FIRST"].name == "FIRST"
    assert var.default == 5
    assert isinstance(var.bitfield, Bitfield)
    assert var.unit == "V"
    assert var.scale == 0.001
    assert isinstance(var.access, Access)
    assert var.access.read is True
    assert var.access.write is False
    assert var.get == "read_func"
    assert var.set == "write_func"


def test_var_invalid_path():
    """Test that invalid path raises validation error."""

    with pytest.raises(ValidationError) as exc_info:
        Var(name="BadPathVar", datatype="uint8", path="invalid path with spaces")
    assert "String should match pattern" in str(exc_info.value)


def test_var_forbid_extra_fields():
    """Test that extra fields raise a validation error."""

    with pytest.raises(ValidationError) as exc_info:
        Var(name="ExtraFieldVar", datatype="uint8", foo="bar")  # type: ignore
    assert "Extra inputs are not permitted" in str(exc_info.value)


def test_var_enum_uniqueness():
    """Test that enum with duplicate values raises error."""

    # Duplicate value 1
    enum_data = {
        "FIRST": EnumEntry(name="FIRST", value=1),
        "SECOND": EnumEntry(name="SECOND", value=1),
    }
    with pytest.raises(ValidationError) as exc_info:
        Enum(**{"class": "TestEnum", "data": enum_data})

    assert "Enum values must be unique" in str(exc_info.value)


def test_var_bitfield_overlap():
    """Test that overlapping bitfields raise error."""

    with pytest.raises(ValueError) as exc_info:
        Bitfield(
            {
                "31..16": "Field1",
                "20..10": "Field2",  # overlaps with previous range
            }
        )

    assert "overlapping" in str(exc_info.value)


def test_var_serialization():
    """Test Var serialization to dict."""

    var = Var(
        name="SerializedVar",
        datatype="uint16",
    )

    dumped = var.model_dump()

    assert dumped["name"] == "SerializedVar"
    assert dumped["datatype"] == "uint16"
    assert dumped["type"] == "var"
