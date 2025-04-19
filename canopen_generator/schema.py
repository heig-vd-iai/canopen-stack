from voluptuous import All, Any, Invalid, Length, Optional, Required, Schema, Coerce

from .type import Access


def validate_access(value):
    if value not in Access.__members__:
        raise Invalid(
            f"Invalid access type. Must be one of {list(Access.__members__.values())}"
        )
    return value


def validate_profile_object(objects):
    for _, object in objects.items():
        if len(object["data"]) > 1 and not all(
            "name" in data for data in object["data"]
        ):
            raise Invalid(
                f"If there are more than one data fields, they must have a name [{hex(object['index'])}]"
            )

        if len(object["data"]) == 1:
            object["data"][0]["name"] = object["name"]
    return objects


def validate_object(objects):
    for index, object in objects.items():
        if index < 0x6000:
            if "logicalDevices" in object:
                raise Invalid(
                    f"Only Standardized profile area object can have logical device field [{hex(index)}]"
                )
        if "data" in object and object["profile"] == 0:
            if len(object["data"]) > 1 and not all(
                "name" in data for data in object["data"]
            ):
                raise Invalid(
                    f"If there are more than one data fields, they must have a name [{hex(index)}]"
                )
            if len(object["data"]) == 1:
                object["data"][0]["name"] = object["name"]
    return objects


def coerce_int_to_enum(value):
    if isinstance(value, int):
        return {"value": value}
    elif isinstance(value, dict):
        return value

    raise ValueError

def coerce_to_enum_items(value):
    if isinstance(value, dict):
        return {
            'class': None,
            'data': value
        }

    raise ValueError

enum_items = {
    str: All(
        Coerce(coerce_int_to_enum),
        {"value": int, Optional("name"): str, Optional("documentation"): str},
    )
}

config_enum = Any(
    {
        Required("class"): str,
        Required("data"): enum_items
    },
    All(enum_items, Coerce(coerce_to_enum_items))
)

def mutually_exclusive_accessor(obj):
    if ('get' in obj and obj['get'] != 'none' or 'set' in obj and obj['set'] != 'none') and 'attribute' in obj:
        print("PROUT", obj['get'], obj['set'], obj['attribute'])
        raise Invalid("Cannot define 'get'/'set' together with 'attribute'")

    return obj

data_schema = [
    All({
        Required("type"): str,
        Optional("name"): str,
        Optional("length", default=1): int,
        Required("access"): All(str, validate_access),
        Optional("pdo_mapping", default=False): bool,
        Optional("default", default=0): Any(int, float, bool, str),
        Optional("lowLimit", default="none"): Any(str, int, float),
        Optional("highLimit", default="none"): Any(str, int, float),
        Optional("get", default="none"): str,
        Optional("set", default="none"): str,
        Optional("attribute"): str,
        Optional("enum"): config_enum,
        Optional("documentation", default=""): str,
    }, mutually_exclusive_accessor)
]

data_object_schema = [
    All({
        Optional("length", default=1): int,
        Optional("pdo_mapping"): bool,
        Optional("default"): Any(int, float, bool, str),
        Optional("lowLimit"): Any(str, int, float),
        Optional("highLimit"): Any(str, int, float),
        Optional("get"): str,
        Optional("set"): str,
        Optional("attribute"): str,
        Optional("enum"): config_enum,
        Optional("documentation", default=""): str,
    }, mutually_exclusive_accessor)
]

profile_schema = Schema(
    {
        Required("functionalities"): {
            Required("baudrate"): {
                Optional(10, default=False): bool,
                Optional(20, default=False): bool,
                Optional(50, default=False): bool,
                Optional(125, default=False): bool,
                Optional(250, default=False): bool,
                Optional(500, default=False): bool,
                Optional(800, default=False): bool,
                Optional(1000, default=False): bool,
            },
            Required("simpleBootUpMaster"): bool,
            Required("simpleBootUpSlave"): bool,
            Required("granularity"): int,
            Required("dynamicChannelsSupported"): bool,
            Required("compactPDO"): bool,
            Required("groupMessaging"): bool,
            Required("LSS_Supported"): bool,
        },
        Required("profiles"): {
            int: {
                Required("name"): str,
                Required("objects"): All(
                    {
                        int: {
                            Required("name"): str,
                            Optional("category", default="optional"): str,
                            Required("data"): data_schema,
                            Optional("get", default="none"): str,
                            Optional("set", default="none"): str,
                            Optional("attribute"): str,
                            # Optional("enum"): config_enum,
                        }
                    },
                    Length(min=1),
                    validate_profile_object,
                ),
            }
        },
    }
)


config_schema = Schema(
    {
        Required("info"): {
            Required("fileVersion"): int,
            Required("fileRevision"): int,
            Required("description"): str,
            Required("createdBy"): str,
            Required("creationTime"): str,
            Required("creationDate"): str,
            Required("modifiedBy"): str,
            Required("device"): {
                Required("vendorName"): str,
                Required("vendorNumber"): int,
                Required("productName"): str,
                Required("productNumber"): int,
                Required("revisionNumber"): int,
                Required("orderCode"): int,
                Required("nodeID"): int,
            },
        },
        # TODO: CANopen stack: this is not related to canopen
        # En bref, ca n'a rien à foutre ici...
        # Required("factoryParameters"): {
        #     Required("pwmFrequency"): int,
        #     Required("pwmDeadBand"): int,
        #     Required("adcSampwin"): int,
        #     Required("absoluteMaxVoltage"): int,
        #     Required("absoluteMaxCurrent"): int,
        #     Required("velocityPrescale"): int,
        # },
        Required("logicalDevices"): {int: int},
        Required("objectDictionary"): All(
            {
                int: {
                    Optional("alias"): str,
                    Optional("profile", default=0): int,
                    Optional("name", default="none"): str,
                    Optional("module", default=""): str,
                    Optional("description", default=""): str,
                    Optional("descriptionFile", default=""): str,
                    Optional("category", default="optional"): Any(
                        "optional", "mandatory", "conditional"
                    ),
                    Optional("data"): Any(data_schema, data_object_schema),
                    Optional("logicalDevices"): Any([int]),
                    Optional("get", default="none"): str,
                    Optional("set", default="none"): str,
                    Optional("attribute"): str,
                    Optional("remote", default=False): bool,
                    Optional("unit", default="none"): str,
                    Optional("documentation", default=""): str,
                    # Optional("enum"): config_enum,
                }
            },
            validate_object,
        ),
        Optional("modules_description"): All(
            {str: {Optional("description"): str, Optional("descriptionFile"): str}}
        ),
    }
)
