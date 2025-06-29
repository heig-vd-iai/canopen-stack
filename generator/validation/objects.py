"""Model for CANopen objects with common attributes and validation."""

from typing import (
    Annotated,
    ClassVar,
    Dict,
    List,
    Literal,
    Optional,
    Union,
)

from pydantic import (
    BaseModel,
    Field,
    StringConstraints,
    model_validator,
)

from . import Access, Bitfield, Datatype, Enum, Limits, Markdown
from .mixins import AccessorMixin, InferArrayLengthMixin, UnitMixin


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

    datatype: Datatype
    limits: Limits = Limits()
    pdo: bool = False
    enum: Optional[Enum] = None
    default: Union[int, float] = 0
    bitfield: Optional[Bitfield] = None


class RecordEntry(VarCommon, HeaderCommon):
    """Record entry object for storing subindex data."""

    class Config:
        """Configuration for RecordEntry model."""

        extra = "forbid"


class Var(HeaderCommon, VarCommon):
    """Variable object for storing subindex data."""

    type: Literal["var"] = "var"

    class Config:
        """Configuration for Var model."""

        extra = "forbid"


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


class Record(HeaderCommon):
    """Record object for storing subindex data."""

    type: Literal["record"] = "record"
    record: List[RecordEntry]

    SIZE_ENTRY_NAME: ClassVar[str] = "Number of records"

    @model_validator(mode="after")
    def validate_and_inject_size_entry(self):
        """Validate the length of the record and inject subindex 0 if needed."""

        if len(self.record) > 255:
            raise ValueError("Subindex length must be less than 256.")

        # Check if subindex 0 is present
        if not self.record or self.record[0].name != self.SIZE_ENTRY_NAME:
            size_entry = RecordEntry(
                name=self.SIZE_ENTRY_NAME,
                datatype=Datatype.from_name("uint8"),
                access=Access.model_validate("r"),
                default=len(self.record),
            )
            self.record.insert(0, size_entry)

        else:
            # Update default if already present
            self.record[0].default = len(self.record)

        return self


class Domain(HeaderCommon):
    """Domain object for storing domain-specific data."""

    type: Literal["domain"] = "domain"
    domain: Dict[str, Annotated[int, Field(ge=1)]]
