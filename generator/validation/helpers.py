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


def resolve_inheritance(objects: dict) -> dict:
    """Resolve inheritance for objects.
    This function merges objects that inherit from each other, ensuring
    that the final object contains all properties from its base objects.
    """
    def resolve(idx: int, visited: set[int]) -> dict:
        obj = objects[idx]
        inherit_id = obj.get("inherit")
        if not inherit_id:
            return obj
        if inherit_id not in objects:
            raise ValueError(
                f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h"
            )
        if inherit_id in visited:
            raise ValueError(f"Circular inheritance involving {idx:04X}h")

        base = resolve(inherit_id, visited | {idx})
        merged = {**base, **obj}
        return merged

    return {idx: resolve(idx, set()) for idx in objects}
