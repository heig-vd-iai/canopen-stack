"""Unit tests for Bitfield model validation."""

# pylint: disable=missing-function-docstring
import pytest
from pydantic import ValidationError

from generator.validation.models.bitfields import Bitfield, BitRange


def test_bitfield_single_bit():
    bf = Bitfield(entries={"7": "FLAG_READY"})
    assert (7, 7) in bf
    assert bf[(7, 7)].name == "FLAG_READY"
    assert bf[(7, 7)].values == {}


def test_bitfield_range_bits_with_valid_values():
    bf = Bitfield(
        entries={
            "15..12": {"name": "MODE", "values": {0: "IDLE", 1: "RUN", 2: "SLEEP"}}  # type: ignore[arg-type]
        }
    )
    entry = bf[(15, 12)]
    assert entry.name == "MODE"
    assert entry.values[0] == "IDLE"
    assert entry.width == 4


def test_bitfield_invalid_range_string():
    with pytest.raises(ValueError, match="start must be >= end"):
        Bitfield.model_validate({"5..7": "BAD"})


def test_bitfield_invalid_key_type():
    with pytest.raises(ValidationError, match="Input should be a valid dictionary"):
        Bitfield(entries={(1, 2): "InvalidKey"})  # type: ignore[arg-type]


def test_bitfield_value_exceeds_width():
    with pytest.raises(ValueError, match="exceeds width limit"):
        Bitfield(
            entries={"3..0": {"name": "TOO_BIG", "values": {16: "X"}}}  # type: ignore[arg-type]
        )  # 4-bit max = 15


def test_bitfield_entry_from_int_key():
    bf = Bitfield(entries={2: "READY"})  # type: ignore[arg-type]
    assert (2, 2) in bf
    assert bf[(2, 2)].name == "READY"


def test_bitfield_entry_serialization():
    bf = Bitfield(
        entries={
            "7": "FLAG_A",
            "6..4": {"name": "MODE", "values": {1: "INIT", 2: "START", 3: "RUN"}},
        }
    )
    serialized = bf.__pydantic_serializer__.to_python(bf)
    assert serialized == {
        "entries": {
            (7, 7): {"name": "FLAG_A", "values": {}},
            (6, 4): {"name": "MODE", "values": {1: "INIT", 2: "START", 3: "RUN"}},
        }
    }


def test_bitfield_range_without_values():
    bf = Bitfield(entries={"10..8": {"name": "SPEED"}})
    entry = bf[(10, 8)]
    assert entry.name == "SPEED"
    assert entry.values == {}
    assert entry.width == 3


def test_bitfield_values_with_non_integer_keys():
    with pytest.raises(ValueError):
        Bitfield(entries={"5..4": {"name": "TEST", "values": {"high": "HIGH"}}})


def test_bitfield_with_invalid_value_type():
    with pytest.raises(ValueError):
        Bitfield(entries={"7": 123})  # type: ignore[arg-type]


def test_bitfield_overlapping_ranges():
    with pytest.raises(ValueError, match="overlapping"):
        Bitfield(entries={"7..5": "FLAG1", "6..4": "FLAG2"})  # type: ignore[arg-type]


def test_bitfield_duplicate_names_allowed():
    bf = Bitfield(entries={"7": "FLAG_X", "6": "FLAG_X"})  # type: ignore[arg-type]
    assert bf[(7, 7)].name == "FLAG_X"
    assert bf[(6, 6)].name == "FLAG_X"


def test_bitfield_single_bit_with_values():
    bf = Bitfield(
        entries={"2": {"name": "ENABLE", "values": {0: "DISABLED", 1: "ENABLED"}}}
    )
    entry = bf[(2, 2)]
    assert entry.name == "ENABLE"
    assert entry.values[0] == "DISABLED"
    assert entry.width == 1


def test_bitfield_invalid_string_key():
    with pytest.raises(ValueError):
        Bitfield(entries={"foo": "INVALID"})


def test_bitrange_invalid_format():
    with pytest.raises(ValueError, match="Invalid bit range format"):
        BitRange("12..foo")


def test_bitrange_invalid_type():
    with pytest.raises(ValueError, match="Invalid bitfield key"):
        BitRange(3.14)


def test_bitrange_invalid_str_key():
    with pytest.raises(ValueError, match="Invalid bitfield key"):
        BitRange("hello")


def test_bitfield_entries_not_dict():
    with pytest.raises(TypeError, match="entries must be a dict"):
        Bitfield.model_validate({"entries": ["not", "a", "dict"]})


def test_bitfield_input_not_dict():
    with pytest.raises(TypeError, match="Bitfield input must be a dict"):
        Bitfield.model_validate("hello world")


def test_bitfield_invalid_value_type_in_parse():
    from generator.validation.models.bitfields import Bitfield

    with pytest.raises(ValueError, match="Invalid bitfield value for key"):
        Bitfield.model_validate({"7": 3.14})


def test_bitfield_getitem_and_contains():
    bf = Bitfield(entries={"3": "READY"})
    assert (3, 3) in bf
    entry = bf[(3, 3)]
    assert isinstance(entry.name, str)


def test_bitfield_iter_methods():
    bf = Bitfield(entries={"7": "FLAG"})
    items = list(bf.items())
    keys = list(bf.keys())
    values = list(bf.values())

    assert len(items) == 1
    assert keys == [(7, 7)]
    assert values[0].name == "FLAG"


def test_bitfield_to_serializable_range_key():
    bf = Bitfield(entries={"5..3": "FLAG_RANGE"})
    serial = bf.to_serializable()
    assert "5..3" in serial
    assert serial["5..3"]["name"] == "FLAG_RANGE"
