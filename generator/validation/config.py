"""Validation schema for the config.yaml file."""

from typing import Annotated, Any, Dict, List, Tuple, Union

from pydantic import BaseModel, Field, RootModel, ValidationError, model_validator
from pydantic_core import ErrorDetails

from . import Array, Device, Record, Var
from .helpers import infer_object_type
from .models.mixins import MappingRootMixin
from ..loaders.profiles import ProfileLoader
from .profile import ObjectTypeProfile

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


def validate_against_profiles(config: SchemaConfig) -> None:
    """Validate the configuration against the specified profiles."""
    if not config.profiles:
        return

    profiles = ProfileLoader()
    errors = profiles.load_all(use_cache=True)

    for profile in config.profiles:
        if profile not in profiles.profiles:
            raise ValueError(f"Profile {profile} not found in loaded profiles")

def validate_against_profile_object(config_object: ObjectType, profile_object: ObjectTypeProfile):
    """Validate a single object against its profile."""
    errors = []

    # Adds description if missing
    if not config_object.description:
        config_object.description = profile_object.description

    # Adds datatype if missing
    if not config_object.datatype:
        config_object.datatype = profile_object.datatype

    # Adds default value if missing
    if not config_object.default:
        config_object.default = profile_object.default


    # Object must be the same type as the profile
    if config_object.type != profile_object.type:
        errors.append(
            f"Type mismatch: {config_object.type} vs {profile_object.type}"
        )

    # Object must be the same datatype as the profile
    if config_object.datatype != profile_object.datatype:
        errors.append(
            f"Datatype mismatch: {config_object.datatype} vs {profile_object.datatype}"
        )

    # Object must have the same access as the profile
    if config_object.access != profile_object.access:
        errors.append(
            f"Access mismatch: {config_object.access} vs {profile_object.access}"
        )

    for field, value in config_object.model_dump().items():
        if field not in profile_object.model_fields:
            raise ValueError(f"Field {field} not found in profile object")

        profile_field = profile_object.model_fields[field]
        if not isinstance(value, profile_field.annotation):
            raise ValueError(
                f"Field {field} type mismatch: {type(value)} vs {profile_field.annotation}"
            )

def Config(config_data) -> Tuple[Union[SchemaConfig, None], List[ErrorDetails]]:
    """Validate the configuration data against the SchemaConfig model."""
    config = None
    errors = []
    try:
        config = SchemaConfig.model_validate(config_data)
    except ValidationError as e:
        errors = e.errors()
        return None, errors

    if len(config.profiles):
        validate_against_profiles(config)

    return config, errors
