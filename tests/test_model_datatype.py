"""Tests for the Datatype model and its methods."""

# pylint: disable=missing-function-docstring
import pytest
from pydantic import BaseModel

from generator.validation import DATATYPES, Datatype


class Model(BaseModel):
    datatype: Datatype


def test_from_name_valid():
    """Datatype.from_name should create Datatype for valid names."""
    for name, info in DATATYPES.items():
        dt = Datatype.from_name(name)
        assert isinstance(dt, Datatype)
        assert dt.name == info.name
        assert dt.code == info.code
        assert dt.ctype == info.ctype
        assert dt.field == info.field


def test_integration_with_pydantic():
    m = Model(datatype="int16")  # type: ignore
    assert isinstance(m.datatype, Datatype)
    assert m.datatype.name == "int16"


def test_from_name_invalid():
    """Datatype.from_name should raise on invalid name."""
    with pytest.raises(ValueError) as e:
        Datatype.from_name("not_a_type")
    assert "Invalid CiA datatype" in str(e.value)


def test_model_validate_from_str():
    """Datatype should be created via model_validate from string."""
    dt = Datatype.model_validate("uint16")
    assert isinstance(dt, Datatype)
    assert dt.name == "uint16"
    assert dt.code == DATATYPES["uint16"].code


def test_model_validate_from_dict():
    """Datatype should accept dict input directly."""
    dct = {
        "name": "float32",
        "code": 0x08,
        "ctype": "float",
        "field": "f32",
    }
    dt = Datatype.model_validate(dct)
    assert isinstance(dt, Datatype)
    assert dt.name == "float32"
    assert dt.code == 0x08


def test_model_validate_from_instance():
    """Datatype model_validate should accept an instance and leave it unchanged."""
    original = Datatype.from_name("int32")
    dt = Datatype.model_validate(original)
    assert dt is original


def test_serialization_to_str():
    """Check that Datatype serializes to its name."""
    dt = Datatype.from_name("int64")
    serialized = Datatype._serialize(dt)  # pylint: disable=protected-access
    assert serialized == "int64"


@pytest.mark.parametrize("name", ["bool", "int8", "domain"])
def test_field_content(name):
    """Datatype attributes should match DATATYPES table."""
    dt = Datatype.from_name(name)
    info = DATATYPES[name]
    assert dt.name == info.name
    assert dt.code == info.code
    assert dt.ctype == info.ctype
    assert dt.field == info.field


def test_serialize_returns_name():
    dt = Datatype.from_name("float32")
    s = Datatype._serialize(dt)
    assert s == "float32"
