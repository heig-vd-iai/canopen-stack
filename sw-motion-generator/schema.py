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
    for index, object in objects.items():
        if index >= 0x6000:
            if "logicalDevices" not in object:
                object["logicalDevices"] = 0
            if isinstance(object["logicalDevices"], list):
                axis_dict = {}
                for axis in object["logicalDevices"]:
                    axis_dict[axis] = {"get": object["get"].replace("?#", str(axis)), "set": object["set"].replace("?#", str(axis))}
                object["logicalDevices"] = axis_dict
            if isinstance(object["logicalDevices"], int):
                object["logicalDevices"] = {object["logicalDevices"]: {"get": object["get"].replace("?#", "0"), "set": object["set"].replace("?#", "0")}
            }
        else:
            if "logicalDevices" in object:
                raise Invalid(f"Only Standardized profile area object can have logical device field [{hex(index)}]")
        if "data" in object:
            if len(object["data"]) > 1 and not all("name" in data for data in object["data"]):
                raise Invalid(f"If there are more than one data fields, they must have a name [{hex(index)}]")
            if len(object["data"]) == 1:
                object["data"][0]["name"] = object["name"]
    return objects

data_schema = [{
    Required("type") : str,
    Optional("name", default="none") : str,
    Required("access") : All(str, validate_access),
    Optional("pdo_mapping", default=False) : bool,
    Optional("default", default="none") : Any(int, float, bool, str),
    Optional("lowLimit", default="none"): str,
    Optional("highLimit", default="none"): str,
}]

data_object_schema = [{
    Optional("pdo_mapping", default=False) : bool,
    Required("default") : Any(int, float, bool, str),
    Optional("lowLimit", default="none"): str,
    Optional("highLimit", default="none"): str,
}]


object_schema_base = {
    Required("index"): int,
    Required("name"): str,
    Optional("category", default="optional"): str,
    Required("data"): data_schema
}

profile_schema = Schema({
    Required("functionalities"): {
        Required("EDSVersion", default="4.0") : All(str, Length(min=3, max=3)),
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
    Required("info"): {
        Required("fileVersion") : int,
        Required("fileRevision") : int,
        Required("description") : str,
        Required("createdBy") : str,
        Required("creationTime"): str,
        Required("creationDate") : str,
        Required("modifiedBy") : str,
        Required("device"): {
            Required("vendorName"): str,
            Required("vendorNumber"): int,
            Required("productNumber"): int,
            Required("revisionNumber"): int,
            Required("orderCode"): int
        },
    },
    
    Required("objectDictionary"): All({
        int: {
            Required("alias"): str,
            Optional("profile", default=0): int,
            Optional("name", default="none"): str,
            Optional("module", default=""): str,
            Optional("description", default=""): str,
            Optional("descriptionFile", default=""): str,
            Optional("category", default="optional"): Any("optional", "mandatory", "conditional"),
            Optional("data"): Any(data_schema, data_object_schema),
            Optional("logicalDevices"): Any([int], int, {
                int:{
                    Optional("get", default="none"): str,
                    Optional("set", default="none"): str,
                }
            }),
            Optional("get", default="none"): str,
            Optional("set", default="none"): str,
            Optional("remote", default=False): bool
        }
    }, validate_object)
})