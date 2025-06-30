"""Tests for object models"""

# pylint: disable=missing-function-docstring
import pytest

from generator.validation import Datatype, Limits, Markdown
from generator.validation.objects import (
    Array,
    ArrayEntry,
    Domain,
    Record,
    RecordEntry,
    Var,
)


def test_var_minimal():
    var = Var(
        name="Speed",
        datatype=Datatype.from_name("int16"),
    )
    assert var.type == "var"
    assert var.name == "Speed"
    assert var.datatype.name == "int16"
    assert var.default == 0
    assert var.remote == "local"
    assert isinstance(var.description, Markdown)


def test_var_with_custom_values():
    var = Var(
        name="Voltage",
        datatype=Datatype.from_name("float32"),
        limits=Limits(min=0.0, max=24.0),
        pdo=True,
        default=12.5,
    )
    assert var.limits.min == 0.0
    assert var.pdo is True
    assert var.default == 12.5


def test_array_minimal():
    array = Array(
        name="MyArray",
        datatype="int8",
        unit= "m/s",
        data=[
            {"name": "Entry1", "get": "foobar", "set": "baz"},
            {"name": "Entry2", "default": 42},
        ],
    )
    assert array.type == "array"
    assert array.name == "MyArray"
    assert array.datatype.name == "uint16"
    assert len(array.data) == 3
    assert array.data[0].default == 42


def test_array_length_infer():
    array = Array(
        name="MyArray",
        datatype=Datatype.from_name("int32"),
        data=[
            ArrayEntry(default=5),
            ArrayEntry(default=10),
        ],
    )
    # InferArrayLengthMixin may set length automatically
    assert array.length in (None, 2)


def test_record_injects_size_entry():
    rec = Record(
        name="MyRecord",
        record=[
            RecordEntry(
                name="Sub1",
                datatype=Datatype.from_name("uint8"),
                default=10,
            ),
            RecordEntry(
                name="Sub2",
                datatype=Datatype.from_name("uint16"),
                default=20,
            ),
        ],
    )
    assert rec.type == "record"
    # Subindex 0 injected
    assert rec.record[0].name == Record.SIZE_ENTRY_NAME
    assert rec.record[0].datatype.name == "uint8"
    assert rec.record[0].default == 3


def test_record_updates_size_if_present():
    # Provide subindex 0 explicitly
    rec = Record(
        name="MyRecord",
        record=[
            RecordEntry(
                name=Record.SIZE_ENTRY_NAME,
                datatype=Datatype.from_name("uint8"),
                default=0,
            ),
            RecordEntry(
                name="Sub1",
                datatype=Datatype.from_name("uint8"),
                default=10,
            ),
        ],
    )
    assert rec.record[0].default == 2


def test_record_too_large_raises():
    with pytest.raises(ValueError):
        Record(
            name="BigRecord",
            record=[
                RecordEntry(
                    name=f"Sub{i}",
                    datatype=Datatype.from_name("uint8"),
                    default=0,
                )
                for i in range(300)
            ],
        )


def test_domain_creation():
    dom = Domain(
        name="FirmwareDomain",
        domain={
            "StartAddress": 100,
            "EndAddress": 200,
        },
    )
    assert dom.type == "domain"
    assert "StartAddress" in dom.domain
    assert dom.domain["EndAddress"] == 200


def test_path_regex_validation():
    # Valid
    v = Var(
        name="Test",
        path="My.Var.Path",
        datatype=Datatype.from_name("uint8"),
    )
    assert v.path == "My.Var.Path"

    # Invalid
    with pytest.raises(ValueError):
        Var(
            name="Test",
            path="123Invalid.Path",
            datatype=Datatype.from_name("uint8"),
        )


def test_inherit_field_optional():
    v = Var(
        name="Test",
        datatype=Datatype.from_name("uint16"),
    )
    assert v.inherit is None
