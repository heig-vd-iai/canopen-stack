"""Model for CANopen objects with common attributes and validation."""

from typing import (
    Annotated,
    List,
    Literal,
    Optional,
    Union,
)

from pydantic import BaseModel, Field, field_validator

from .enum import Enum
from .mixins import AccessorMixin, InferArrayLengthMixin, UnitMixin
from .object_common import HeaderCommon, VarCommon
from .types import Limits


class ArrayEntry(AccessorMixin, UnitMixin, HeaderCommon, BaseModel):
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

    @field_validator("data", mode="before")
    @classmethod
    def ensure_array_entry(cls, v, info):
        if v is None:
            return []
        if not isinstance(v, list):
            raise TypeError(f"data must be a list, got {type(v)}")

        result = []
        array_name = info.data.get("name", "Array")

        for i, item in enumerate(v, start=1):
            if isinstance(item, ArrayEntry):
                result.append(item)
            else:
                # Si item est un dict, vérifier si 'name' est présent
                item_data = dict(item)  # safe copy
                if "name" not in item_data:
                    item_data["name"] = f"{array_name}_{i}"
                result.append(ArrayEntry(**item_data))

        return result
