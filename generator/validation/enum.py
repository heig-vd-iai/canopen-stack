"""Model for enum types and entries."""

from typing import Dict, List, Optional, Tuple

from pydantic import BaseModel, ConfigDict, Field, field_validator, model_validator

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

    class_: str = Field(..., alias="class")
    data: Dict[str, EnumEntry]

    @model_validator(mode="before")
    @classmethod
    def validate_enum_data(cls, values):
        """Validate the enum data structure."""
        data = values.get("data")
        if not isinstance(data, dict):
            raise ValueError("Enum data must be a dictionary.")

        seen_values = set()
        for entry in data.values():
            if isinstance(entry, dict):
                val = entry.get("value")
            elif isinstance(entry, EnumEntry):
                val = entry.value
            else:
                raise ValueError(f"Invalid enum entry: {entry}")
            if val in seen_values:
                raise ValueError("Enum values must be unique.")
            seen_values.add(val)

        return values

    @field_validator("data", mode="before")
    @classmethod
    def validate_enum_keys(cls, v):
        """Ensure all dictionary keys are valid identifiers."""
        return {validate_identifier(k): val for k, val in v.items()}

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
