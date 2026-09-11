"""Fields shared by every object dictionary entry."""

import re
from typing import Annotated, ClassVar, List, Literal, Optional, Union

from pydantic import (
    BaseModel,
    ConfigDict,
    Field,
    StringConstraints,
    model_validator,
)

from .access import Access
from .bitfields import Bitfield
from .datatype import Datatype
from .enum import Enum, EnumProfile
from .limits import Limits
from .markdown import Markdown
from .mixins import AccessorMixin, UnitMixin

NODE_ID_DEFAULT = re.compile(r"^\$NODEID\+(0x[0-9A-Fa-f]+|\d+)$")

Category = Literal["conditional", "optional", "mandatory"]


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
    module: Optional[str] = None
    category: Category = "optional"

    @property
    def is_remote(self) -> bool:
        return self.remote not in (None, "local")


class HeaderCommonProfile(HeaderCommon):
    """Common attributes for header objects defined in a CiA profile."""


class Sub0(BaseModel):
    """Sub-index 0 of an array or a record: the highest sub-index supported."""

    model_config = ConfigDict(extra="forbid")

    name: str = "Highest sub-index supported"
    access: Access = Field(default_factory=lambda: Access(read=True))
    default: Optional[int] = None


class VarCommon(AccessorMixin, UnitMixin, BaseModel):
    """Common attributes for variable objects."""

    datatype: Union[Datatype, str]
    limits: Limits = Limits()
    unit: Optional[str] = None
    pdo: bool = False
    enum: Optional[Enum] = None
    default: Union[int, float, str] = 0
    bitfield: Optional[Bitfield] = None
    size: Optional[Annotated[int, Field(ge=1)]] = None

    REQUIRES_SIZE: ClassVar[bool] = True

    @model_validator(mode="after")
    def check_default_type(self) -> "VarCommon":
        """Validate the type of the default value based on the datatype."""
        dt_str = str(self.datatype_name).lower()

        if dt_str.startswith(("int", "uint")):
            if isinstance(self.default, str) and NODE_ID_DEFAULT.match(self.default):
                return self
            if not isinstance(self.default, int):
                raise ValueError(
                    f"default should be of type int when datatype is '{dt_str}', "
                    f"got {type(self.default).__name__}"
                )
        elif "float" in dt_str:
            if not isinstance(self.default, (int, float)):
                raise ValueError(
                    f"default should be of type float when datatype is '{dt_str}', "
                    f"got {type(self.default).__name__}"
                )
        elif "string" in dt_str:
            if not isinstance(self.default, str):
                if self.default == 0:
                    self.default = ""
                else:
                    raise ValueError(
                        f"default should be of type str when datatype is '{dt_str}', "
                        f"got {type(self.default).__name__}"
                    )
            if self.size is None and self.REQUIRES_SIZE:
                raise ValueError("a string needs a 'size' (capacity in bytes)")

        return self

    @model_validator(mode="after")
    def check_limits_are_paired(self) -> "VarCommon":
        if (self.limits.min is None) != (self.limits.max is None):
            raise ValueError("limits need both 'min' and 'max'")
        return self

    @property
    def datatype_name(self) -> str:
        if isinstance(self.datatype, Datatype):
            return self.datatype.name
        return str(self.datatype)

    @property
    def limited(self) -> bool:
        return self.limits.min is not None


class VarCommonProfile(VarCommon):
    """Variable profile with additional information.

    A profile never fixes the capacity of a string: the device does.
    """

    enum: Optional[Union[Enum, EnumProfile]] = None
    mandatory_conditions: List[str] = Field(default_factory=list)

    REQUIRES_SIZE: ClassVar[bool] = False
