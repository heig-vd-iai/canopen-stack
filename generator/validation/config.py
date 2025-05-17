"""Validation schema for the config.yaml file."""

import warnings
from typing import (
    Annotated,
    Any,
    ClassVar,
    Dict,
    List,
    Literal,
    Optional,
    Union,
)

from pydantic import (
    BaseModel,
    Field,
    RootModel,
    StringConstraints,
    field_validator,
    model_validator,
)
from pydantic_core import core_schema

from .mixins import AccessorMixin, InferArrayLengthMixin, MappingRootMixin, UnitMixin
from .types import Baudrate, Bitfield, Datatype, Enum, Limits, Markdown, Revision

STRICT_VALIDATION = False


class HeaderCommon(BaseModel):
    """Common attributes for header objects."""

    name: str
    remote: Optional[str] = Field(default="local", min_length=1)
    path: Annotated[
        Optional[str],
        StringConstraints(
            pattern=r"^([a-zA-Z][a-zA-Z0-9]*)(?:\.[a-zA-Z][a-zA-Z0-9]*)*$"
        ),
    ] = ""
    description: Markdown = Field(default=Markdown(""))
    inherit: Optional[int] = None


class VarCommon(AccessorMixin, UnitMixin, BaseModel):
    """Common attributes for variable objects."""

    datatype: Datatype
    limits: Limits = Limits()
    pdo: bool = False
    enum: Optional[Enum] = None
    default: Union[int, float] = 0
    bitfield: Optional[Bitfield] = None


class RecordEntry(VarCommon, HeaderCommon):
    """Record entry object for storing subindex data."""

    class Config:
        extra = "forbid"


class Var(HeaderCommon, VarCommon):
    """Variable object for storing subindex data."""

    type: Literal["var"] = "var"

    class Config:
        extra = "forbid"


class ArrayEntry(AccessorMixin, UnitMixin, BaseModel):
    """Array entry object for storing subindex data."""

    limits: Limits = Limits()
    enum: Optional[Enum] = None
    default: Union[int, float] = 0


class BaseArray(HeaderCommon, VarCommon, InferArrayLengthMixin):
    """Base class for array objects."""

    type: Literal["array"] = "array"
    length: Optional[Annotated[int, Field(ge=0, le=255)]] = None


class Array(BaseArray):
    """Array object for storing subindex data."""

    data: List[ArrayEntry] = []


class Record(HeaderCommon):
    """Record object for storing subindex data."""

    type: Literal["record"] = "record"
    record: List[RecordEntry]

    SIZE_ENTRY_NAME: ClassVar[str] = "Number of records"

    @model_validator(mode="after")
    def validate_and_inject_size_entry(self):
        """Validate the length of the record and inject subindex 0 if needed."""

        if len(self.record) > 255:
            raise ValueError("Subindex length must be less than 256.")

        # Check if subindex 0 is present
        if not self.record or self.record[0].name != self.SIZE_ENTRY_NAME:
            size_entry = RecordEntry(
                name=self.SIZE_ENTRY_NAME,
                datatype="uint8",
                access="r",
                default=len(self.record),
            )
            self.record.insert(0, size_entry)

        else:
            # Update default if already present
            self.record[0].default = len(self.record)

        return self


class Domain(HeaderCommon):
    """Domain object for storing domain-specific data."""

    type: Literal["domain"] = "domain"
    domain: Dict[str, Annotated[int, Field(ge=1)]]



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
