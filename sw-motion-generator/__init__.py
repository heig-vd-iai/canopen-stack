#!/usr/bin/python3
import os
import yaml
import jinja2
from datetime import datetime
from voluptuous import MultipleInvalid
from .schema import profile_schema, config_schema
from .type import Access, Type, DataType, ObjectType

script_dir = os.path.dirname(__file__)

TEMPLATE_DIR = os.path.join(script_dir, "templates")

DOCUMENTATION_TEMPLATE = "documentation.md.j2"
EDS_TEMPLATE = "od.eds.j2"

LOGICAL_DEVICE_OFFSET = 0x800

class Data:
    type: str
    name: str
    access: str
    pdo_mapping: bool
    default: bool

    def __init__(self, data: dict) -> None:
        self.type = data["type"]
        self.name = data["name"]
        self.access = data["access"]
        self.pdo_mapping = data["pdo_mapping"]
        self.default = data["default"]
    
    @property
    def data_type(self):
        return DataType[self.type].value

class SubObject:
    index: int
    subindex: int
    parameter_name: str
    data_type: DataType
    access: Access
    default: str
    pdo_mapping: bool

    def __init__(self, data: Data, index: int, subindex: int) -> None:
        self.index = index
        self.subindex = subindex
        self.parameter_name = data.name
        self.data_type = data.data_type
        self.access = data.access
        self.default = data.default
        self.pdo_mapping = int(data.pdo_mapping)

    @property
    def index_hex(self):
        return hex(self.index)[2:]


class Object:
    alias: str
    index: int
    profile: int
    name: str
    category: str
    type: str
    data: []

    def __init__(self, object: dict, profiles: dict, index: int, axis: int) -> None:
        self.index = index + LOGICAL_DEVICE_OFFSET  * axis
        self.alias = object["alias"]
        self.profile = object["profile"]
        self.remote = object["remote"]
        if "logicalDevices" in object:
            self.getter = object["logicalDevices"][axis]["get"]
            self.setter = object["logicalDevices"][axis]["set"]
        else:
            self.getter = object["get"]
            self.setter = object["set"]
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
            self.data = profileObject.data
            if "data" in object:
                if len(object["data"]) != len(self.data):
                    print(f"Data length mismatch for object {hex(self.index)}")
                    exit(1)
                for i, data in enumerate(object["data"]):
                    self.data[i].default = data["default"]
                    if "pdo_mapping" in data:
                        self.data[i].pdo_mapping = data["pdo_mapping"]
                    
        else:
            self.name = object["name"]
            self.category = object["category"]
            self.data = [Data(data) for data in object["data"]]

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
        return [SubObject(data, self.index, index) for index, data in enumerate(self.data)]

    @property
    def index_hex(self):
        return hex(self.index)[2:]
            

class ProfileObject:
    index: int
    name: str
    category: str
    data: []

    def __init__(self, object: dict) -> None:
        self.index = object["index"]
        self.name = object["name"]
        self.category = object["category"]
        self.data = [Data(data) for data in object["data"]]

    def to_md(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(DOCUMENTATION_TEMPLATE)
        return template.render(object=self)

class Profile:
    profile: int
    name: str
    objects: []

    def __init__(self, profile: dict) -> None:
        self.profile = profile["profile"]
        self.name = profile["name"]
        self.objects = [ProfileObject(object) for object in profile["objects"]]


class ObjectDictionary:
    profiles: []
    objects: []

    def __init__(self, profiles: dict, od: dict, file_name: str) -> None:
        self.file_name = file_name
        try:
            self.profile = profile_schema(profiles)
            self.profiles = [Profile(profile) for profile in self.profile["profiles"]]
            self.fonctionalities = self.profile["functionalities"]
        except MultipleInvalid as e:
            print("sw-motion-generator profile error: " + str(e))
            exit(1)
        try:
            self.config = config_schema(od)
            # self.objects = [Object(object, self.profiles) for object in self.config["objectDictionary"].items()]
            self.objects = []
            for object in self.config["objectDictionary"].items():
                if "logicalDevices" not in object[1]:
                    self.objects.append(Object(object[1], self.profiles, object[0], 0))
                else:
                    for axis in object[1]["logicalDevices"]:
                        self.objects.append(Object(object[1], self.profiles, object[0], axis))
            self.objects.sort(key=lambda x: x.index)
            self.info = self.config["info"]
        except MultipleInvalid as e:
            print("sw-motion-generator objectDictionary error: " + str(e))
            exit(1)

        self.nrOfRXPDO = 0
        self.nrOfTXPDO = 0
        for object in self.objects:
            if object.index >= 0x1400 and object.index < 0x1600:
                self.nrOfRXPDO += 1
            if object.index >= 0x1800 and object.index < 0x1A00:
                self.nrOfTXPDO += 1

        self.mandatoryObjects = [object for object in self.objects if object.category == "mandatory"]
        self.optionalObjects = [object for object in self.objects if object.category == "optional"]

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


