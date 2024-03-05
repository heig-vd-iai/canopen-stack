from .type import Access, Type
from voluptuous import Schema, Required, Optional, All, Length, Any, Invalid

def validate_access(value):
    if value not in Access.__members__:
        raise Invalid(f"Invalid access type. Must be one of {list(Access.__members__.values())}")
    return value

def validate_profile_object(objects):
    for object in objects:
        if len(object["data"]) > 1 and not all("name" in data for data in object["data"]):
            raise Invalid(f"If there are more than one data fields, they must have a name [{hex(object['index'])}]")

        if object["type"] == "none" and all(data["type"] == "none" for data in object["data"]):
            raise Invalid(f"Object or data need type [[{hex(object['index'])}]]")
        if len(object["data"]) == 1:
            object["data"][0]["name"] = object["name"]
            if object["type"] != "none":
                object["data"][0]["type"] = object["type"]
        for data in object["data"]:
            if not data["default"] and data["default_value"] != "none":
                raise Invalid(f"Cannot have a default value if default is false [[{hex(object['index'])}]]")
    return objects

def validate_object(objects):
    # return validate_profile_object(objects)
    for object in objects:
        if object["profile"] == 0:
            if not object["data"] or any("type" not in data or "name" not in data or "access" not in data for data in object["data"]):
                raise Invalid(f"Standard object need data fields [[{hex(object['index'])}]]")
    return objects

profile_schema = Schema({
    Required("profiles") : [{
        Required("profile") : int,
        Required("name") : str,
        Required("objects") : All([{
            Required("index") : int,
            Required("name") : str,
            Optional("category", default="optional") : str,
            Optional("type", default="none") : str,
            Required("data") : [{
                Optional("type", default="none") : str,
                Optional("name") : str,
                Required("access") : All(str, validate_access),
                Optional("pdo_mapping", default=False) : bool,
                Optional("default", default=True ): bool,
                Optional("default_value", default="none") : Any(str, int, float, bool)
            }]
        }],
        Length(min=1),
        validate_profile_object)
    }]
})

config_schema = Schema({
    Required("objectDictionary") : All([{
        Required("alias") : str,
        Required("index") : int,
        Optional("profile", default=0) : int,
        Optional("name", default="none") : str,
        Optional("category", default="optional") : Any("optional", "mandatory", "conditional"),
        Optional("type", default="none") : str,
        Optional("data", default=[]) : [{
            Optional("type") : str,
            Optional("name") : str,
            Optional("access") : All(str, validate_access),
            Optional("pdo_mapping", default=False) : bool,
            Optional("default", default=True ): bool,
            Optional("default_value", default="none") : Any(str, int, float, bool)
        }]
    }],
    validate_object)
})