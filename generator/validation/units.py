"""Various helper functions for validation of objects and enums."""

from typing import Dict, NamedTuple

from pint import Quantity, UndefinedUnitError, UnitRegistry


class SIUnitInfo(NamedTuple):
    unit: str
    symbol: str
    value: int


# CiA DR‑303‑2 / CiA‑303‑2 (used by CiA 890)
SI_UNITS: Dict[str, SIUnitInfo] = {
    # Base SI units (CiA DR-303-2 Table 4.1)
    "m": SIUnitInfo("meter", "m", 0x01),
    "kg": SIUnitInfo("kilogram", "kg", 0x02),
    "s": SIUnitInfo("second", "s", 0x03),
    "A": SIUnitInfo("ampere", "A", 0x04),
    "K": SIUnitInfo("kelvin", "K", 0x05),
    "mol": SIUnitInfo("mole", "mol", 0x06),
    "cd": SIUnitInfo("candela", "cd", 0x07),

    # Supplementary SI units (CiA DR-303-2 Table 4.2)
    "rad": SIUnitInfo("radian", "rad", 0x10),
    "sr": SIUnitInfo("steradian", "sr", 0x11),

    # Derived SI units (CiA DR-303-2 Table 4.3)
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

    # Other units from ISO 1000 (CiA DR-303-2 Table 4.4)
    "grade": SIUnitInfo("grade", "grad", 0x40),
    "deg": SIUnitInfo("degree", "°", 0x41),
    "arcmin": SIUnitInfo("arcminute", "'", 0x42),
    "arcsec": SIUnitInfo("arcsecond", '"', 0x43),
    "l": SIUnitInfo("liter", "l", 0x44),
    "a": SIUnitInfo("are", "a", 0x45),
    "ha": SIUnitInfo("hectare", "ha", 0x46),
    "min": SIUnitInfo("minute", "min", 0x47),
    "h": SIUnitInfo("hour", "h", 0x48),
    "d": SIUnitInfo("day", "d", 0x49),
    "y": SIUnitInfo("year", "y", 0x4A),
    "g": SIUnitInfo("gram", "g", 0x4B),
    "t": SIUnitInfo("tonne", "t", 0x4C),
    "bar": SIUnitInfo("bar", "bar", 0x4E),
    "P": SIUnitInfo("poise", "P", 0x4F),
    "St": SIUnitInfo("stokes", "St", 0x50),
    "eV": SIUnitInfo("electronvolt", "eV", 0x51),
    "u": SIUnitInfo("atomic mass unit", "u", 0x52),
    "AU": SIUnitInfo("astronomical unit", "AU", 0x53),
    "pc": SIUnitInfo("parsec", "pc", 0x54),
    "m/s2": SIUnitInfo("meter per second squared", "m/s²", 0x55),
    "Nm": SIUnitInfo("newton meter", "Nm", 0x56),
    "s2": SIUnitInfo("second squared", "s²", 0x57),
    "m2": SIUnitInfo("square meter", "m²", 0x58),
    "m3": SIUnitInfo("cubic meter", "m³", 0x59)
}

CIA_UNITS: Dict[int, str] = {value.value: value.unit for value in SI_UNITS.values()}

# CiA 303-2
CIA_PREFIXES: Dict[int, float] = {
    0x00: 1.0,
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
