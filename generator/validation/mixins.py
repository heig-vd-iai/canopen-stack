from collections.abc import Mapping
from typing import Any, ClassVar, Generic, Optional, TypeVar

from pydantic import (
    BaseModel,
    Field,
    field_validator,
    model_validator,
)

from .types import Access
from .validators import validate_unit_string

T = TypeVar("T")


class AccessorMixin(BaseModel):
    """Mixin for accessor validation."""

    access: Access = Field(default_factory=Access)
    get: Optional[str] = None
    set: Optional[str] = None

    @model_validator(mode="after")
    def update_access(self):
        """Update access rights based on the presence of get/set methods."""
        inferred = False

        if not self.access.read and not self.access.write:
            read = self.get is not None
            write = self.set is not None
            self.access = Access(read=read, write=write)
            inferred = True

        if not inferred:
            if self.get is not None and not self.access.read:
                raise ValueError(
                    "Access mismatch: 'get' defined but 'read' not allowed in 'access'."
                )
            if self.set is not None and not self.access.write:
                raise ValueError(
                    "Access mismatch: 'set' defined but 'write' not allowed in 'access'."
                )

        return self


class InferArrayLengthMixin:
    """Mixin to infer array length from data."""

    length: Optional[int]
    data: list

    ARRAY_SIZE_ENTRY_NAME: ClassVar[str] = "Number of array entries"

    @model_validator(mode="after")
    def _infer_length_from_data(self) -> Any:
        entry_count = len(self.data)
        if self.length is None:
            self.length = entry_count
        elif self.length < entry_count:
            raise ValueError(
                f"Inconsistent array length: length={self.length} but data has {entry_count} entries."
            )

        if entry_count == 0 or not self._has_subindex_0():
            entry = self._make_subindex_0(self.length)
            self.data.insert(0, entry)

        return self

    def _has_subindex_0(self) -> bool:
        """Heuristic: entry 0 must be the special descriptor."""
        if not self.data:
            return False
        maybe = self.data[0]
        return (
            isinstance(maybe, dict) and maybe.get("name") == self.ARRAY_SIZE_ENTRY_NAME
        ) or (hasattr(maybe, "name") and maybe.name == self.ARRAY_SIZE_ENTRY_NAME)

    def _make_subindex_0(self, value: int) -> Any:
        """Create the subindex 0 structure (as dict or model depending on context)."""
        return {
            "name": self.ARRAY_SIZE_ENTRY_NAME,
            "datatype": "uint8",
            "type": "var",
            "access": "r",
            "default": value,
        }


class MappingRootMixin(Mapping, Generic[T]):
    """Mixin for mapping-like behavior in Pydantic models."""

    def __getitem__(self, key):
        return self.root[key]

    def __iter__(self):
        return iter(self.root)

    def __len__(self):
        return len(self.root)

    def keys(self):
        return self.root.keys()

    def values(self):
        return self.root.values()

    def items(self):
        return self.root.items()


class UnitMixin(BaseModel):
    """Mixin for unit validation."""

    unit: Optional[str] = Field(default="", validate_default=True)
    scale: Optional[float] = None

    @field_validator("unit")
    @classmethod
    def validate_unit(cls, v):
        """Validate the unit string."""
        return validate_unit_string(v)
