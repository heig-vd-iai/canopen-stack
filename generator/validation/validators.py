"""Various helper functions for validation of objects and enums."""
from pint import UndefinedUnitError, UnitRegistry

ureg = UnitRegistry()
ureg.define("udv = [speed]")
ureg.define("udp = [position]")
ureg.define("uda = [acceleration]")
ureg.define("inc = [increment]")
ureg.define("ratedTorque = [torque]")

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
