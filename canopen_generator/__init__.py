import configparser
import io
from datetime import datetime
from pathlib import Path
import textwrap
import jinja2
import mdformat
from voluptuous import MultipleInvalid

from . import tree
from .schema import config_schema, profile_schema
from .type import Ctype_name, DataType, ObjectType, Type_code

SCRIPT_DIR = Path(__file__).parent.absolute()
TEMPLATE_DIR = SCRIPT_DIR / "templates"

DOCUMENTATION_TEMPLATE = "documentation.md.j2"
MODULE_DOCUMENTATION_TEMPLATE = "module_documentation.md.j2"

LOGICAL_DEVICE_OFFSET = 0x800

class Data:

    def __init__(self, data: dict) -> None:
        self.data = data
        self.type = data["type"]
        self.name = data["name"]
        self.access = data["access"]
        self.pdo_mapping = data["pdo_mapping"]
        self.default = data["default"]
        self.low_limit = data["lowLimit"]
        self.high_limit = data["highLimit"]
        self.get = data["get"]
        self.set = data["set"]
        self.writeable = True if self.access in ['rw', 'wo'] else False
        self.readable = True if self.access in ['rw', 'ro'] else False
        self.length = data["length"]
        if str(self.default).startswith("$NODEID+"):
            self.default = (
                int(str(self.default).split("+")[-1], 0) + 1
            )  ## TODO: add node id

    @property
    def data_type(self):
        return DataType[self.type].value

    def __repr__(self):
        return f"DATA({repr(self.data)})"

class SubObject:

    def __init__(self, data: Data, index: int, subindex: int, object) -> None:
        self.data = data
        self.prout = data
        self.index = index
        self.subindex = subindex
        self.parameter_name = data.name
        self.type = data.type
        self.access = data.access
        self.default = data.default
        self.pdo_mapping = int(data.pdo_mapping)
        self.low_limit = data.low_limit
        self.high_limit = data.high_limit
        self.remote = object.remote
        self.get = data.get
        self.set = data.set
        self.length = data.length

        # if 'encoderAVelocityResolution' in str(object.object):
        #     print("Object", object.object)
        #     print("Data", data)
    @property
    def index_hex(self):
        return hex(self.index)[2:]

    @property
    def cpp_instance_name(self):
        return f"object{self.index_hex}sub{self.subindex}"

    @property
    def ctype_name(self):
        return Ctype_name[self.type].value

    @property
    def type_code(self):
        return Type_code[self.type].value

    @property
    def type_value(self):
        return DataType[self.type].value

    @property
    def meta_data(self) -> str:
        readable = True if self.access != "wo" else False
        writeable = True if self.access != "ro" or self.access != "c" else False
        mappable = True if self.pdo_mapping else False
        limited = (
            True if self.low_limit != "none" and self.high_limit != "none" else False
        )
        remote = True if self.remote else False
        meta = f"0b{int(remote)}{int(limited)}{int(mappable)}{int(writeable)}{int(readable)}"
        return meta


class Object:
    def __init__(self, object: dict, profiles: dict, index: int, axis: int) -> None:
        self.prout = object
        self.object = object
        self.index = index + LOGICAL_DEVICE_OFFSET * axis
        self.profile = object["profile"]
        self.remote = object["remote"]
        self.description = object["description"]
        self.module = object["module"]
        self.get = object["get"]
        self.set = object["set"]
        self.attribute = object.get("attribute", None)
        self.category = object["category"]
        self.module = object["module"]
        self.logicalDevice = axis
        if self.profile != 0:
            try:
                profile = next(
                    profile for profile in profiles if profile.profile == self.profile
                )
            except StopIteration:
                print(f"Profile {self.profile} not found")
                exit(1)
            try:
                profileObject = next(
                    profileObject
                    for profileObject in profile.objects
                    if profileObject.index == index
                )
            except StopIteration:
                print(
                    f"Object {hex(self.index)} not found in profile {profile.profile}"
                )
                exit(1)
            self.name = profileObject.name
            self.category = profileObject.category
            if profileObject.get != "none":
                self.get = profileObject.get
            if profileObject.set != "none":
                self.set = profileObject.set
            self.data = profileObject.data
            length = self.data[0].length
            if length > 1:
                name = self.data[1].name
                for i in range(1, length):
                    self.data.append(
                        Data(
                            {
                                "data": self.data[1],
                                "type": self.data[1].type,
                                "name": name.replace("?#", str(i)),
                                "access": self.data[1].access,
                                "pdo_mapping": self.data[1].pdo_mapping,
                                "default": self.data[1].default,
                                "lowLimit": self.data[1].low_limit,
                                "highLimit": self.data[1].high_limit,
                                "get": self.data[1].get,
                                "set": self.data[1].set,
                                "length": 1,
                            }
                        )
                    )

            # Overwrite data with object specific data
            if "data" in object:
                if len(object["data"]) > len(self.data):
                    print(f"Data length mismatch for object {hex(self.index)}")
                    exit(1)
                for i, data in enumerate(object["data"]):
                    if "default" in data:
                        self.data[i].default = data["default"]
                    if "pdo_mapping" in data:
                        self.data[i].pdo_mapping = data["pdo_mapping"]
                    if "lowLimit" in data:
                        self.data[i].low_limit = data["lowLimit"]
                    if "highLimit" in data:
                        self.data[i].high_limit = data["highLimit"]
                    if "get" in data:
                        self.data[i].get = data["get"]
                    if "set" in data:
                        self.data[i].set = data["set"]
                    if "length" in data:
                        self.data[i].length = data["length"]
                    if "attribute" in data:
                        self.data[i].data['attribute'] = data["attribute"]

                self.data = self.data[: len(object["data"])]

        else:
            if "name" in object:
                self.name = object["name"]
            else:
                raise ValueError(f"Object name not found[{hex(index)}]")
            self.category = object["category"]
            self.data = [Data(data) for data in object["data"]]

        for _, data in enumerate(self.data):
            if data.get == "none":
                data.get = self.get
            if data.set == "none":
                data.set = self.set
            data.get = data.get.replace("?#", str(axis))
            data.set = data.set.replace("?#", str(axis))

        if 'attribute' in object:
            self.data[0].data['attribute'] = object['attribute']

    @property
    def object_type(self):
        if self.data[0].type == "domain":
            return ObjectType.domain.value
        if len(self.data) == 1:
            return ObjectType.var.value
        else:
            first_data = self.data[0].type
            if all(data.type == first_data for data in self.data):
                return ObjectType.array.value
            else:
                return ObjectType.record.value

    @property
    def get_subobjects(self):
        return [
            SubObject(data, self.index, index, self)
            for index, data in enumerate(self.data)
        ]

    @property
    def index_hex(self):
        return hex(self.index)[2:]

    @property
    def subNumber(self):
        return len(self.data)


class ProfileObject:

    def __init__(self, object: dict, index) -> None:
        self.index = index
        self.name = object["name"]
        self.category = object["category"]
        self.data = [Data(data) for data in object["data"]]
        self.get = object["get"]
        self.set = object["set"]
        self.category = object["category"]

    @property
    def index_hex(self):
        return hex(self.index)[2:]


class Profile:

    def __init__(self, profile: dict, profile_index) -> None:
        self.profile = profile_index
        self.name = profile["name"]
        self.objects = [
            ProfileObject(object, index) for index, object in profile["objects"].items()
        ]


class ObjectDictionary:
    # TODO: verifiy double object
    def __init__(self, profiles: dict, od: dict, file_name: str) -> None:
        self.file_name = file_name

        self.profile = profile_schema(profiles)
        self.profiles = [
            Profile(profile, index)
            for index, profile in self.profile["profiles"].items()
        ]
        self.fonctionalities = self.profile["functionalities"]

        self.config = config_schema(od)

        self.objects = []
        for object in self.config["objectDictionary"].items():
            if "logicalDevices" not in object[1]:
                self.objects.append(Object(object[1], self.profiles, object[0], 0))
            else:
                for axis in object[1]["logicalDevices"]:
                    self.objects.append(
                        Object(object[1], self.profiles, object[0], axis)
                    )

        self.info = self.config["info"]
        # TODO: NON rien à foutre ici
        # self.factory_parameters = self.config["factoryParameters"]
        self.logical_devices = self.config["logicalDevices"]

        self.env = jinja2.Environment(
            loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
            trim_blocks=True,
            lstrip_blocks=True,
        )

        # Configure logical devices object with profile used
        if len(self.logical_devices) == 1:
            additional_info = "0000"
        else:
            additional_info = "FFFF"
        try:
            device_type_index = next(
                index
                for index, object in enumerate(self.objects)
                if object.index == 0x1000
            )
        except StopIteration:
            print("Object 0x1000 Device type not found")
            exit(1)
        self.objects[device_type_index].data[
            0
        ].default = f"0x{additional_info}{self.logical_devices[0]:04X}"
        for i in range(1, len(self.logical_devices)):
            self.objects.append(
                Object(
                    {
                        "name": f"Device Type Logical Device {i}",
                        "alias": f"deviceTypeLogicalDevice{i}",
                        "profile": 0,
                        "remote": False,
                        "description": "Logical device",
                        "module": "none",
                        "get": "none",
                        "set": "none",
                        "category": "optional",
                        "data": [
                            {
                                "type": "int16",
                                "name": "none",
                                "access": "ro",
                                "pdo_mapping": 0,
                                "default": f"0x{self.logical_devices[i]:04X}",
                                "lowLimit": "none",
                                "highLimit": "none",
                                "get": "none",
                                "set": "none",
                            }
                        ],
                    },
                    self.profiles,
                    0x67FF,
                    i - 1,
                )
            )
        self.objects.sort(key=lambda x: x.index)

        # test if all mandatory is present
        for profile in self.profiles:
            for profileObject in profile.objects:
                if profileObject.category == "mandatory":
                    try:
                        next(
                            object
                            for object in self.objects
                            if object.index == profileObject.index
                        )
                    except StopIteration:
                        print(f"Mandatory object {hex(profileObject.index)} not found")

        # count pdo parameter
        self.nrOfRXPDO = 0
        self.nrOfTXPDO = 0
        for object in self.objects:
            if object.index >= 0x1400 and object.index < 0x1600:
                self.nrOfRXPDO += 1
            if object.index >= 0x1800 and object.index < 0x1A00:
                self.nrOfTXPDO += 1

        self.mandatoryObjects = [
            object for object in self.objects if object.category == "mandatory"
        ]
        self.optionalObjects = [
            object for object in self.objects if object.category == "optional"
        ]

        self.modules = []
        for object in self.objects:
            if object.module not in self.modules:
                self.modules.append(object.module)

        # self.modules_descriptions = self.config["modules_description"]

    @property
    def subindex_count(self):
        return sum(len(obj.data) for obj in self.objects)

    class TypeNum:
        def __init__(self) -> None:
            self.bool = 0
            self.i8 = 0
            self.i16 = 0
            self.i32 = 0
            self.i64 = 0
            self.u8 = 0
            self.u16 = 0
            self.u32 = 0
            self.u64 = 0
            self.f32 = 0
            self.f64 = 0
            self.str = 0

    @property
    def type_count(self) -> TypeNum:
        type_num = self.TypeNum()
        for obj in self.objects:
            for subobject in obj.get_subobjects:
                if subobject.type == "bool":
                    type_num.bool += 1
                elif subobject.type == "int8":
                    type_num.i8 += 1
                elif subobject.type == "int16":
                    type_num.i16 += 1
                elif subobject.type == "int32":
                    type_num.i32 += 1
                elif subobject.type == "int64":
                    type_num.i64 += 1
                elif subobject.type == "uint8":
                    type_num.u8 += 1
                elif subobject.type == "uint16":
                    type_num.u16 += 1
                elif subobject.type == "uint32":
                    type_num.u32 += 1
                elif subobject.type == "uint64":
                    type_num.u64 += 1
                elif subobject.type == "float32":
                    type_num.f32 += 1
                elif subobject.type == "float64":
                    type_num.f64 += 1
                elif subobject.type == "string":
                    type_num.str += 1
        return type_num

    def to_md(self):
        env = jinja2.Environment(
            loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
            trim_blocks=True,
            lstrip_blocks=True,
        )
        template = env.get_template(DOCUMENTATION_TEMPLATE)

        text = template.render(
            profiles=self.profiles, objects=self.objects, modules=self.modules
        )

        return mdformat.text(text, extensions={"tables"})

    def to_doc(self, module):
        # if module in self.modules_descriptions:
        #     if "descriptionFile" in self.modules_descriptions[module]:
        #         with open(
        #             self.modules_descriptions[module]["descriptionFile"], "r"
        #         ) as file:
        #             description = file.read()
        #     elif "description" in self.modules_descriptions[module]:
        #         description = self.modules_descriptions[module]["description"]
        #     else:
        #         description = ""
        # else:
        description = ""

        template = self.env.get_template(MODULE_DOCUMENTATION_TEMPLATE)
        text = template.render(
            profiles=self.profiles,
            objects=[object for object in self.objects if object.module == module],
            module=module,
            description=description,
        )
        return mdformat.text(text, extensions={"tables"})

    def to_eds(self, filename=None):
        """Generate EDS file from object dictionary"""
        config = configparser.ConfigParser(allow_no_value=True)
        config.optionxform = str  # Preserve case (not the default case on INI files)

        config["FileInfo"] = {
            "FileName": f"{self.file_name}.eds",
            "FileVersion": self.info["fileVersion"],
            "FileRevision": self.info["fileRevision"],
            "EDSVersion": "4.0",
            "Description": self.info["description"],
            "CreationTime": self.info["creationTime"],
            "CreationDate": self.info["creationDate"],
            "CreatedBy": self.info["createdBy"],
            "ModificationTime": datetime.now().strftime("%H:%M"),
            "ModificationDate": datetime.now().strftime("%Y-%m-%d"),
            "ModifiedBy": self.info["modifiedBy"],
        }

        config["DeviceInfo"] = {
            "VendorName": self.info["device"]["vendorName"],
            "VendorNumber": str(self.info["device"]["vendorNumber"]),
            "ProductName": self.info["device"]["productName"],
            "ProductNumber": str(self.info["device"]["productNumber"]),
            "RevisionNumber": str(self.info["device"]["revisionNumber"]),
            "OrderCode": self.info["device"]["orderCode"],
            **{
                f"Baudrate_{rate}": str(
                    int(self.fonctionalities["baudrate"].get(rate, False))
                )
                for rate in [10, 20, 50, 125, 250, 500, 800, 1000]
            },
            "SimpleBootUpMaster": str(
                int(self.fonctionalities.get("simpleBootUpMaster", False))
            ),
            "SimpleBootUpSlave": str(
                int(self.fonctionalities.get("simpleBootUpSlave", False))
            ),
            "Granularity": str(self.fonctionalities["granularity"]),
            "DynamicChannelsSupported": str(
                int(self.fonctionalities.get("dynamicChannelsSupported", False))
            ),
            "CompactPDO": str(int(self.fonctionalities.get("compactPDO", False))),
            "GroupMessaging": str(
                int(self.fonctionalities.get("groupMessaging", False))
            ),
            "NrOfRXPDO": str(self.nrOfRXPDO),
            "NrOfTXPDO": str(self.nrOfTXPDO),
            "LSS_Supported": str(int(self.fonctionalities.get("LSS_Supported", False))),
        }

        config["MandatoryObjects"] = {
            "SupportedObjects": str(len(self.mandatoryObjects)),
            **{
                str(i + 2): f"0x{obj.index_hex}"
                for i, obj in enumerate(self.mandatoryObjects)
            },
        }

        for obj in self.mandatoryObjects:
            if obj.object_type not in ["0x07", "0x02"]:
                config[f"{obj.index_hex}"] = {
                    "ParameterName": obj.name,
                    "ObjectType": obj.object_type,
                    "SubNumber": str(obj.subNumber),
                }

            for sub in obj.get_subobjects:
                section_name = (
                    f"{sub.index_hex}sub{sub.subindex}"
                    if obj.object_type not in ["0x07", "0x02"]
                    else sub.index_hex
                )
                config[section_name] = {
                    "ParameterName": sub.parameter_name,
                    "ObjectType": obj.object_type,
                    "DataType": sub.type_value,
                    "AccessType": sub.access,
                    "PDOMapping": sub.pdo_mapping,
                }
                if sub.default != "none":
                    config[section_name]["DefaultValue"] = str(sub.default)

        config["OptionalObjects"] = {
            "SupportedObjects": str(len(self.optionalObjects)),
            **{
                str(i + 2): f"0x{obj.index_hex}"
                for i, obj in enumerate(self.optionalObjects)
            },
        }

        for obj in self.optionalObjects:
            if obj.object_type not in ["0x07", "0x02"]:
                config[f"{obj.index_hex}"] = {
                    "ParameterName": obj.name,
                    "ObjectType": obj.object_type,
                    "SubNumber": str(obj.subNumber),
                }

            for sub in obj.get_subobjects:
                if obj.category != "mandatory":
                    section_name = (
                        f"{sub.index_hex}sub{sub.subindex}"
                        if obj.object_type not in ["0x07", "0x02"]
                        else sub.index_hex
                    )
                    config[section_name] = {
                        "ParameterName": sub.parameter_name,
                        "ObjectType": obj.object_type,
                        "DataType": sub.type_value,
                        "AccessType": sub.access,
                        "PDOMapping": sub.pdo_mapping,
                    }
                    if sub.default != "none":
                        config[section_name]["DefaultValue"] = str(sub.default)

        if filename:
            with open(filename, "w") as configfile:
                config.write(configfile)
        else:
            sb = io.StringIO()
            config.write(sb)
            return sb.getvalue()

    def to_enum(self):
        template = self.env.get_template('enum.j2')
        content = textwrap.dedent("""
            // Autogenerated file, do not edit
            #pragma once

        """)
        for object in self.objects:
            if 'enum' in object.prout:
                content += template.render(
                    object=object.index,
                    subindex=0,
                    class_name=object.prout['enum']['class'],
                    items=object.prout['enum']['data']
                ) + "\n\n"
            for sub in object.get_subobjects:
                if 'enum' in sub.data.data:
                    content += template.render(
                        object=object.index,
                        subindex=sub.subindex,
                        class_name=sub.data.data['enum']['class'],
                        items=sub.data.data['enum']['data']
                    ) + "\n\n"
        return content

    def to_modes(self):
        template = self.env.get_template('modes.j2')
        for object in self.objects:
            if object.name == "Modes of operation":
                headers = []
                modes = []
                items=object.prout["enum"]["data"]
                for item in items:
                    _, right = item.split("__", 1)
                    first, *rest = right.split("_")
                    suffix = first.lower() + "".join(word.capitalize() for word in rest)
                    mode_suffix =  "".join(word.capitalize() for word in right.split("_"))
                    headers.append(f"mode_{suffix}.hpp")
                    modes.append((f"Mode{mode_suffix}", suffix, item))
                return template.render(
                    headers=headers,
                    modes=modes
                )

    def to_cpp(self):
        template = self.env.get_template("cpp.j2")
        return template.render(
            objects=self.objects,
            info=self.info,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects,
        )

    def to_hpp(self):
        template = self.env.get_template("hpp.j2")
        return template.render(
            objects=self.objects,
            info=self.info,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            signature=datetime.now().strftime("%Y%m%d%H%M"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects,
            type_count=self.type_count,
            subindex_count=self.subindex_count,
            node_id=self.info["device"]["nodeID"],
        ) + "\n"

    def to_remote(self):
        template = self.env.get_template("remote.j2")

        # Compute BST tree for revert mapping
        prout = []
        for object in self.objects:
            for subobject in object.get_subobjects:
                if (subobject.data.access in ['ro', 'rw'] and subobject.data.data.get('attribute', None) or subobject.get) != "none" and object.remote:
                    prout.append((subobject.index << 8 | subobject.subindex, subobject.cpp_instance_name))

        sorted_pairs = sorted(prout, key=lambda x: x[0])
        root = tree.build_balanced_bst(sorted_pairs)
        tree_array = tree.bst_to_array_zero_indexed(root)

        return template.render(
            objects=self.objects,
            info=self.info,
            # factory_parameters=self.factory_parameters,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects,
            tree_array=tree_array,
        )

    # def to_config(self):
    #     env = jinja2.Environment(
    #         loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
    #         trim_blocks=True,
    #         lstrip_blocks=True,
    #     )
    #     template = env.get_template("config.j2")
    #     return template.render(
    #         factory_parameters=self.factory_parameters,
    #     )
