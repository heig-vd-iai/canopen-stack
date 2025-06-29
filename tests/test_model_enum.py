"""Unit tests for Enum model validation."""

# pylint: disable=missing-function-docstring
import pytest
from pydantic import ValidationError

from generator.validation.enum import Enum, EnumEntry, EnumEntryProfile, EnumProfile


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
        class_="TestClass",
        data={
            "FOO": EnumEntry(name="FOO", value=1),
            "BAR": EnumEntry(name="BAR", value=2),
        },
    )  # type: ignore[call-arg]
    assert model.class_ == "TestClass"
    assert len(model.data) == 2
    assert model.data["FOO"].value == 1


def test_enum_duplicate_values_should_fail():
    with pytest.raises(ValidationError) as exc_info:
        Enum(
            class_="DuplicateTest",
            data={
                "ONE": EnumEntry(name="ONE", value=1),
                "ALSO_ONE": EnumEntry(name="ALSO_ONE", value=1),
            },
        )  # type: ignore[call-arg]
    assert "Enum values must be unique" in str(exc_info.value)


def test_enum_invalid_key_should_fail():
    with pytest.raises(ValidationError) as exc_info:
        Enum(class_="InvalidKey", data={"123-INVALID": EnumEntry(name="BAD", value=10)})

    error_message = str(exc_info.value)
    assert "Invalid enum name: 123-INVALID" in error_message
    assert "Must be a valid identifier" in error_message


def test_enum_profile_valid_override():
    model = EnumProfile(
        class_="WithOverride",
        data={
            "A": EnumEntry(name="A", value=1),
            "B": EnumEntry(name="B", value=2),
        },
        allow_override=[(0, 5), (10, 20)],
    )  # type: ignore[call-arg]
    assert model.allow_override == [(0, 5), (10, 20)]


def test_enum_profile_invalid_override_format():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            class_="BadOverride",
            data={"A": EnumEntry(name="A", value=1)},
            allow_override=[[1]],  # Invalid: only one value
        )  # type: ignore[call-arg]
    assert "must be a tuple of two integers" in str(exc_info.value)


def test_enum_profile_override_start_greater_than_end():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            class_="ReversedRange",
            data={"A": EnumEntry(name="A", value=1)},
            allow_override=[(10, 5)],  # Invalid: 10 > 5
        )  # type: ignore[call-arg]
    assert "first integer must be <= the second" in str(exc_info.value)


def test_enum_profile_non_integer_override():
    with pytest.raises(ValidationError) as exc_info:
        EnumProfile(
            class_="NonIntRange",
            data={"A": EnumEntry(name="A", value=1)},
            allow_override=[("a", "b")],
        )  # type: ignore[call-arg]
    assert "must be integers" in str(exc_info.value)
