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

from voluptuous import (
    All,
    Any,
    Match,
    Invalid,
    Invalid,
    Range,
    Length,
    Optional,
    Required,
    Schema,
    Coerce,
)
from pint import UnitRegistry, UndefinedUnitError
from .types import datatypes

ureg = UnitRegistry()


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
        raise Invalid(f"Invalid type: {value}. valid types: {', '.join(datatypes.keys())}")
    return value


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


def validate_enum_data(value):
    if not isinstance(value, dict):
        raise Invalid(f"Invalid enum data: {value}. Enum data must be a dictionary.")
    if len(set(value.values())) != len(value):
        raise Invalid(f"Invalid enum data: {value}. Enum values must be unique.")
    return value


def post_validate_od(value):
    return value


header_common = {
    "name": str,
    Optional("remote", default="local"): All(str, Length(min=1)),
    Optional("path", default=""): Any('', Match(
        r"^([a-zA-Z][a-zA-Z0-9]*)(?:\.[a-zA-Z][a-zA-Z0-9]*)*$"
    )),
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
    Optional("unit", default=''): All(str, validate_unit_string),
    "enum": {
        "class": str,
        "data": All({All(str, validate_enum_name): int}, validate_enum_data),
    },
    # Default value; type must match the type of the object
    Optional("default", 0): Any(int, float),
    "access": All(str, Match(r"^r?w?$")),
    Optional("get", default=None): Any(None, str),
    Optional("set", default=None): Any(None, str),
}

var = {
    **header_common,
    "profile": int,
    Optional("type", default="var"): "var",
    **var_common,
}

array = {
    **header_common,
    "profile": int,
    Optional("type", default="array"): "array",
    Required("length"): All(int, Range(min=1, max=255)),
    **var_common,
}

record = {
    **header_common,
    "profile": int,
    Optional("type", default="record"): "record",
    Required("subindex"): [
        All(
            {
                **var_common,
            },
            lambda x: len(x) < 255,
        )
    ],
}

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
            "vendor": {"name": str, "number": int},
            "product": {"name": str, "number": int},
            "revision": int,
            "baudrate": Coerce(set),
            "node_id": All(int, Range(min=1, max=127)),
        },
        "objects": od,
    }
)
