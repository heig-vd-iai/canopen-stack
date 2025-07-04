"""Validation schema for the config.yaml file."""

from typing import Annotated, Any, Dict, List, Tuple, Union

from pydantic import BaseModel, Field, RootModel, ValidationError, model_validator
from pydantic_core import ErrorDetails

from .. import Array, Device, Record, Var
from ..helpers import infer_object_type
from .mixins import MappingRootMixin

ObjectType = Annotated[Union[Var, Array, Record], Field(discriminator="type")]


class Objects(MappingRootMixin[ObjectType], RootModel[Dict[int, ObjectType]]):
    """Mapping of object index to object definition."""

    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        """Resolve inheritance and infer types for objects."""
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        def resolve(idx: int, visited: set[int]) -> dict:
            obj = v[idx]
            inherit_id = obj.get("inherit")
            if not inherit_id:
                return obj
            if inherit_id not in v:
                raise ValueError(
                    f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h"
                )
            if inherit_id in visited:
                raise ValueError(f"Circular inheritance detected at {idx:04X}h")
            base = resolve(inherit_id, visited | {idx})
            merged = {**base, **obj}
            return merged

        v = {idx: resolve(idx, set()) for idx in v}

        for obj in v.values():
            obj.setdefault("type", infer_object_type(obj))

        return v


class SchemaConfig(BaseModel):
    """Configuration schema for the generator."""

    device: Device
    profiles: List[Annotated[int, Field(ge=300, le=500)]]
    objects: Objects


def Config(config_data) -> Tuple[Union[SchemaConfig, None], List[ErrorDetails]]:
    """Validate the configuration data against the SchemaConfig model."""
    config = None
    errors = []
    try:
        config = SchemaConfig.model_validate(config_data)
    except ValidationError as e:
        errors = e.errors()
        return None, errors

    return config, errors
