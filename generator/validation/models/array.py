"""Model for CANopen objects with common attributes and validation."""

from typing import (
    Annotated,
    Any,
    ClassVar,
    List,
    Literal,
    Optional,
    Union,
)

from pydantic import BaseModel, Field, field_validator, model_validator

from .enum import Enum, EnumProfile
from .limits import Limits
from .mixins import AccessorMixin, UnitMixin
from .object_common import HeaderCommon, VarCommon


class ArrayEntry(AccessorMixin, UnitMixin, HeaderCommon, BaseModel):
    """Array entry object for storing subindex data."""

    limits: Limits = Limits()
    enum: Optional[Enum] = None
    default: Union[int, float] = 0


class InferArrayLengthMixin:
    """Mixin to infer array length from data."""

    length: Optional[int]
    array: list

    ARRAY_SIZE_ENTRY_NAME: ClassVar[str] = "Number of array entries"

    @model_validator(mode="after")
    def _infer_length_from_data(self) -> Any:
        entry_count = len(self.array)
        if self.length is None:
            self.length = entry_count
        elif self.length < entry_count:
            raise ValueError(
                f"Inconsistent array length: length={self.length} but data has {entry_count} entries."
            )

        if entry_count == 0 or not self._has_subindex_0():
            entry = self._make_subindex_0(self.length)
            self.array.insert(0, entry)

        return self

    def _has_subindex_0(self) -> bool:
        if not self.array:
            return False
        maybe = self.array[0]
        if isinstance(maybe, dict):
            return maybe.get("name") == self.ARRAY_SIZE_ENTRY_NAME
        elif hasattr(maybe, "name"):
            return maybe.name == self.ARRAY_SIZE_ENTRY_NAME
        return False

    def _make_subindex_0(self, value: int) -> Any:
        """Create the subindex 0 structure (as dict or model depending on context)."""

        return ArrayEntry(
            name=self.ARRAY_SIZE_ENTRY_NAME,
            #datatype="uint8",
            access="r",
            default=value,
        )


class BaseArray(HeaderCommon, VarCommon, InferArrayLengthMixin):
    """Base class for array objects."""

    type: Literal["array"] = "array"
    length: Optional[Annotated[int, Field(ge=0, le=255)]] = None


class Array(BaseArray):
    """Array object for storing subindex data."""

    array: List[ArrayEntry] = []

    @field_validator("array", mode="before")
    @classmethod
    def ensure_array_entry(cls, v, info):
        return cls._ensure_array_entry(v, info, model_cls=ArrayEntry)

    @classmethod
    def _ensure_array_entry(cls, v, info, model_cls):
        """Ensure that the array is a list of ArrayEntry objects."""
        if v is None:
            return []
        if not isinstance(v, list):
            raise TypeError(f"array must be a list, got {type(v)}")

        result = []
        array_name = info.data.get("name", "Array")

        for i, item in enumerate(v, start=1):
            if isinstance(item, model_cls):
                result.append(item)
            else:
                # Si item est un dict, vérifier si 'name' est présent
                item_data = dict(item)  # safe copy
                if "name" not in item_data:
                    item_data["name"] = f"{array_name}_{i}"
                result.append(model_cls(**item_data))

        return result


class ArrayEntryProfile(ArrayEntry):
    """Array entry profile with additional information."""

    enum: Optional[Union[Enum, EnumProfile]] = None

class ArrayProfile(Array):
    """Array profile with additional information."""

    array: List[ArrayEntryProfile] = []

    @field_validator("array", mode="before")
    @classmethod
    def ensure_array_entry(cls, v, info):
        return cls._ensure_array_entry(v, info, model_cls=ArrayEntryProfile)
