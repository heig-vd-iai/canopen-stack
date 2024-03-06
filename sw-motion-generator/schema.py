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
            
        if len(object["data"]) == 1:
            object["data"][0]["name"] = object["name"]
    return objects

def validate_object(objects):
    for object in objects:
        if object["profile"] == 0:
            if not object["data"] or any("type" not in data or "name" not in data or "access" not in data for data in object["data"]):
                raise Invalid(f"Non Standard object need data fields [[{hex(object['index'])}]]")
    return objects

data_schema = [{
                Required("type") : str,
                Optional("name", default="none") : str,
                Required("access") : All(str, validate_access),
                Optional("pdo_mapping", default=False) : bool,
                Optional("default", default="none") : Any(int, float, bool, str)
            }]

data_object_schema = [{
    Optional("pdo_mapping", default=False) : bool,
    Required("default") : Any(int, float, bool, str),
}]


object_schema_base = {
    Required("index"): int,
    Required("name"): str,
    Optional("category", default="optional"): str,
    Required("data"): data_schema
}

profile_schema = Schema({
    Required("functionalities"): {
        Required("baudrate"): {
            Optional("baudrate_10", default=False): bool,
            Optional("baudrate_20", default=False): bool,
            Optional("baudrate_50", default=False): bool,
            Optional("baudrate_125", default=False): bool,
            Optional("baudrate_250", default=False): bool,
            Optional("baudrate_500", default=False): bool,
            Optional("baudrate_800", default=False): bool,
            Optional("baudrate_1000", default=False): bool
        },
        Required("simpleBootUpMaster"): bool,
        Required("simpleBootUpSlave"): bool,
        Required("granularity"): int,
        Required("dynamicChannelsSupported"): bool,
        Required("compactPDO"): bool,
        Required("groupMessaging"): bool,
        Required("LSS_Supported"): bool
    },
    Required("profiles"): [{
        Required("profile"): int,
        Required("name"): str,
        Required("objects"): All([object_schema_base], Length(min=1), validate_profile_object)
    }]
})

config_schema = Schema({
    Required("fileInfo"): {
        Required("fileVersion") : int,
        Required("fileRevision") : int,
        Required("EDSVersion", default="4.0") : All(str, Length(min=3, max=3)),
        Required("description") : str,
        Required("createdBy") : str,
        Required("creationTime"): str,
        Required("creationDate") : str,
        Required("modifiedBy") : str,
    },
    Required("deviceInfo"): {
        Required("vendorName"): str,
        Required("vendorNumber"): int,
        Required("productNumber"): int,
        Required("revisionNumber"): int,
        Required("orderCode"): int
    },
    Required("objectDictionary"): All([{
        Required("alias"): str,
        Required("index"): int,
        Optional("profile", default=0): int,
        Optional("name", default="none"): str,
        Optional("category", default="optional"): Any("optional", "mandatory", "conditional"),
        Optional("data", default=[]): Any(data_schema, data_object_schema),
    }], validate_object)
})