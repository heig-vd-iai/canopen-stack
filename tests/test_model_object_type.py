import pytest
from pydantic import ValidationError

from generator.validation.models import ObjectType


def test_create_with_name():
    obj = ObjectType(name="var")
    assert obj.name == "var"
    assert obj.cia_id == 0x7


def test_create_with_cia_id():
    obj = ObjectType(cia_id=0x9)
    assert obj.name == "record"
    assert obj.cia_id == 0x9


@pytest.mark.parametrize(
    "name, expected_code",
    [
        ("null", 0x0),
        ("domain", 0x2),
        ("deftype", 0x5),
        ("defstruct", 0x6),
        ("var", 0x7),
        ("array", 0x8),
        ("record", 0x9),
    ],
)
def test_all_valid_names(name, expected_code):
    obj = ObjectType(name=name)
    assert obj.name == name
    assert obj.cia_id == expected_code


@pytest.mark.parametrize(
    "cia_id, expected_name",
    [
        (0x0, "null"),
        (0x2, "domain"),
        (0x5, "deftype"),
        (0x6, "defstruct"),
        (0x7, "var"),
        (0x8, "array"),
        (0x9, "record"),
    ],
)
def test_all_valid_codes(cia_id, expected_name):
    obj = ObjectType(cia_id=cia_id)
    assert obj.cia_id == cia_id
    assert obj.name == expected_name


def test_invalid_name():
    with pytest.raises(ValidationError) as exc:
        ObjectType(name="invalid_name")
    assert "Invalid object type name" in str(exc.value)


def test_invalid_cia_id():
    with pytest.raises(ValidationError) as exc:
        ObjectType(cia_id=0x99)
    assert "Invalid cia_id" in str(exc.value)


def test_missing_fields():
    with pytest.raises(ValidationError) as exc:
        ObjectType()
    assert "Either 'name' or 'cia_id' must be provided." in str(exc.value)


def test_frozen_model():
    obj = ObjectType(name="var")
    with pytest.raises(ValidationError) as exc:
        obj.name = "domain"
    assert "frozen" in str(exc.value)
