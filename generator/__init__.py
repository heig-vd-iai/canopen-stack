import configparser
import datetime
import io
import os
from collections import defaultdict

import yaml

from .tree import bst_to_array_zero_indexed, build_balanced_bst


def flatten_od(od):
    """
    Flattens the obj dictionary. Hierarchical objects in the YAML file
    are only a convenience for the user. The actual obj dictionary is flat.
    This function takes the hierarchical obj dictionary and flattens it
    into a flat dictionary.

    It also adds additional information such as c/c++ type.

    """
    flat_od = {}
    for index, obj in od.items():
        match obj.type:
            case "var":
                flat_od[ObjectCode(index, 0)] = obj
            case "array":
                for subindex in range(obj.length):
                    flat_od[ObjectCode(index, subindex + 1)] = {
                        **obj,
                        "datatype": obj.datatype,
                        "type": "var",
                        "name": f'{obj.name} {subindex + 1}',
                        "access": obj.access,
                        "get": obj.get.replace("#", str(subindex + 1)),
                        "set": obj.set.replace("#", str(subindex + 1)),
                    }
            case "record":
                for subindex, subobject in enumerate(obj.subindex):
                    flat_od[ObjectCode(index, subindex + 1)] = {
                        **subobject,
                        "type": "var",
                    }
            case _:
                raise ValueError(f"Unknown type: {obj['type']}")

    # Add additional information to the flat obj dictionary
    # used by the Jinja templates
    for code, obj in flat_od.items():
        obj.id = code

    # Set the default index value.
    # Default values are grouped by types.
    index_counter = defaultdict(int)
    for obj in flat_od.values():
        if obj["default"] is not None:
            index_counter[obj["datatype"]] += 1

    return flat_od


def get_objects_per_type(od_flat):
    """
    Returns a dictionary with the number of objects per type.
    The keys are the datatype.
    """
    objects_per_type = {}
    for obj in od_flat.values():
        if obj["default"] is not None:
            if obj["datatype"] not in objects_per_type:
                objects_per_type[obj["datatype"]] = None
            objects_per_type[obj["datatype"]] = obj["default"]
    return objects_per_type


def get_bst_search_array(od_flat):
    """
    Returns a dictionary with the number of objects per type.
    The keys are the datatype.
    """
    root = build_balanced_bst(list(od_flat.items()))
    return bst_to_array_zero_indexed(root)


def get_git_info():
    def to_date(date):
        return datetime.datetime.strptime(date, "%Y-%m-%d %H:%M:%S %z")

    return {
        "committer": os.popen("git config user.name").read().strip(),
        "branch": os.popen("git rev-parse --abbrev-ref HEAD").read().strip(),
        "commit": os.popen("git rev-parse HEAD").read().strip(),
        "date": to_date(os.popen("git show -s --format=%ci HEAD").read().strip()),
        "first_commit_date": to_date(
            os.popen("git show -s --format=%ci $(git rev-list --max-parents=0 HEAD)")
            .read()
            .strip()
        ),
    }


class Config:
    def __init__(self, filename: str):
        with open(filename, "r") as f:
            data = yaml.safe_load(f)

        self.config = SchemaConfig(data)
        self.flattened_od = flatten_od(self.config["objects"])
        self.objects_per_type = get_objects_per_type(self.flattened_od)
        self.bst_search_array = get_bst_search_array(self.flattened_od)

        try:
            self.git_info = get_git_info()
        except Exception as e:
            self.git_info = {
                "committer": "unknown",
                "branch": "unknown",
                "commit": "unknown",
                "date": datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                "first_commit_date": datetime.datetime.now().strftime(
                    "%Y-%m-%d %H:%M:%S"
                ),
            }
            print(f"Error getting git info: {e}")

    def dump_enum(self, filename: str = None):
        return render_template("enum.hpp.j2", od_flat=self.flattened_od)

    def dump_eds_dict(self, filename: str = None):
        revision_number = 0x12345678

        baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}
        supported_baudrates = {
            f"BaudRate_{baudrate}": baudrate in self.config["device"]["baudrate"]
            for baudrate in baudrates
        }

        eds = {
            "FileInfo": {
                "CreatedBy": self.git_info["committer"],
                "ModifiedBy": self.git_info["committer"],
                "Description": "Elmo Gold drive EDS file",
                "CreationTime": self.git_info["first_commit_date"].strftime("%I:%M%p"),
                "CreationDate": self.git_info["first_commit_date"].strftime("%d-%m-%Y"),
                "ModificationTime": datetime.datetime.now().strftime("%I:%M%p"),
                "ModificationDate": datetime.datetime.now().strftime("%d-%m-%Y"),
                "FileName": filename if filename else "unknown.eds",
                "FileVersion": "002",
                "FileRevision": "01",
                "EDSVersion": "4.0",
            },
            "DeviceInfo": {
                "VendorName": self.config["device"]["vendor"]["name"],
                "VendorNumber": self.config["device"]["vendor"]["number"],
                "ProductName": self.config["device"]["product"]["name"],
                "ProductNumber": self.config["device"]["product"]["number"],
                "RevisionNumber": revision_number,
                "OrderCode": 1,
                "SimpleBootUpMaster": 0,
                "SimpleBootUpSlave": 0,
                "Granularity": 8,
                "DynamicChannelsSupported": 0,
                "CompactPDO": 23,
                "GroupMessaging": 0,
                "NrOfRXPDO": 4,
                "NrOfTXPDO": 4,
                "LSS_Supported": 0,
                **supported_baudrates,
            },
            "MandatoryObjects": {},
            "OptionalObjects": {},
        }

        for code, item in self.flattened_od.items():
            access = "ro"
            if {"r", "w"} <= set(item["access"]):
                access = "rw"
            elif {"r"} <= set(item["access"]):
                access = "ro"
            elif {"w"} <= set(item["access"]):
                access = "wo"

            highlimit = (
                {"HighLimit": str(item["limits"]["max"])}
                if item["limits"]["max"] is not None
                else {}
            )
            lowlimit = (
                {"LowLimit": str(item["limits"]["min"])}
                if item["limits"]["min"] is not None
                else {}
            )

            key = hex(code.index)[2:]
            keyfull = f"{key}sub{code.subindex:02x}"

            if "parent_type" in item:
                eds[key] = {
                    "ParameterName": item["name"],
                    "ObjectType": str(object_types[item["parent_type"]]),
                    "DataType": str(item["datatype"].code),
                    "SubNumber": str(item["default"]),
                }

            eds[keyfull] = {
                "ParameterName": item["name"],
                "ObjectType": object_types[item["type"]],
                "DataType": item["datatype"].code,
                "AccessType": access,
                "DefaultValue": str(item["default"]),
                "PDOMapping": str(int(bool(item["pdo"])) if "pdo" in item else 0),
                **lowlimit,
                **highlimit,
            }
        return eds

    def dump_eds(self, filename: str = None):
        eds = self.dump_eds_dict(filename)

        config = configparser.ConfigParser()
        config.optionxform = str  # Preserve case of keys
        config.read_dict(eds)

        if filename is None:
            output = io.StringIO()
            config.write(output)
            return output.getvalue()

        with open(filename, "w") as configfile:
            config.write(configfile)
            configfile.write("\n")

        return eds
