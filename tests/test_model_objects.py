"""Tests for object models"""

# pylint: disable=missing-function-docstring
import pytest

from generator.validation import Datatype, Var


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
