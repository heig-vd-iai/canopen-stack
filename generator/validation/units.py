"""Various helper functions for validation of objects and enums."""

from typing import Dict, NamedTuple

from pint import Quantity, UndefinedUnitError, UnitRegistry


class SIUnitInfo(NamedTuple):
    unit: str
    symbol: str
    value: int


# CiA DR‑303‑2 / CiA‑303‑2 (used by CiA 890)
SI_UNITS: Dict[str, SIUnitInfo] = {
    "m": SIUnitInfo("meter", "m", 0x01),
    "kg": SIUnitInfo("kilogram", "kg", 0x02),
    "s": SIUnitInfo("second", "s", 0x03),
    "A": SIUnitInfo("ampere", "A", 0x04),
    "K": SIUnitInfo("kelvin", "K", 0x05),
    "mol": SIUnitInfo("mole", "mol", 0x06),
    "cd": SIUnitInfo("candela", "cd", 0x07),
    "rad": SIUnitInfo("radian", "rad", 0x10),
    "Hz": SIUnitInfo("hertz", "Hz", 0x20),
    "N": SIUnitInfo("newton", "N", 0x21),
    "Pa": SIUnitInfo("pascal", "Pa", 0x22),
    "J": SIUnitInfo("joule", "J", 0x23),
    "W": SIUnitInfo("watt", "W", 0x24),
    "C": SIUnitInfo("coulomb", "C", 0x25),
    "V": SIUnitInfo("volt", "V", 0x26),
    "F": SIUnitInfo("farad", "F", 0x27),
    "Ohm": SIUnitInfo("ohm", "Ohm", 0x28),
    "S": SIUnitInfo("siemens", "S", 0x29),
    "Wb": SIUnitInfo("weber", "Wb", 0x2A),
    "T": SIUnitInfo("tesla", "T", 0x2B),
    "H": SIUnitInfo("henry", "H", 0x2C),
    "°C": SIUnitInfo("degree Celsius", "°C", 0x2D),
    "lm": SIUnitInfo("lumen", "lm", 0x2E),
    "lx": SIUnitInfo("lux", "lx", 0x2F),
    "Bq": SIUnitInfo("becquerel", "Bq", 0x30),
    "Gy": SIUnitInfo("gray", "Gy", 0x31),
    "Sv": SIUnitInfo("sievert", "Sv", 0x32),
}

# Table des indices des unités CiA 303-2 (extrait pour démo)
CIA_UNITS: Dict[int, str] = {
    0x01: "meter",
    0x02: "kilogram",
    0x03: "second",
    0x04: "ampere",
    0x10: "radian",
    0x20: "hertz",
    0x21: "newton",
    0x22: "pascal",
    0x23: "joule",
    0x24: "watt",
    0x25: "coulomb",
    0x26: "volt",
    0x27: "farad",
    0x28: "ohm",
    0x29: "siemens",
    0x2A: "weber",
    0x2B: "tesla",
    0x2C: "henry",
    0x2D: "degC",
    0x2E: "lumen",
    0x2F: "lux",
    0x30: "becquerel",
    0x31: "gray",
    0x32: "sievert",
    0xB5: "inc",  # Unité interne spécifique CANopen
}

# Table des préfixes SI selon CiA 303-2
CIA_PREFIXES: Dict[int, float] = {
    0x00: 1.0,  # pas de préfixe
    0xFD: 1e-3,  # milli
    0xFC: 1e-6,  # micro
    0xFB: 1e-9,  # nano
    0x03: 1e3,  # kilo
    0x06: 1e6,  # mega
    0x09: 1e9,  # giga
}

ureg = UnitRegistry()

# Define custom units (CANopen specific)
ureg.define("udv = [speed]")  # Defined in 60A9h
ureg.define("udp = [position]")
ureg.define("uda = [acceleration]")
ureg.define("inc = [increment]")
ureg.define("ratedTorque = [torque]")

# Resolve ambiguous units
ureg.define("Nm = newton * meter")


def validate_unit_string(value: str) -> str:
    """Validate that the value is a valid unit string.
    Raises ValueError if the value is not a valid unit string.
    """
    try:
        ureg.parse_units(value)
    except UndefinedUnitError as e:
        raise ValueError(f"Invalid units: {value}: {str(e)}") from e
    return value


def validate_identifier(value: str) -> str:
    """Validate that the value is a valid Python identifier.
    Raises ValueError if the value is not a valid identifier.
    """
    if not value.isidentifier():
        raise ValueError(f"Invalid enum name: {value}. Must be a valid identifier.")
    return value


def resolve_quantity(qty: Quantity, **substitutions) -> Quantity:
    """Resolve a Quantity by substituting symbolic units with real units."""

    units = qty.units
    for symbolic_unit, real_unit in substitutions.items():
        if symbolic_unit in str(units):
            real_u = ureg.parse_units(real_unit)
            symb_u = ureg.parse_units(symbolic_unit)
            # Replace symbolic unit with real unit by multiplying/dividing
            units = units * (real_u / symb_u)
    return qty.magnitude * units


def unit_from_cia(value: int) -> Quantity:
    """Convert a CiA unit such as 60AAh or 60A9h to a Pint Quantity."""
    prefix_idx = (value >> 24) & 0xFF
    numerator_idx = (value >> 16) & 0xFF
    denominator_idx = (value >> 8) & 0xFF

    factor = CIA_PREFIXES.get(prefix_idx, 1.0)

    numerator = CIA_UNITS.get(numerator_idx)
    if not numerator:
        raise ValueError(f"Unknown numerator index: 0x{numerator_idx:02X}")

    if denominator_idx == 0x01:
        unit_expr = numerator
    else:
        denominator = CIA_UNITS.get(denominator_idx)
        if not denominator:
            raise ValueError(f"Unknown denominator index: 0x{denominator_idx:02X}")
        unit_expr = f"{numerator} / {denominator}"

    return factor * ureg(unit_expr)
