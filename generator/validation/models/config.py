"""Validation schema for a device configuration file."""

from typing import Annotated, Any, Dict, List, Optional, Tuple, Union

from pydantic import (
    BaseModel,
    ConfigDict,
    Field,
    RootModel,
    ValidationError,
    model_validator,
)
from pydantic_core import ErrorDetails

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


def validate_config(
    config_data,
) -> Tuple[Optional[SchemaConfig], List[ErrorDetails]]:
    """Validate raw configuration data, returning the model or the errors."""
    try:
        return SchemaConfig.model_validate(config_data), []
    except ValidationError as error:
        return None, error.errors()
