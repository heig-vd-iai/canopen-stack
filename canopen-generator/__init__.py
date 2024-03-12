#!/usr/bin/python3
import os
import yaml
import jinja2
from datetime import datetime
from voluptuous import MultipleInvalid
from .schema import profile_schema, config_schema
from .type import Access, Type, DataType, ObjectType, Ctype_name, Type_code

script_dir = os.path.dirname(__file__)

TEMPLATE_DIR = os.path.join(script_dir, "templates")

DOCUMENTATION_TEMPLATE = "documentation.md.j2"
EDS_TEMPLATE = "od.eds.j2"
CPP_TEMPLATE = "cpp.j2"
HPP_TEMPLATE = "hpp.j2"
REMOTE_TEMPLATE = "remote.j2"

LOGICAL_DEVICE_OFFSET = 0x800

class Data:

    def __init__(self, data: dict) -> None:
        self.type = data["type"]
        self.name = data["name"]
        self.access = data["access"]
        self.pdo_mapping = data["pdo_mapping"]
        self.default = data["default"]
        self.low_limit = data["lowLimit"]
        self.high_limit = data["highLimit"]
        self.get = data["get"]
        self.set = data["set"]
        if str(self.default).startswith("$NODEID+"):
            self.default = int(str(self.default).split("+")[-1], 0) + 1 ## TODO: add node id
    
    @property
    def data_type(self):
        return DataType[self.type].value

class SubObject:

    def __init__(self, data: Data, index: int, subindex: int, object) -> None:
        self.index = index
        self.subindex = subindex
        self.parameter_name = data.name
        self.type = data.type
        self.type_value = data.data_type
        self.access = data.access
        self.default = data.default
        self.pdo_mapping = int(data.pdo_mapping)
        self.low_limit = data.low_limit
        self.high_limit = data.high_limit
        self.remote = object.remote
        self.get = data.get
        self.set = data.set

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
    def meta_data(self) -> str:
        meta : str = "{.bits = {"
        if self.access == "ro" or self.access == "const":
            meta += ".readable = true, .writeable = false, "
        if self.access == "wo":
            meta += ".readable = false, .writeable = true, "
        if self.access == "rw":
            meta += ".readable = true, .writeable = true, "
        if self.pdo_mapping:
            meta += ".mappable = true, "
        else:
            meta += ".mappable = false, "
        if self.low_limit == "none" or self.high_limit == "none":
            meta += ".limited = false, "
        else:
            meta += ".limited = true, "
        if self.remote == False:
            meta += ".remote = false"
        else:
            meta += ".remote = true"
        meta += "}}"
        return meta


class Object:

    def __init__(self, object: dict, profiles: dict, index: int, axis: int) -> None:
        self.index = index + LOGICAL_DEVICE_OFFSET  * axis
        self.alias = object["alias"]
        self.profile = object["profile"]
        self.remote = object["remote"]
        self.description = object["description"]
        self.module = object["module"]
        self.get = object["get"]
        self.set = object["set"]
        self.category = object["category"]
        self.logicalDevice = axis
        if self.profile != 0:
            try:
                profile = next(profile for profile in profiles if profile.profile == self.profile)
            except StopIteration:
                print(f"Profile {self.profile} not found")
                exit(1)
            try:
                profileObject = next(profileObject for profileObject in profile.objects if profileObject.index == index)
            except StopIteration:
                print(f"Object {hex(self.index)} not found in profile {profile.profile}")
                exit(1)
            self.name = profileObject.name
            self.category = profileObject.category
            if profileObject.get != "none":
                self.get = profileObject.get
            if profileObject.set != "none":
                self.set = profileObject.set
            self.data = profileObject.data
            # Overwrite data with object specific data
            if "data" in object:
                if len(object["data"]) != len(self.data):
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
        else:
            self.name = object["name"]
            self.category = object["category"]
            self.data = [Data(data) for data in object["data"]]
        for i, data in enumerate(self.data):
            if data.get == "none":
                data.get = self.get
            if data.set == "none":
                data.set = self.set
            data.get = data.get.replace("?#", str(axis))
            data.set = data.set.replace("?#", str(axis))

    @property
    def object_type(self):
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
        return [SubObject(data, self.index, index, self) for index, data in enumerate(self.data)]

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
        self.objects = [ProfileObject(object, index) for index, object in profile["objects"].items()]


class ObjectDictionary:

    def __init__(self, profiles: dict, od: dict, file_name: str) -> None:
        self.file_name = file_name
        try:
            self.profile = profile_schema(profiles)
            self.profiles = [Profile(profile, index) for index, profile in self.profile["profiles"].items()]
            self.fonctionalities = self.profile["functionalities"]
        except MultipleInvalid as e:
            print("sw-motion-generator profile error: " + str(e))
            exit(1)
        try:
            self.config = config_schema(od)
            self.objects = []
            for object in self.config["objectDictionary"].items():
                if "logicalDevices" not in object[1]:
                    self.objects.append(Object(object[1], self.profiles, object[0], 0))
                else:
                    for axis in object[1]["logicalDevices"]:
                        self.objects.append(Object(object[1], self.profiles, object[0], axis))
            self.info = self.config["info"]
            self.logical_devices = self.config["logicalDevices"]
        except MultipleInvalid as e:
            print("sw-motion-generator objectDictionary error: " + str(e))
            exit(1)

        # Configure logical devices object with profile used
        if len(self.logical_devices) == 1:
            additional_info = "0000"
        else:
            additional_info = "FFFF"
        try:
            device_type_index = next(index for index, object in enumerate(self.objects) if object.index == 0x1000)
        except StopIteration:
            print("Object 0x1000 Device type not found")
            exit(1)
        self.objects[device_type_index].data[0].default = f"0x {additional_info}{self.logical_devices[0]:04X}"
        for i in range(1, len(self.logical_devices)):
            self.objects.append(Object({
                "name": f"Device Type Logical Device {i}",
                "alias": f"deviceTypeLogicalDevice{i}",
                "profile": 0,
                "remote": False,
                "description": "Logical device",
                "module": "none",
                "get": "none",
                "set": "none",
                "category": "optional",
                "data": [{"type": "int16", "name":"none", "access": "ro", "pdo_mapping": 0, "default": f"0x{self.logical_devices[i]:04X}", "lowLimit": "none", "highLimit": "none", "get": "none", "set": "none"}]
            }, self.profiles, 0x67FF, i-1))
        self.objects.sort(key=lambda x: x.index)

        # count pdo parameter
        self.nrOfRXPDO = 0
        self.nrOfTXPDO = 0
        for object in self.objects:
            if object.index >= 0x1400 and object.index < 0x1600:
                self.nrOfRXPDO += 1
            if object.index >= 0x1800 and object.index < 0x1A00:
                self.nrOfTXPDO += 1

        self.mandatoryObjects = [object for object in self.objects if object.category == "mandatory"]
        self.optionalObjects = [object for object in self.objects if object.category == "optional"]
    
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
        return type_num

    def to_md(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(DOCUMENTATION_TEMPLATE)
        return template.render(profiles=self.profiles, objects=self.objects)

    def to_eds(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(EDS_TEMPLATE)
        return template.render(
            objects=self.objects, 
            file_name=self.file_name, 
            info=self.info,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects)
    
    def to_cpp(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(CPP_TEMPLATE)
        return template.render(
            objects=self.objects, 
            info=self.info,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects)

    def to_hpp(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(HPP_TEMPLATE)
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
            type_count=self.type_count,
            subindex_count=self.subindex_count,
            node_id=self.info["device"]["nodeID"])

    def to_remote(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(REMOTE_TEMPLATE)
        return template.render(
            objects=self.objects, 
            info=self.info,
            fonctionalities=self.fonctionalities,
            time=datetime.now().strftime("%H:%M"),
            date=datetime.now().strftime("%Y-%m-%d"),
            nrOfRXPDO=self.nrOfRXPDO,
            nrOfTXPDO=self.nrOfTXPDO,
            mandatoryObjects=self.mandatoryObjects,
            optionalObjects=self.optionalObjects)


