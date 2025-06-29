"""Tests for the units validation module."""
# pylint: disable=missing-function-docstring
import pytest
from pint import Quantity

from generator.validation.units import (
    resolve_quantity,
    unit_from_cia,
    ureg,
    validate_identifier,
    validate_unit_string,
)


def test_validate_unit_string_valid():
    assert validate_unit_string("m") == "m"
    assert validate_unit_string("Nm") == "Nm"
    assert validate_unit_string("newton meter") == "newton meter"


def test_validate_unit_string_invalid():
    with pytest.raises(ValueError) as e:
        validate_unit_string("unknownunit")
    assert "Invalid units" in str(e.value)


def test_validate_identifier_valid():
    assert validate_identifier("valid_name") == "valid_name"
    assert validate_identifier("_valid") == "_valid"


@pytest.mark.parametrize(
    "value",
    [
        "123abc",
        "invalid-name",
        "has space",
        "",
        "!",
    ],
)
def test_validate_identifier_invalid(value):
    with pytest.raises(ValueError) as e:
        validate_identifier(value)
    assert "Invalid enum name" in str(e.value)


def test_resolve_quantity_simple_substitution():
    q = 5 * ureg("udv")  # symbolic unit
    resolved = resolve_quantity(q, udv="m/s")
    assert isinstance(resolved, Quantity)
    assert resolved.units == ureg("m/s")
    assert resolved.magnitude == 5


def test_resolve_quantity_multiple_substitutions():
    q = 10 * ureg("udv * udp")
    resolved = resolve_quantity(q, udv="m/s", udp="s")
    # Should resolve to m
    assert resolved.units == ureg("meter")
    assert resolved.magnitude == 10


def test_resolve_quantity_no_substitution():
    q = 3 * ureg("m")
    resolved = resolve_quantity(q)
    assert resolved == q


def test_unit_from_cia_without_denominator():
    # Construct value with:
    # - prefix = 0x03 → kilo → 1e3
    # - numerator = 0x01 → meter
    # - denominator = 0x01 → no denominator
    value = (0x03 << 24) | (0x01 << 16) | (0x01 << 8)
    qty = unit_from_cia(value)
    assert isinstance(qty, Quantity)
    assert qty.units == ureg("meter")
    assert qty.magnitude == pytest.approx(1e3)


def test_unit_from_cia_with_denominator():
    # kilo newton / meter
    value = (0x03 << 24) | (0x21 << 16) | (0x01 << 8)  # numerator=0x21 (N), denominator=0x01
    qty = unit_from_cia(value)
    assert isinstance(qty, Quantity)
    # Check that it resolves to kilonewton
    assert qty.units == ureg("newton")
    assert qty.magnitude == pytest.approx(1e3)


def test_unit_from_cia_with_denominator_real():
    # kilo Nm / s
    numerator_idx = 0x56  # Nm
    denominator_idx = 0x03  # second
    prefix = 0x03  # kilo

    value = (prefix << 24) | (numerator_idx << 16) | (denominator_idx << 8)
    qty = unit_from_cia(value)
    assert isinstance(qty, Quantity)
    # Check magnitude
    assert qty.magnitude == pytest.approx(1e3)
    # Check units
    expected_units = ureg("newton meter / second")
    assert qty.units == expected_units.units


def test_unit_from_cia_invalid_numerator():
    value = (0x00 << 24) | (0xFE << 16) | (0x01 << 8)  # invalid numerator index
    with pytest.raises(ValueError) as e:
        unit_from_cia(value)
    assert "Unknown numerator index" in str(e.value)


def test_unit_from_cia_invalid_denominator():
    value = (0x00 << 24) | (0x21 << 16) | (0xFE << 8)  # invalid denominator index
    with pytest.raises(ValueError) as e:
        unit_from_cia(value)
    assert "Unknown denominator index" in str(e.value)
