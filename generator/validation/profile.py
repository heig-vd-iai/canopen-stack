"""Validation schema for the CiA profiles definitions."""

from typing import Annotated, Any, Dict, List, Union

from pydantic import (
    BaseModel,
    Field,
    RootModel,
    field_validator,
    model_validator,
)

from .helpers import infer_object_type
from .models.array import ArrayProfile
from .models.markdown import Markdown
from .models.mixins import MappingRootMixin
from .models.record import RecordProfile
from .models.validators import validate_identifier
from .models.var import VarProfile

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
            obj.setdefault("type", infer_object_type(obj))

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


class Profiles(BaseModel):
    """Schema for the CiA profiles."""

    profiles: Dict[Annotated[int, Field(ge=300, le=500)], Profile]
