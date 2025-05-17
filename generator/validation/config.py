"""Validation schema for the config.yaml file."""

from typing import (
    Annotated,
    Any,
    Dict,
    List,
    Union,
)

from pydantic import (
    BaseModel,
    Field,
    RootModel,
    field_validator,
    model_validator,
)

from .mixins import MappingRootMixin
from .objects import Array, Record, Var
from .types import (
    Baudrate,
    Revision,
    VendorProduct,
)


class Device(BaseModel):
    """Device information."""

    vendor: VendorProduct = Field(default_factory=VendorProduct)
    product: VendorProduct = Field(default_factory=VendorProduct)
    revision: Revision = Field(default_factory=lambda: Revision.model_validate("1.0.0"))
    baudrate: Baudrate
    node_id: int = Field(1, ge=1, le=127)

    @field_validator("baudrate", mode="before")
    @classmethod
    def convert_baudrate(cls, v: Any) -> Baudrate:
        """Convert various formats to Baudrate."""
        if isinstance(v, dict):
            # {125: True, 250: None, 300: False}
            return Baudrate(k for k, val in v.items() if val is not False)
        elif isinstance(v, (list, set)):
            return Baudrate(v)
        raise TypeError("Invalid baudrate format: expected dict, list, or set.")


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
            if "type" not in obj:
                if "length" in obj or "data" in obj:
                    obj["type"] = "array"
                elif "record" in obj:
                    obj["type"] = "record"
                else:
                    obj["type"] = "var"

        return v


class SchemaConfig(BaseModel):
    """Configuration schema for the generator."""

    device: Device
    profiles: List[Annotated[int, Field(ge=300, le=500)]]
    objects: Objects


def Config(config_file):
    return SchemaConfig.model_validate(config_file)
