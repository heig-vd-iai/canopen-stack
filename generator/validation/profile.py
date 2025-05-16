"""Validation schema for the CiA profiles definitions."""

from typing import Annotated, Any, Dict, List, Literal, Optional, Tuple, Union

from pydantic import (
    BaseModel,
    Field,
    RootModel,
    field_validator,
    model_validator,
)

from .config import (
    ArrayEntry,
    BaseArray,
    EnumData,
    MappingRootMixin,
    Markdown,
    Record,
    RecordEntry,
    Var,
)
from .helpers import validate_identifier


class EnumDataProfile(EnumData):
    """Enum data with additional profile information."""

    override_ranges: List[Tuple[int, int]] = Field(default_factory=list)


class ProfileExtra(BaseModel):
    """Base class for profile extra information."""

    category: Literal["conditional", "optional", "mandatory"] = Field(
        default="optional"
    )
    mandatory_conditions: List[str] = Field(default_factory=list)


class VarProfile(ProfileExtra, Var):
    """Variable profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class ArrayEntryProfile(ArrayEntry):
    """Array entry profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class ArrayProfile(BaseArray):
    """Array profile with additional information."""

    data: List[ArrayEntryProfile] = []


class RecordEntryProfile(ProfileExtra, RecordEntry):
    """Record entry profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class RecordProfile(ProfileExtra, Record):
    """Record profile with additional information."""

    record: List[RecordEntryProfile] = []


ObjectTypeProfile = Annotated[
    Union[VarProfile, ArrayProfile, RecordProfile], Field(discriminator="type")
]


class ObjectsProfile(
    MappingRootMixin[ObjectTypeProfile], RootModel[Dict[int, ObjectTypeProfile]]
):
    """Profile for objects with inheritance and type resolution."""

    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        """Resolve inheritance and infer types for objects."""
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        # Étape 1 : Résolution de l’héritage
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


class Profile(BaseModel):
    """Profile for a specific CiA profile."""

    name: str
    description: Markdown = Field(default=Markdown(""))
    standards: List[str] = []
    features: Dict[str, str] = {}
    objects: ObjectsProfile

    @field_validator("features", mode="before")
    @classmethod
    def validate_enum_keys(cls, v):
        """Validate that all keys in the features dictionary are valid identifiers."""
        return {validate_identifier(k): val for k, val in v.items()}


class SchemaProfile(BaseModel):
    """Schema for the CiA profiles."""

    profiles: Dict[Annotated[int, Field(ge=300, le=500)], Profile]
