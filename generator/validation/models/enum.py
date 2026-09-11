"""Model for enum types and entries."""

from typing import Dict, List, Optional, Tuple

from pydantic import BaseModel, ConfigDict, Field, field_validator

from .validators import validate_identifier


class RangeValidatorMixin:
    """Mixin for validating a list of (int, int) ranges."""

    @classmethod
    def _validate_range_list(
        cls, v: List[Tuple[int, int]], field_name: str
    ) -> List[Tuple[int, int]]:
        if not isinstance(v, list):
            raise ValueError(f"{field_name} must be a list of (int, int) tuples.")
        for item in v:
            if not isinstance(item, (list, tuple)) or len(item) != 2:
                raise ValueError(
                    f"Each entry in {field_name} must be a tuple of two integers. Got: {item}"
                )
            a, b = item
            if not isinstance(a, int) or not isinstance(b, int):
                raise ValueError(
                    f"Elements in {field_name} must be integers. Got: {item}"
                )
            if a > b:
                raise ValueError(
                    f"In {field_name}, the first integer must be <= the second. Got: {item}"
                )
        return v


class EnumEntry(BaseModel):
    """Enum entry for storing enum values and their names."""

    name: str
    description: Optional[str] = None
    value: int


class Enum(BaseModel):
    """Enum data for variable types."""

    typedef: str
    values: Dict[str, EnumEntry]

    @field_validator("values", mode="before")
    @classmethod
    def validate_enum_keys(cls, v):
        if not isinstance(v, dict):
            raise ValueError("Enum values must be a dictionary.")

        new_data = {}
        seen_values = set()

        for k, val in v.items():
            key = validate_identifier(k)

            if isinstance(val, int):
                new_entry = {"name": key, "value": val, "description": None}
            elif isinstance(val, dict):
                new_entry = val
            elif isinstance(val, EnumEntry):
                new_entry = val.model_dump()
            else:
                raise ValueError(f"Invalid enum entry for key {key}: {val}")

            val_value = new_entry["value"]
            if val_value in seen_values:
                raise ValueError(f"Duplicate enum value: {val_value}")
            seen_values.add(val_value)

            new_data[key] = new_entry

        return new_data

    model_config = ConfigDict(populate_by_name=True)


class EnumEntryProfile(EnumEntry):
    """Enum entry with additional profile constraints."""

    mandatory_conditions: List[str] = Field(default_factory=list)


class EnumProfile(Enum, RangeValidatorMixin):
    """Enum type extended with override range support."""

    allow_override: List[Tuple[int, int]]

    @field_validator("allow_override", mode="before")
    @classmethod
    def validate_allow_override(cls, v):
        """Validate the allow_override field."""
        return cls._validate_range_list(v, "allow_override")
