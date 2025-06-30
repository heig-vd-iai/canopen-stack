"""Model for CANopen objects with common attributes and validation."""

from typing import (
    Annotated,
    List,
    Literal,
    Optional,
    Union,
)

from pydantic import (
    BaseModel,
    Field,
)

from .enum import Enum
from .mixins import AccessorMixin, InferArrayLengthMixin, UnitMixin
from .object_common import HeaderCommon, VarCommon
from .types import Limits


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
