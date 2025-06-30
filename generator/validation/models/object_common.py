from typing import (
    Annotated,
    Optional,
    Union,
)

from pydantic import (
    BaseModel,
    Field,
    StringConstraints,
)

from .bitfields import Bitfield
from .datatype import Datatype
from .enum import Enum
from .limits import Limits
from .markdown import Markdown
from .mixins import AccessorMixin, UnitMixin


class HeaderCommon(BaseModel):
    """Common attributes for header objects."""

    name: str
    remote: Optional[str] = Field(default="local", min_length=1)
    path: Annotated[
        Optional[str],
        StringConstraints(
            pattern=r"^([a-zA-Z][a-zA-Z0-9]*)(?:\.[a-zA-Z][a-zA-Z0-9]*)*$"
        ),
    ] = ""
    description: Markdown = Field(default=Markdown(""))
    inherit: Optional[int] = None


class VarCommon(AccessorMixin, UnitMixin, BaseModel):
    """Common attributes for variable objects."""

    datatype: Union[Datatype, str]
    limits: Limits = Limits()
    unit: Optional[str] = None
    pdo: bool = False
    enum: Optional[Enum] = None
    default: Union[int, float] = 0
    bitfield: Optional[Bitfield] = None
