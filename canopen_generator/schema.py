from voluptuous import All, Any, Invalid, Length, Optional, Required, Schema

from .type import Access
import datetime
import warnings
import semver

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

def deprecated(data):
    """ EDS by default use a fileVersion which is an int. We want to enforce
    the use of semver, or much better, take the version tag from Git. """
    major, minor = 0, 0
    if 'fileVersion' in data:
        warnings.warn("'fileVersion' is deprecated, use 'version' instead", DeprecationWarning)
        if ('version' in data):
            raise Invalid("Both 'fileVersion' and 'version' are present")
        major = int(data['fileVersion'])
        del data['fileVersion']
    if 'fileRevision' in data:
        warnings.warn("'fileRevision' is deprecated, use 'version' instead", DeprecationWarning)
        if ('version' in data):
            raise Invalid("Both 'fileRevision' and 'version' are present")
        minor = int(data['fileRevision'])
        del data['fileRevision']

    if major or minor:
        data['version'] = semver.VersionInfo(major, minor)

    return data

semver_schema = All(
    Any(
        [int],
        All(str, lambda s: [int(x) for x in s.split('.')])
    ), Length(min=3, max=3)
)

data_schema = [
    {
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
    }
]

data_object_schema = [
    {
        Optional("length", default=1): int,
        Optional("pdo_mapping"): bool,
        Optional("default"): Any(int, float, bool, str),
        Optional("lowLimit"): Any(str, int, float),
        Optional("highLimit"): Any(str, int, float),
        Optional("get"): str,
        Optional("set"): str,
    }
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
        Required("info"): All({

            Required("description"): str,

            # Normally taken from Git
            Optional("createdBy"): Any(str, [str]),
            Optional("createdAt"): datetime.datetime,
            Optional("version"): semver_schema,

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

            # Deprecated values
            Required("fileVersion"): int,
            Optional("fileRevision"): int,

            Optional("creationTime"): str,
            Optional("creationDate"): str,
        },
           deprecated,
        ),

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
                    Required("alias"): str,
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
                    Optional("remote", default=False): bool,
                    Optional("unit", default="none"): str,
                }
            },
            validate_object,
        ),
        Optional("modules_description"): All(
            {str: {Optional("description"): str, Optional("descriptionFile"): str}}
        ),
    }
)
