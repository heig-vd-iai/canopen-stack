import pytest

from generator.validation.bitfields import Bitfield


def test_bitfield_single_bit():
    bf = Bitfield({
        "7": "FLAG_READY"
    })
    assert (7, 7) in bf
    assert bf[(7, 7)].name == "FLAG_READY"
    assert bf[(7, 7)].values == {}


def test_bitfield_range_bits_with_valid_values():
    bf = Bitfield({
        "15..12": {
            "name": "MODE",
            "values": {
                0: "IDLE",
                1: "RUN",
                2: "SLEEP"
            }
        }
    })
    entry = bf[(15, 12)]
    assert entry.name == "MODE"
    assert entry.values[0] == "IDLE"
    assert entry._width == 4


def test_bitfield_invalid_range_string():
    with pytest.raises(ValueError, match="start must be >= end"):
        Bitfield({
            "5..7": "BAD"
        })


def test_bitfield_invalid_key_type():
    with pytest.raises(ValueError, match="Invalid bitfield key"):
        Bitfield({
            (1, 2): "InvalidKey"
        })


def test_bitfield_value_exceeds_width():
    with pytest.raises(ValueError, match="exceeds width limit"):
        Bitfield({
            "3..0": {
                "name": "TOO_BIG",
                "values": {
                    16: "X"  # 4-bit max = 15
                }
            }
        })


def test_bitfield_entry_from_int_key():
    bf = Bitfield({
        2: "READY"
    })
    assert (2, 2) in bf
    assert bf[(2, 2)].name == "READY"


def test_bitfield_entry_serialization():
    bf = Bitfield({
        "7": "FLAG_A",
        "6..4": {
            "name": "MODE",
            "values": {
                1: "INIT",
                2: "START",
                3: "RUN"
            }
        }
    })
    serialized = bf.__pydantic_serializer__.to_python(bf)
    assert serialized == {
        "7": {"name": "FLAG_A", "values": {}},
        "6..4": {"name": "MODE", "values": {1: "INIT", 2: "START", 3: "RUN"}}
    }
