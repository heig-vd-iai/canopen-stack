"""Validation schema for the config.yaml file."""

import re
import warnings
from collections.abc import Mapping
from typing import (
    Annotated,
    Any,
    Dict,
    Generic,
    List,
    Literal,
    Optional,
    Tuple,
    TypeVar,
    Union,
    ClassVar,
)

import mistune
import semver
from pydantic import (
    BaseModel,
    Field,
    PrivateAttr,
    RootModel,
    StringConstraints,
    field_validator,
    model_validator,
)
from pydantic_core import core_schema

from .helpers import validate_identifier, validate_unit_string

STRICT_VALIDATION = False


T = TypeVar("T")


class MappingRootMixin(Mapping, Generic[T]):
    """Mixin for mapping-like behavior in Pydantic models."""

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


class Markdown(str):
    """String subclass representing Markdown with validation and conversion."""

    def to_html(self) -> str:
        """Convert Markdown to HTML using mistune."""
        return mistune.create_markdown()(self)

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


class BitfieldEntry(BaseModel):
    """Bitfield entry for storing bit ranges and their names."""

    name: str
    values: Dict[int, str] = {}

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
                entry._width = width  # not public, internal use
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


class Limits(BaseModel):
    """Limits for variable types."""

    min: Optional[Union[int, float]] = None
    max: Optional[Union[int, float]] = None


class EnumData(BaseModel):
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


class UnitMixin(BaseModel):
    """Mixin for unit validation."""

    unit: Optional[str] = Field(default="", validate_default=True)

    @field_validator("unit")
    @classmethod
    def validate_unit(cls, v):
        """Validate the unit string."""
        return validate_unit_string(v)


class AccessorMixin(BaseModel):
    """Mixin for accessor validation."""

    access: Access = Field(default_factory=Access)
    get: Optional[str] = None
    set: Optional[str] = None

    @model_validator(mode="after")
    def update_access(self):
        """Update access rights based on the presence of get/set methods."""
        inferred = False

        if not self.access.read and not self.access.write:
            read = self.get is not None
            write = self.set is not None
            self.access = Access(read=read, write=write)
            inferred = True

        if not inferred:
            if self.get is not None and not self.access.read:
                raise ValueError(
                    "Access mismatch: 'get' defined but 'read' not allowed in 'access'."
                )
            if self.set is not None and not self.access.write:
                raise ValueError(
                    "Access mismatch: 'set' defined but 'write' not allowed in 'access'."
                )

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
        """Heuristic: entry 0 must be the special descriptor."""
        if not self.data:
            return False
        maybe = self.data[0]
        return (
            isinstance(maybe, dict) and maybe.get("name") == self.ARRAY_SIZE_ENTRY_NAME
        ) or (hasattr(maybe, "name") and maybe.name == self.ARRAY_SIZE_ENTRY_NAME)

    def _make_subindex_0(self, value: int) -> Any:
        """Create the subindex 0 structure (as dict or model depending on context)."""
        return {
            "name": self.ARRAY_SIZE_ENTRY_NAME,
            "datatype": "uint8",
            "type": "var",
            "access": "r",
            "default": value,
        }


class VarCommon(AccessorMixin, UnitMixin, BaseModel):
    """Common attributes for variable objects."""
    datatype: str
    limits: Limits = Limits()
    pdo: bool = False
    scale: Optional[float] = None
    enum: Optional[EnumData] = None
    default: Union[int, float] = 0
    bitfield: Optional[Bitfield] = None


class Var(HeaderCommon, VarCommon):
    """Variable object for storing subindex data."""
    type: Literal["var"] = "var"
    class Config:
        extra = "forbid"


class ArrayEntry(AccessorMixin, UnitMixin, BaseModel):
    """Array entry object for storing subindex data."""
    limits: Limits = Limits()
    enum: Optional[EnumData] = None
    default: Union[int, float] = 0


class BaseArray(HeaderCommon, VarCommon, InferArrayLengthMixin):
    """Base class for array objects."""
    type: Literal["array"] = "array"
    length: Optional[Annotated[int, Field(ge=0, le=255)]] = None


class Array(BaseArray):
    """Array object for storing subindex data."""
    data: List[ArrayEntry] = []


class RecordEntry(VarCommon, HeaderCommon):
    """Record entry object for storing subindex data."""
    class Config:
        extra = "forbid"


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
                datatype="uint8",
                access="r",
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


class Baudrate(set):
    """Set of supported baudrates."""

    valid_baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}

    def __init__(self, iterable=()):
        if not all(item in self.valid_baudrates for item in iterable):
            raise ValueError(
                f"Invalid Baudrate: {iterable}. Valid values are: {self.valid_baudrates}"
            )
        super().__init__(item for item in iterable if item in self.valid_baudrates)

    def to_dict(self) -> dict[int, bool]:
        """Convert the Baudrate set to a dictionary."""
        return {b: (b in self) for b in self.valid_baudrates}

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        return core_schema.no_info_after_validator_function(
            cls._validate,
            core_schema.list_schema(items_schema=core_schema.int_schema()),
            serialization=core_schema.plain_serializer_function_ser_schema(list),
        )

    @classmethod
    def _validate(cls, v):
        if isinstance(v, dict):
            # e.g. {125: True, 250: None, 300: False}
            v = [k for k, val in v.items() if val is not False]
        elif not isinstance(v, (list, set, tuple)):
            raise TypeError(f"Invalid Baudrate input: {v}")
        return cls(v)

    def __repr__(self):
        return f"Baudrate({', '.join(map(str, sorted(self)))})"


class Revision(BaseModel):
    """Revision number of the device."""

    major: Annotated[int, Field(ge=0, le=255)]
    minor: Annotated[int, Field(ge=0, le=255)]
    patch: Annotated[int, Field(ge=0, le=255)] = 0

    @model_validator(mode="before")
    @classmethod
    def parse_revision(cls, v: Any) -> dict:
        """Parse the revision number from various formats."""
        if isinstance(v, str):
            try:
                ver = semver.VersionInfo.parse(v)
            except ValueError as e:
                raise ValueError(f"Invalid version string: {v}. {str(e)}") from e
            return {"major": ver.major, "minor": ver.minor, "patch": ver.patch}

        elif isinstance(v, (list, tuple)):
            if len(v) == 2:
                major, minor = v
                return {"major": major, "minor": minor, "patch": 0}
            elif len(v) == 3:
                major, minor, patch = v
                return {"major": major, "minor": minor, "patch": patch}
            else:
                raise ValueError(
                    "List/tuple must be [major, minor] or [major, minor, patch]."
                )

        elif isinstance(v, dict):
            if "major" in v and "minor" in v:
                return {
                    "major": v["major"],
                    "minor": v["minor"],
                    "patch": v.get("patch", 0),
                }
            raise ValueError("Dict must have at least 'major' and 'minor'.")

        elif isinstance(v, semver.VersionInfo):
            return {"major": v.major, "minor": v.minor, "patch": v.patch}

        raise TypeError(f"Invalid input for Revision: {v} (type: {type(v)})")


class VendorProduct(BaseModel):
    """Vendor and product information."""

    name: str = Field(default="Unknown", min_length=1)
    number: int = Field(0x12345678, ge=0, le=0xFFFFFFFF)

    @model_validator(mode="after")
    def check_default_values(self):
        """Check if default values are set and warn the user."""
        if self.name == "Unknown":
            warnings.warn(
                "Vendor name is set to default value 'Unknown'. Please set a valid name.",
                UserWarning,
                stacklevel=2,
            )
        if self.number == 0x12345678:
            warnings.warn(
                "Vendor number is set to default value 0x12345678. Please set a valid number.",
                UserWarning,
                stacklevel=2,
            )
        return self


class Device(BaseModel):
    """Device information."""

    vendor: VendorProduct = Field(default_factory=VendorProduct)
    product: VendorProduct = Field(default_factory=VendorProduct)
    revision: Revision = Field(default_factory=lambda: Revision.model_validate("1.0.0"))
    baudrate: Baudrate
    node_id: int = Field(1, ge=1, le=127)

    @field_validator("baudrate", mode="before")
    @classmethod
    def convert_baudrate(cls, v: Any) -> Baudrate:
        """Convert various formats to Baudrate."""
        if isinstance(v, dict):
            # {125: True, 250: None, 300: False}
            return Baudrate(k for k, val in v.items() if val is not False)
        elif isinstance(v, (list, set)):
            return Baudrate(v)
        raise TypeError("Invalid baudrate format: expected dict, list, or set.")


ObjectType = Annotated[Union[Var, Array, Record], Field(discriminator="type")]


class Objects(MappingRootMixin[ObjectType], RootModel[Dict[int, ObjectType]]):
    """Mapping of object index to object definition."""

    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        """Resolve inheritance and infer types for objects."""
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        def resolve(idx: int, visited: set[int]) -> dict:
            obj = v[idx]
            inherit_id = obj.get("inherit")
            if not inherit_id:
                return obj
            if inherit_id not in v:
                raise ValueError(
                    f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h"
                )
            if inherit_id in visited:
                raise ValueError(f"Circular inheritance detected at {idx:04X}h")
            base = resolve(inherit_id, visited | {idx})
            merged = {**base, **obj}
            return merged

        v = {idx: resolve(idx, set()) for idx in v}

        for obj in v.values():
            if "type" not in obj:
                if "length" in obj or "data" in obj:
                    obj["type"] = "array"
                elif "record" in obj:
                    obj["type"] = "record"
                else:
                    obj["type"] = "var"

        return v


class SchemaConfig(BaseModel):
    """Configuration schema for the generator."""

    device: Device
    profiles: List[Annotated[int, Field(ge=300, le=500)]]
    objects: Objects
