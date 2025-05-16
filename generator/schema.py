"""
Validation schema for the YAML configuration file.

Getter:
  We use # to indicate the index in the array.

  - instance.method()
  - instance.method(#) # for array, where # is the index of the array
  - instance.attribute
  - instance.attribute[#] # for array, where # is the index of the array

Setter:
  We use @ to indicate the value to be set.
  We use # to indicate the index in the array.

  - instance.method(@)
  - instance.method(#, @)
  - instance.attribute[#] = @
"""

from pint import UnitRegistry, UndefinedUnitError
from voluptuous import (
    All,
    Any,
    Coerce,
    Invalid,
    Invalid,
    Length,
    Match,
    Optional,
    Range,
    Required,
    Schema,
)
from .types import datatypes

ureg = UnitRegistry()

# User Defined Speed
ureg.define("uds = [speed]")
# User Defined Position
ureg.define("udp = [position]")


def validate_object(value):
    if not isinstance(value, int):
        raise Invalid(f"Invalid object: {value}. Must be an integer.")
    if value < 0x1000 or value > 0x9FFF:
        raise Invalid(f"Invalid object: {value}. Must be between 0x1000 and 0x9FFF.")
    return value


def validate_profile(value):
    if not isinstance(value, int):
        raise Invalid(f"Invalid profile: {value}. Must be an integer.")
    if value not in [301, 402]:
        raise Invalid(f"Invalid profile: {value}. Must be between 0 and 65535.")
    return value


def validate_baudrates(value):
    print("PROUT")
    valid_baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}
    if value not in valid_baudrates:
        raise Invalid(
            f"Invalid baudrate: {value}. valid baudrates: {', '.join(map(str, valid_baudrates))}"
        )
    return value


def validate_type(value):
    if value not in datatypes:
        raise Invalid(
            f"Invalid type: {value}. valid types: {', '.join(datatypes.keys())}"
        )
    return datatypes[value]


def validate_unit_string(value):
    try:
        ureg.parse_units(value)  # Tente de parser la chaîne comme unité
    except UndefinedUnitError as e:
        raise Invalid(f"Invalid units: {value}: {str(e)}")
    return value


def validate_enum_name(value):
    if not value.isidentifier():
        raise Invalid(
            f"Invalid enum name: {value}. Enum names must be valid C/C++ identifiers."
        )
    return value


def validate_subindices_length(value):
    if not isinstance(value, dict):
        raise Invalid(f"Invalid subindex: {value}. Subindex must be a dictionary.")
    if len(value) > 255:
        raise Invalid(
            f"Invalid subindex: {value}. Subindex length must be less than 256."
        )
    return value


def validate_enum_data(value):
    if not isinstance(value, dict):
        raise Invalid(f"Invalid enum data: {value}. Enum data must be a dictionary.")
    if len(set(value.values())) != len(value):
        raise Invalid(f"Invalid enum data: {value}. Enum values must be unique.")
    return value


def update_access(value):
    if "get" in value and value["get"] is not None:
        value["access"] = "".join(set(value["access"]) | {"r"})
    if "set" in value and value["set"] is not None:
        value["access"] = "".join(set(value["access"]) | {"w"})
    return value


def post_validate_od(value):
    return value


header_common = {
    "name": str,
    Optional("remote", default="local"): All(str, Length(min=1)),
    Optional("path", default=""): Any(
        "", Match(r"^([a-zA-Z][a-zA-Z0-9]*)(?:\.[a-zA-Z][a-zA-Z0-9]*)*$")
    ),
    Optional("description", default=""): str,
}

var_common = {
    **header_common,
    Required("datatype"): All(str, validate_type),
    Optional("limits", default={"min": None, "max": None}): {
        Optional("min", None): Any(None, int, float),
        Optional("max", None): Any(None, int, float),
    },
    Optional("pdo", default=False): bool,
    Optional("unit", default=""): All(str, validate_unit_string),
    "enum": {
        "class": str,
        "data": All({All(str, validate_enum_name): int}, validate_enum_data),
    },
    # Default value; type must match the type of the object
    Optional("default", default=0): Any(int, float),
    Optional("access", default=""): All(str, Match(r"^r?w?$")),
    Optional("get", default=None): Any(None, str),
    Optional("set", default=None): Any(None, str),
}

def bitfield_range(value):
    if not isinstance(value, str):
        raise Invalid(f"Invalid bitfield range: {value}. Must be a string.")
    if not Match(r"^\d+\.\.\d+$")(value):
        raise Invalid(f"Invalid bitfield range: {value}. Must be in the form 'x..y'.")
    start, end = map(int, value.split(".."))
    if start < 0 or end < 0 or start > end:
        raise Invalid(f"Invalid bitfield range: {value}. Must be in the form 'x..y'.")
    return (start, end)

bitfield = {
    Any(int, All(str, bitfield_range)): {
        Any(All(str, Coerce(lambda x: { 'name': x })), { 'name': str, 'description': str, 'values': { int: str } }),
    },
}

var = All(
    {
        **header_common,
        "profile": int,
        Optional("type", default="var"): "var",
        **var_common,
    },
    update_access,
)

array = All(
    {
        **header_common,
        "profile": int,
        Optional("type", default="array"): "array",
        Required("length"): All(int, Range(min=1, max=255)),
        **var_common,
    },
    update_access,
)

record = All(
    {
        **header_common,
        "profile": int,
        Optional("type", default="record"): "record",
        Required("subindex"): [
            All(
                {
                    **var_common,
                },
                validate_subindices_length,
            )
        ],
    },
    update_access,
)

domain = {
    **header_common,
    Optional("type", default="domain"): "domain",
    "domain": {
        "max_length": All(int, Range(min=1)),
    },
}

od = All(
    {
        int: Any(array, record, var),
    },
    post_validate_od,
)


SchemaConfig = Schema(
    {
        "device": {
            Optional("vendor", default={"name": "Unknown", "number": 0x12345678}): {
                "name": str,
                "number": int,
            },
            Optional("product", default={"name": "Unknown", "number": 0x12345678}): {
                "name": str,
                "number": int,
            },
            Optional("revision", default=1): int,
            "baudrate": Coerce(set),
            Optional("node_id", default=1): All(int, Range(min=1, max=127)),
        },
        "objects": od,
    }
)
