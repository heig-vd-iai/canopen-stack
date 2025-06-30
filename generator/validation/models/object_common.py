from typing import Annotated, List, Literal, Optional, Union

from pydantic import (
    BaseModel,
    Field,
    StringConstraints,
    model_validator,
)

from .bitfields import Bitfield
from .datatype import Datatype
from .enum import Enum, EnumProfile
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
    default: Union[int, float, str] = 0
    bitfield: Optional[Bitfield] = None

    @model_validator(mode="after")
    def check_default_type(self) -> "VarCommon":
        """Validate the type of the default value based on the datatype."""
        dt_str = str(self.datatype).lower()

        if dt_str.startswith(("int", "uint")):
            if not isinstance(self.default, int):
                raise ValueError(
                    f"default should be of type int when datatype is '{self.datatype}', "
                    f"got {type(self.default).__name__}"
                )
        elif "float" in dt_str:
            if not isinstance(self.default, float):
                if self.default == 0:
                    self.default = 0.0
                else:
                    raise ValueError(
                        f"default should be of type float when datatype is '{self.datatype}', "
                        f"got {type(self.default).__name__}"
                    )
        elif "string" in dt_str:
            if not isinstance(self.default, str):
                raise ValueError(
                    f"default should be of type str when datatype is '{self.datatype}', "
                    f"got {type(self.default).__name__}"
                )
        # If datatype doesn't match any of the above, optionally ignore or raise
        else:
            pass  # Or raise an error if datatype must be known

        return self


class HeaderCommonProfile(HeaderCommon):
    pass


class VarCommonProfile(VarCommon):
    """Variable profile with additional information."""

    enum: Optional[EnumProfile] = None
    category: Literal["conditional", "optional", "mandatory"] = Field(
        default="optional"
    )
    mandatory_conditions: List[str] = Field(default_factory=list)
