"""Types definitions according to the CiA 301 specification."""

import re
from typing import (
    Any,
    ClassVar,
    Dict,
    Optional,
    Tuple,
    Union,
    cast,
)

import mistune
from pydantic import (
    BaseModel,
    PrivateAttr,
    field_validator,
    model_validator,
)
from pydantic_core import core_schema

from .registry import DATATYPES


class ObjectId(int):
    """Object identifier for CANopen objects."""

    def __new__(cls, index, subindex):
        return super().__new__(cls, (index << 8) | subindex)

    def __init__(self, index, subindex):
        self.index = index
        self.subindex = subindex
        self.eds_name = f"{index:04x}sub{subindex:x}"

    def __str__(self):
        return f"0x{self.index:04x}_{self.subindex:02d}"

    def __repr__(self):
        return f"ObjectCode(index={self.index:04x}, subindex={self.subindex})"

    def __getitem__(self, item):
        if item == "index":
            return self.index
        elif item == "subindex":
            return self.subindex
        else:
            raise KeyError(f"Invalid key: {item}")


class Markdown(str):
    """String subclass representing Markdown with validation and conversion."""

    def to_html(self) -> str:
        """Convert Markdown to HTML using mistune."""
        renderer = mistune.HTMLRenderer()
        parser = mistune.create_markdown(renderer=renderer)
        return cast(str, parser(self))

    def __repr__(self):
        preview = str(self).replace("\n", " ")[:40]
        return f"Markdown({preview!r}...)"

    @classmethod
    def validate(cls, value):
        """Validate and parse the input value as Markdown."""
        if not isinstance(value, str):
            raise TypeError("Markdown must be a string.")
        try:
            mistune.create_markdown()(value)  # parsing attempt
        except Exception as e:
            raise ValueError(f"Invalid Markdown: {e}") from e
        return cls(value)

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        return core_schema.no_info_after_validator_function(
            cls.validate,
            core_schema.str_schema(),
            serialization=core_schema.plain_serializer_function_ser_schema(str),
        )


class Access(BaseModel):
    """Object access rights (CANopen-style: r/w/rw)."""

    read: bool = False
    write: bool = False

    def __str__(self):
        return (
            "rw"
            if self.read and self.write
            else "r"
            if self.read
            else "w"
            if self.write
            else ""
        )

    def __repr__(self):
        return f"Access(read={self.read}, write={self.write})"

    def to_eds(self) -> str:
        return (
            "rw"
            if self.read and self.write
            else "ro"
            if self.read
            else "wo"
            if self.write
            else ""
        )

    @model_validator(mode="before")
    @classmethod
    def _coerce(cls, v: Any) -> Any:
        if isinstance(v, cls):
            return v
        if isinstance(v, str):
            return {"read": "r" in v, "write": "w" in v}
        if isinstance(v, dict):
            return {
                "read": bool(v.get("read", False)),
                "write": bool(v.get("write", False)),
            }
        raise TypeError(f"Cannot parse Access from {v!r}")


class Limits(BaseModel):
    """Limits for variable types."""

    min: Optional[Union[int, float]] = None
    max: Optional[Union[int, float]] = None


class Datatype(BaseModel):
    """CiA 301 data types."""

    name: str
    code: int
    ctype: str
    field: str

    @classmethod
    def from_name(cls, name: str) -> "Datatype":
        """Create a Datatype instance from a name."""
        try:
            dt = DATATYPES[name]
        except KeyError as e:
            raise ValueError(f"Invalid CiA datatype: {name}") from e
        return cls(name=dt.name, code=dt.code, ctype=dt.ctype, field=dt.field)

    @classmethod
    def __get_pydantic_core_schema__(cls, source_type, handler):  # pylint: disable=arguments-differ
        """Get the Pydantic core schema for Datatype."""
        schema = handler(source_type)

        def preprocess(value: Any) -> Any:
            if isinstance(value, cls):
                return value
            if isinstance(value, str):
                return cls.from_name(value)
            return value  # dict ou autres : laisser le handler valider

        return core_schema.no_info_before_validator_function(
            preprocess,
            schema,
            serialization=core_schema.plain_serializer_function_ser_schema(
                cls._serialize, return_schema=core_schema.str_schema()
            ),
        )

    @classmethod
    def _serialize(cls, value: "Datatype") -> str:
        return value.name


class ObjectType:
    """Class to represent object values."""

    OBJECTS_TYPE: ClassVar[Dict[str, int]] = {
        "null": 0x0,
        "domain": 0x2,
        "deftype": 0x5,
        "defstruct": 0x6,
        "var": 0x7,
        "array": 0x8,
        "record": 0x9,
    }

    def __init__(self, name):
        if name not in self.OBJECTS_TYPE:
            raise ValueError(f"Invalid object type name: {name}")
        self.name = name
        self.code = self.OBJECTS_TYPE[name]


objects_ranges = {
    (0x0001, 0x001F): "Static data types",
    (0x0020, 0x003F): "Complex data types",
    (0x0040, 0x005F): "Manufacturer-specific complex data types",
    (0x0060, 0x025F): "Device profile specific data types",
    # Reserved range
    (0x1000, 0x1FFF): "Communication profile area",
    (0x2000, 0x5FFF): "Manufacturer-specific profile area",
    (0x6000, 0x67FF): "Standardized profile 1st logical device",
    (0x6800, 0x6FFF): "Standardized profile 2nd logical device",
    (0x7000, 0x77FF): "Standardized profile 3rd logical device",
    (0x7800, 0x7FFF): "Standardized profile 4th logical device",
    (0x8000, 0x87FF): "Standardized profile 5th logical device",
    (0x8800, 0x8FFF): "Standardized profile 6th logical device",
    (0x9000, 0x97FF): "Standardized profile 7th logical device",
    (0x9800, 0x9FFF): "Standardized profile 8th logical device",
    (0xA000, 0xAFFF): "Standardized network variable area",
    (0xB000, 0xBFFF): "Standardized system variable area",
    # Reserved range
}
