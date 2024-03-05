#!/usr/bin/python3
import os
import yaml
import jinja2
from .type import Access, Type
from .schema import profile_schema, config_schema

script_dir = os.path.dirname(__file__)

TEMPLATE_DIR = os.path.join(script_dir, "templates")

DOCUMENTATION_TEMPLATE = "documentation.md.jinja"
EDS_TEMPLATE = "od.eds.jinja"

class Data:
    type: str
    name: str
    access: Access
    pdo_mapping: bool
    default: bool

    def __init__(self, data: dict) -> None:
        self.type = data["type"]
        self.name = data["name"]
        self.access = data["access"]
        self.pdo_mapping = data["pdo_mapping"]
        self.default = data["default"]
        self.default_value = data["default_value"]

class Object:
    alias: str
    index: int
    profile: int
    name: str
    category: str
    type: str
    data: []

    def __init__(self, object: dict) -> None:
        print(object)
        self.alias = object["alias"]
        self.index = object["index"]
        self.profile = object["profile"]
        self.name = object["name"]
        self.category = object["category"]
        self.type = object["type"]
        self.data = [Data(data) for data in object["data"]]

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
        self.profiles = [Profile(profile) for profile in profile_schema(profiles)["profiles"]]
        self.objects = [Object(object) for object in config_schema(od)["objectDictionary"]]


    def to_md(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(DOCUMENTATION_TEMPLATE)
        return template.render(profiles=self.profiles)

    def to_eds(self):
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATE_DIR), trim_blocks=True, lstrip_blocks=True)
        template = env.get_template(EDS_TEMPLATE)
        return template.render(profiles=self.profiles)


