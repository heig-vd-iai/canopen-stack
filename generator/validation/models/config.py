"""Validation schema for a device configuration file."""

from typing import Annotated, Any, Dict, List, Union

from pydantic import BaseModel, ConfigDict, Field, RootModel, model_validator

from ..helpers import infer_object_type, resolve_inheritance
from .array import Array
from .device_info import Device
from .file_info import Info
from .mixins import MappingRootMixin
from .record import Record
from .var import Var

ObjectType = Annotated[Union[Var, Array, Record], Field(discriminator="type")]

ProfileId = Annotated[int, Field(ge=300, le=500)]


class Objects(MappingRootMixin[ObjectType], RootModel[Dict[int, ObjectType]]):
    """Mapping of object index to object definition."""

    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        """Resolve inheritance and infer types for objects."""
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        v = resolve_inheritance(v)

        for obj in v.values():
            obj.setdefault("type", infer_object_type(obj))

        return v


class SchemaConfig(BaseModel):
    """Configuration schema for the generator."""

    model_config = ConfigDict(extra="forbid")

    device: Device
    info: Info = Info()
    profiles: List[ProfileId] = []
    objects: Objects

    @property
    def device_profile(self) -> int:
        """The CiA device profile number announced by object 0x1000."""
        return next((p for p in self.profiles if p >= 400), 0)
