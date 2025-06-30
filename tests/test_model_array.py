import pytest

from generator.validation import Array, ArrayEntry, BaseArray, Datatype, Enum


def test_array_entry_creation():
    ae = ArrayEntry(
        **{
            "name": "MyEntry",
            "limits": {"min": 0, "max": 10},
            "enum": {"class": "MyEnum", "data": {"A": 1}},
            "default": 5,
        }
    )
    assert ae.default == 5
    assert ae.limits.min == 0
    assert isinstance(ae.enum, Enum)
    assert ae.enum.class_ == "MyEnum"


def test_simple_array_creation():
    arr = Array(length=10, datatype="int32", name="MyArray")
    assert arr.type == "array"
    assert arr.length == 10
    assert arr.datatype == Datatype.from_name("int32")
    assert arr.name == "MyArray"


def test_base_array_length_invalid():
    with pytest.raises(ValueError):
        BaseArray(length=-1, datatype="int32", name="InvalidArray")
    with pytest.raises(ValueError):
        BaseArray(length=256, datatype="int32", name="InvalidArray")


def test_array_creation():
    arr = Array(
        **{
            "name": "TestArray",
            "datatype": "int32",
            "length": 5,
            "data": [{"limits": {"min": 0, "max": 100}, "default": 42}],
        }
    )
    assert arr.type == "array"
    assert arr.length == 5
    assert isinstance(arr.data[0], ArrayEntry)
    assert arr.data[0].default == 5
    assert arr.data[1].default == 42


def test_array_defaults():
    arr = Array(name="EmptyArray", datatype="float32")
    assert arr.type == "array"
    assert arr.length == 0
    assert len(arr.data) == 1
    assert isinstance(arr.data[0], ArrayEntry)
    assert arr.data[0].name == "Number of array entries"


def test_array_invalid_data():
    with pytest.raises(ValueError):
        Array(data=["not an ArrayEntry"])  # type: ignore


def test_array_length_boundaries():
    arr = Array(name="Foo", datatype="int32", length=0)
    assert arr.length == 0
    arr = Array(name="Foo", datatype="int32", length=255)
    assert arr.length == 255
    with pytest.raises(ValueError):
        Array(name="Foo", datatype="int32", length=256)
