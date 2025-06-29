# pylint: disable=missing-function-docstring
import pytest

from generator.validation.access import Access


def test_access_default_values():
    """Default Access should be neither read nor write."""
    a = Access()
    assert a.read is False
    assert a.write is False
    assert str(a) == ""
    assert a.to_eds() == ""
    assert repr(a) == "Access(read=False, write=False)"


@pytest.mark.parametrize(
    "s, expected_read, expected_write, expected_eds",
    [
        ("r", True, False, "ro"),
        ("w", False, True, "wo"),
        ("rw", True, True, "rw"),
        ("", False, False, ""),
    ],
)
def test_access_from_string(s, expected_read, expected_write, expected_eds):
    """Test string-based parsing of Access rights."""
    a = Access.model_validate(s)
    assert a.read is expected_read
    assert a.write is expected_write
    # __str__
    assert str(a) == s
    # to_eds
    assert a.to_eds() == expected_eds


def test_access_from_dict():
    """Access can be created from a dictionary."""
    data = {"read": True, "write": False}
    a = Access.model_validate(data)
    assert a.read is True
    assert a.write is False
    assert str(a) == "r"
    assert a.to_eds() == "ro"


def test_access_from_instance():
    """Model validator should accept an Access instance."""
    original = Access(read=True, write=True)
    a = Access.model_validate(original)
    assert a == original


def test_access_repr_and_str():
    """Check string and repr output."""
    a = Access(read=True, write=False)
    assert str(a) == "r"
    assert repr(a) == "Access(read=True, write=False)"


def test_access_to_eds():
    """Check EDS string generation."""
    assert Access(read=True, write=False).to_eds() == "ro"
    assert Access(read=False, write=True).to_eds() == "wo"
    assert Access(read=True, write=True).to_eds() == "rw"
    assert Access(read=False, write=False).to_eds() == ""


@pytest.mark.parametrize("bad_input", [42, None, 3.14, ["r"], ("rw",)])
def test_access_invalid_input_raises(bad_input):
    """Invalid input types should raise TypeError."""
    with pytest.raises(TypeError):
        Access.model_validate(bad_input)
