import pytest

from generator.validation.models.object_id import ObjectId


def test_object_id_creation():
    """Test creation of ObjectId and its attributes."""
    obj_id = ObjectId(0x1234, 0x56)

    # Check that the integer value is correct
    expected_value = (0x1234 << 8) | 0x56
    assert int(obj_id) == expected_value

    # Check attributes
    assert obj_id.index == 0x1234
    assert obj_id.subindex == 0x56
    assert obj_id.eds_name == "1234sub56"


def test_object_id_str():
    """Test __str__ representation."""
    obj_id = ObjectId(0x1AB, 0x2)
    assert str(obj_id) == "0x01ab_02"


def test_object_id_repr():
    """Test __repr__ representation."""
    obj_id = ObjectId(0x1234, 0x56)
    assert repr(obj_id) == "ObjectCode(index=1234, subindex=86)"


def test_object_id_getitem():
    """Test __getitem__ for index and subindex."""
    obj_id = ObjectId(0x55AA, 0x99)

    assert obj_id["index"] == 0x55AA
    assert obj_id["subindex"] == 0x99

    with pytest.raises(KeyError, match="Invalid key: foo"):
        _ = obj_id["foo"]
