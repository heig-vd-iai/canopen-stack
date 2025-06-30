from collections.abc import Mapping
from typing import Any, ClassVar, Generic, Optional, TypeVar, Union

from pydantic import (
    BaseModel,
    Field,
    field_validator,
    model_validator,
)

from . import Access
from .units import validate_unit_string

T = TypeVar("T")


class AccessorMixin(BaseModel):
    """Mixin for accessor validation."""

    access: Union[Access, str, dict[str, Any]] = Field(default_factory=Access)
    get: Optional[str] = None
    set: Optional[str] = None

    @model_validator(mode="after")
    def _auto_infer_access(self) -> "AccessorMixin":
        """
        If 'access' is not explicitly set (rw/ro/wo),
        infer from presence of get/set.
        """
        # Force conversion if still str or dict
        if not isinstance(self.access, Access):
            self.access = Access.model_validate(self.access)

        # Do not override explicit access
        if not self.access.read and not self.access.write:
            object.__setattr__(self.access, "read", self.get is not None)
            object.__setattr__(self.access, "write", self.set is not None)

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
        if not self.data:
            return False
        maybe = self.data[0]
        if isinstance(maybe, dict):
            return maybe.get("name") == self.ARRAY_SIZE_ENTRY_NAME
        elif hasattr(maybe, "name"):
            return maybe.name == self.ARRAY_SIZE_ENTRY_NAME
        return False

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

    root: dict[Any, T]

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
