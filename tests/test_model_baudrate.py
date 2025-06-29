"""Unit tests for the Baudrate model."""

import pytest
from generator.validation.baudrate import Baudrate, CIA_BAUDRATES

def test_baudrate_creation_with_valid_values():
    """Test creating a Baudrate with valid baudrates."""
    br = Baudrate([125, 250, 1000])
    assert 125 in br
    assert 250 in br
    assert 1000 in br
    assert 20 not in br
    assert isinstance(br, set)
    assert repr(br) == "Baudrate(125, 250, 1000)"


def test_baudrate_creation_with_duplicates():
    """Test that duplicates are removed like in a set."""
    br = Baudrate([125, 250, 125, 250])
    assert sorted(br) == [125, 250]


def test_baudrate_creation_with_invalid_values():
    """Test creating a Baudrate with invalid baudrates raises ValueError."""
    with pytest.raises(ValueError, match="Invalid Baudrate"):
        Baudrate([123, 250])


def test_baudrate_creation_with_mixed_values():
    """Test that creating Baudrate with invalid entries raises an error."""
    with pytest.raises(ValueError, match="Invalid Baudrate"):
        Baudrate([125, 123, 500])


def test_baudrate_to_dict():
    """Test conversion to dictionary."""
    br = Baudrate([125, 500])
    d = br.to_dict()
    assert d[125] is True
    assert d[500] is True
    assert d[10] is False
    assert set(d.keys()) == CIA_BAUDRATES


def test_baudrate_repr_empty():
    """Test __repr__ of an empty Baudrate."""
    br = Baudrate()
    assert repr(br) == "Baudrate()"


def test_baudrate_validate_with_list():
    """Test _validate classmethod with list input."""
    result = Baudrate._validate([125, 500])
    assert isinstance(result, Baudrate)
    assert sorted(result) == [125, 500]


def test_baudrate_validate_with_dict():
    """Test _validate classmethod with dict input."""
    d = {125: True, 250: None, 300: False}
    result = Baudrate._validate(d)
    assert isinstance(result, Baudrate)
    assert sorted(result) == [125, 250]


def test_baudrate_validate_with_invalid_type():
    """Test _validate raises TypeError for invalid types."""
    with pytest.raises(TypeError):
        Baudrate._validate("invalid_string")


def test_baudrate_with_empty_input():
    """Test creating an empty Baudrate."""
    br = Baudrate([])
    assert len(br) == 0
    d = br.to_dict()
    assert all(val is False for val in d.values())
