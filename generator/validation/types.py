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
    Field,
    PrivateAttr,
    field_validator,
    model_validator,
)
from pydantic_core import core_schema

from .registry import DATATYPES
from .validators import validate_identifier


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


class Access:
    """Object access rights."""

    def __init__(self, read=False, write=False):
        self.read = bool(read)
        self.write = bool(write)

    def __str__(self):
        if self.read and self.write:
            return "rw"
        if self.read:
            return "r"
        if self.write:
            return "w"
        return ""

    def __repr__(self):
        if self.read and self.write:
            return "Access(Read Write)"
        if self.read:
            return "Access(Read Only)"
        if self.write:
            return "Access(Write Only)"
        return "Access(None)"

    def to_dict(self):
        """Convert Access object to a dictionary."""
        return {"read": self.read, "write": self.write}

    def to_eds(self):
        """EDS representation is a bit weird:

        r -> ro
        w -> wo
        rw -> rw
        """
        if self.read and self.write:
            return "rw"
        if self.read:
            return "ro"
        if self.write:
            return "wo"
        return ""

    @classmethod
    def parse(cls, value):
        """Parse various formats into an Access object."""
        if isinstance(value, Access):
            return value
        if isinstance(value, str):
            return cls(read="r" in value, write="w" in value)
        if isinstance(value, dict):
            return cls(read=value.get("read", False), write=value.get("write", False))
        raise TypeError(f"Invalid access value: {value}")

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        return core_schema.no_info_after_validator_function(
            cls.parse,
            core_schema.str_schema(),
            serialization=core_schema.plain_serializer_function_ser_schema(str),
        )


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
    def __get_pydantic_core_schema__(
        cls, source_type, handler
    ):  # pylint: disable=arguments-differ
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


class BitfieldEntry(BaseModel):
    """Bitfield entry for storing bit ranges and their names."""

    name: str
    values: Dict[int, str] = {}
    _width: Optional[int] = PrivateAttr(default=None)

    @field_validator("values")
    @classmethod
    def check_values_fit_width(cls, values: Dict[int, str], info) -> Dict[int, str]:
        """Check if values fit within the specified width."""
        # We will inject the field width externally since this depends on the key
        width = getattr(info.data, "_width", None)
        if width is not None:
            max_val = (1 << width) - 1
            for k in values:
                if k > max_val:
                    raise ValueError(
                        f"Value {k} exceeds width limit {width} (max={max_val})"
                    )
        return values


class Bitfield(Dict[Tuple[int, int], BitfieldEntry]):
    """Bitfield representation for storing bit ranges and their names."""

    _width: Optional[int] = PrivateAttr(default=None)

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        def validate(v: Any) -> "Bitfield":
            if not isinstance(v, dict):
                raise TypeError("Bitfield must be a dict")
            result = {}
            for key, val in v.items():
                # Normalize key
                if isinstance(key, int):
                    start, end = key, key
                elif isinstance(key, str) and re.match(r"^\d+\.\.\d+$", key):
                    start, end = map(int, key.split(".."))
                    if start < end:
                        raise ValueError(
                            f"Invalid bit range '{key}': start must be >= end"
                        )
                else:
                    raise ValueError(f"Invalid bitfield key: {key}")

                width = start - end + 1

                # Normalize value
                if isinstance(val, str):
                    entry = BitfieldEntry(name=val, values={})
                elif isinstance(val, dict):
                    entry = BitfieldEntry(**val)
                else:
                    raise TypeError(
                        f"Invalid bitfield entry value for key {key}: {val}"
                    )

                # Inject width dynamically to perform value checks
                entry._width = width
                entry = BitfieldEntry.model_validate(entry.model_dump())  # re-validate
                result[(start, end)] = entry
            return cls(result)

        return core_schema.no_info_after_validator_function(
            validate,
            core_schema.dict_schema(
                keys_schema=core_schema.any_schema(),
                values_schema=core_schema.any_schema(),
            ),
            serialization=core_schema.plain_serializer_function_ser_schema(
                lambda v: {
                    f"{k[0] if k[0] == k[1] else f'{k[0]}..{k[1]}'}": e.model_dump()
                    for k, e in v.items()
                }
            ),
        )


class Enum(BaseModel):
    """Enum data for variable types."""

    class_: str = Field(..., alias="class")
    data: Dict[str, int]

    @model_validator(mode="before")
    @classmethod
    def validate_enum_data(cls, values):
        """Validate enum data."""
        if "data" not in values:
            raise ValueError("Enum field data is required.")
        if not isinstance(values.get("data"), dict):
            raise ValueError("Enum data must be a dictionary.")
        if len(set(values["data"].values())) != len(values["data"]):
            raise ValueError("Enum values must be unique.")
        return values

    @field_validator("data", mode="before")
    @classmethod
    def validate_enum_keys(cls, v):
        """Validate that all keys in the enum data dictionary are valid identifiers."""
        return {validate_identifier(k): val for k, val in v.items()}


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
