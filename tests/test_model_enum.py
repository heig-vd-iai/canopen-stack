"""Unit tests for Enum model validation."""

# pylint: disable=missing-function-docstring
import pytest
from pydantic import ValidationError

from generator.validation.models.enum import (
    Enum,
    EnumEntry,
    EnumEntryProfile,
    EnumProfile,
)


def test_enum_entry_creation():
    entry = EnumEntry(name="Test", value=42)
    assert entry.name == "Test"
    assert entry.value == 42
    assert entry.description is None


def test_enum_entry_profile_inherits_and_extends():
    entry = EnumEntryProfile(name="Test", value=1, mandatory_conditions=["A", "B"])
    assert entry.mandatory_conditions == ["A", "B"]
    assert entry.name == "Test"
    assert entry.value == 1


def test_enum_valid_data():
    model = Enum(
        typedef="TestClass",
        values={
            "FOO": EnumEntry(name="FOO", value=1),
            "BAR": EnumEntry(name="BAR", value=2),
        },
    )  # type: ignore[call-arg]
    assert model.typedef == "TestClass"
    assert len(model.values) == 2
    assert model.values["FOO"].value == 1


def test_enum_duplicate_values_should_fail():
    with pytest.raises(ValidationError) as exc_info:
        Enum(
            typedef="DuplicateTest",
            values={
                "ONE": EnumEntry(name="ONE", value=1),
                "ALSO_ONE": EnumEntry(name="ALSO_ONE", value=1),
            },
        )  # type: ignore[call-arg]
    assert "Duplicate enum value" in str(exc_info.value)


def test_enum_invalid_key_should_fail():
    with pytest.raises(ValidationError) as exc_info:
        Enum(
            typedef="InvalidKey",
            values={"123-INVALID": EnumEntry(name="BAD", value=10)},
        )

    error_message = str(exc_info.value)
    assert "Invalid enum name: 123-INVALID" in error_message
    assert "Must be a valid identifier" in error_message


def test_enum_profile_valid_override():
    model = EnumProfile(
        typedef="WithOverride",
        values={
            "A": EnumEntry(name="A", value=1),
            "B": EnumEntry(name="B", value=2),
        },
        allow_override=[(0, 5), (10, 20)],
    )
    assert model.allow_override == [(0, 5), (10, 20)]


def test_enum_profile_invalid_override_format():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            typedef="BadOverride",
            values={"A": EnumEntry(name="A", value=1)},
            allow_override=[[1]],  # type: ignore[arg-type]
        )
    assert "must be a tuple of two integers" in str(exc_info.value)


def test_enum_profile_override_start_greater_than_end():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            typedef="ReversedRange",
            values={"A": EnumEntry(name="A", value=1)},
            allow_override=[(10, 5)],  # Invalid: 10 > 5
        )
    assert "first integer must be <= the second" in str(exc_info.value)


def test_enum_profile_non_integer_override():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            typedef="NonIntRange",
            values={"A": EnumEntry(name="A", value=1)},
            allow_override=[("a", "b")],  # type: ignore[arg-type]
        )  # type: ignore[call-arg]
    assert "must be integers" in str(exc_info.value)


def test_enum_from_dict():
    e = Enum(
        **{
            "typedef": "TestEnum",
            "values": {
                "FOO": {"name": "A", "value": 1},
                "BAR": {"name": "B", "value": 2, "description": "Test description"},
            },
        }
    )
    assert e.typedef == "TestEnum"
    assert e.values["FOO"].name == "A"
    assert e.values["BAR"].name == "B"
    assert e.values["BAR"].description == "Test description"


def test_enum_from_dict_with_string():
    e = Enum(**{"typedef": "TestEnum", "values": {"FOO": 1, "BAR": 2}})
    assert e.typedef == "TestEnum"
    assert e.values["FOO"].name == "FOO"
    assert e.values["FOO"].value == 1
    assert e.values["BAR"].value == 2
    assert e.values["BAR"].description is None
