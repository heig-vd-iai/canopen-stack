#!/usr/bin/python3
import os
import yaml
import jinja2
from voluptuous import MultipleInvalid
from .schema import profile_schema, config_schema
from .type import Access, Type, DataType, ObjectType

script_dir = os.path.dirname(__file__)

TEMPLATE_DIR = os.path.join(script_dir, "templates")

DOCUMENTATION_TEMPLATE = "documentation.md.j2"
EDS_TEMPLATE = "od.eds.j2"

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
        self.default_value = data["default_value"]
    
    @property
    def data_type(self):
        return DataType[self.type].value

class SubObject:
    index: int
    subindex: int
    parameter_name: str
    data_type: DataType
    access: Access
    default_value: str
    pdo_mapping: bool

    def __init__(self, data: Data, index: int, subindex: int) -> None:
        self.index = index
        self.subindex = subindex
        self.parameter_name = data.name
        self.data_type = data.data_type
        self.access = data.access
        self.default_value = data.default_value
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

    def __init__(self, object: dict, profiles: dict) -> None:
        self.alias = object["alias"]
        self.index = object["index"]
        self.profile = object["profile"]
        if self.profile != 0:
            profile = next(profile for profile in profiles if profile.profile == self.profile)
            profileObject = next(profileObject for profileObject in profile.objects if profileObject.index == self.index)
            self.name = profileObject.name
            self.category = profileObject.category
            self.data = profileObject.data
        else:
            self.name = object["name"]
            self.category = object["category"]
            self.type = object["type"]
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

    def __init__(self, profiles: dict, od: dict) -> None:
        try:
            self.profiles = [Profile(profile) for profile in profile_schema(profiles)["profiles"]]
        except MultipleInvalid as e:
            print("sw-motion-generator profile error: " + str(e))
            exit(1)
        try:
            self.objects = [Object(object, self.profiles) for object in config_schema(od)["objectDictionary"]]
        except MultipleInvalid as e:
            print("sw-motion-generator objectDictionary error: " + str(e))
            exit(1)

    def to_md(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(DOCUMENTATION_TEMPLATE)
        return template.render(profiles=self.profiles)

    def to_eds(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(EDS_TEMPLATE)
        return template.render(objects=self.objects)


