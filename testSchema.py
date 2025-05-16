import os
import re
import warnings
import semver
import mistune
import yaml
import subprocess
import warnings
from pathlib import Path
from collections.abc import Mapping
from datetime import datetime
from typing import Optional, Union, Tuple, Dict, Any, List, Literal, Generic, TypeVar
from pydantic_core import core_schema
from pydantic import (
    BaseModel,
    Field,
    field_validator,
    model_validator,
    StringConstraints,
    GetCoreSchemaHandler,
    ValidationError,
    RootModel
)

from typing import Annotated, Set
from pint import UnitRegistry, UndefinedUnitError

STRICT_VALIDATION = False

ureg = UnitRegistry()
ureg.define("udv = [speed]")
ureg.define("udp = [position]")
ureg.define("uda = [acceleration]")
ureg.define("inc = [increment]")
ureg.define("ratedTorque = [torque]")


def validate_unit_string(value: str) -> str:
    try:
        ureg.parse_units(value)
    except UndefinedUnitError as e:
        raise ValueError(f"Invalid units: {value}: {str(e)}")
    return value

def validate_identifier(value: str) -> str:
    if not value.isidentifier():
        raise ValueError(f"Invalid enum name: {value}. Must be a valid identifier.")
    return value

def resolve_inheritance(objects: dict) -> dict:
    def resolve(idx: int, visited: set[int]) -> dict:
        obj = objects[idx]
        inherit_id = obj.get("inherit")
        if not inherit_id:
            return obj
        if inherit_id not in objects:
            raise ValueError(f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h")
        if inherit_id in visited:
            raise ValueError(f"Circular inheritance involving {idx:04X}h")

        base = resolve(inherit_id, visited | {idx})
        merged = {**base, **obj}
        return merged

    return {idx: resolve(idx, set()) for idx in objects}

T = TypeVar("T")

class MappingRootMixin(Mapping, Generic[T]):
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
        return mistune.create_markdown()(self)

    def __repr__(self):
        preview = str(self).replace("\n", " ")[:40]
        return f"Markdown({preview!r}...)"

    @classmethod
    def validate(cls, value):
        if not isinstance(value, str):
            raise TypeError("Markdown must be a string.")
        try:
            mistune.create_markdown()(value)  # parsing attempt
        except Exception as e:
            raise ValueError(f"Invalid Markdown: {e}")
        return cls(value)

    @classmethod
    def __get_pydantic_core_schema__(cls, source_type, handler: GetCoreSchemaHandler):
        return core_schema.no_info_after_validator_function(
            cls.validate,
            core_schema.str_schema(),
            serialization=core_schema.plain_serializer_function_ser_schema(lambda v: str(v))
        )

class Access:
    """ Object access rights.
    """
    def __init__(self, read=False, write=False):
        self.read = bool(read)
        self.write = bool(write)

    def __str__(self):
        if self.read and self.write:
            return "rw"
        elif self.read:
            return "r"
        elif self.write:
            return "w"
        return ""

    def __repr__(self):
        if self.read and self.write:
            return "Access(Read Write)"
        elif self.read:
            return "Access(Read Only)"
        elif self.write:
            return "Access(Write Only)"
        return "Access(None)"

    def to_dict(self):
        return {"read": self.read, "write": self.write}

    def to_eds(self):
        """ EDS representation is a bit weird:

        r -> ro
        w -> wo
        rw -> rw
        """
        if self.read and self.write:
            return "rw"
        elif self.read:
            return "ro"
        elif self.write:
            return "wo"
        return ""

    @classmethod
    def parse(cls, value):
        if isinstance(value, Access):
            return value
        if isinstance(value, str):
            return cls(read="r" in value, write="w" in value)
        if isinstance(value, dict):
            return cls(read=value.get("read", False), write=value.get("write", False))
        raise TypeError(f"Invalid access value: {value}")

    @classmethod
    def __get_pydantic_core_schema__(cls, source_type, handler: GetCoreSchemaHandler):
        return core_schema.no_info_after_validator_function(
            cls.parse,
            core_schema.str_schema(),
            serialization=core_schema.plain_serializer_function_ser_schema(lambda v: str(v)),
        )

class BitfieldEntry(BaseModel):
    name: str
    values: Dict[int, str] = {}

    @field_validator("values")
    @classmethod
    def check_values_fit_width(cls, values: Dict[int, str], info) -> Dict[int, str]:
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
    @classmethod
    def __get_pydantic_core_schema__(cls, source_type: Any, handler: GetCoreSchemaHandler) -> core_schema.CoreSchema:
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
                        raise ValueError(f"Invalid bit range '{key}': start must be >= end")
                else:
                    raise ValueError(f"Invalid bitfield key: {key}")

                width = start - end + 1

                # Normalize value
                if isinstance(val, str):
                    entry = BitfieldEntry(name=val, values={})
                elif isinstance(val, dict):
                    entry = BitfieldEntry(**val)
                else:
                    raise TypeError(f"Invalid bitfield entry value for key {key}: {val}")

                # Inject width dynamically to perform value checks
                entry._width = width  # not public, internal use
                entry = BitfieldEntry.model_validate(entry.model_dump())  # re-validate
                result[(start, end)] = entry
            return cls(result)

        return core_schema.no_info_after_validator_function(
            validate,
            core_schema.dict_schema(
                keys_schema=core_schema.any_schema(),
                values_schema=core_schema.any_schema()
            ),
            serialization=core_schema.plain_serializer_function_ser_schema(
                lambda v: {f"{k[0] if k[0] == k[1] else f'{k[0]}..{k[1]}'}": e.model_dump() for k, e in v.items()}
            )
        )

class HeaderCommon(BaseModel):
    name: str
    remote: Optional[str] = Field(default="local", min_length=1)
    path: Annotated[
            Optional[str],
            StringConstraints(pattern=r"^([a-zA-Z][a-zA-Z0-9]*)(?:\.[a-zA-Z][a-zA-Z0-9]*)*$")
        ] = ""
    description: Markdown = Field(default=Markdown(""))
    inherit: Optional[int] = None

class Limits(BaseModel):
    min: Optional[Union[int, float]] = None
    max: Optional[Union[int, float]] = None

class EnumData(BaseModel):
    class_: str = Field(..., alias="class")
    data: Dict[str, int]

    @model_validator(mode='before')
    @classmethod
    def validate_enum_data(cls, values):
        if 'data' not in values:
            raise ValueError("Enum field data is required.")
        if not isinstance(values.get("data"), dict):
            raise ValueError("Enum data must be a dictionary.")
        if len(set(values["data"].values())) != len(values["data"]):
            raise ValueError("Enum values must be unique.")
        return values

    @field_validator('data', mode='before')
    @classmethod
    def validate_enum_keys(cls, v):
        return {validate_identifier(k): val for k, val in v.items()}

class VarCommon(BaseModel):
    datatype: str
    limits: Limits = Limits()
    pdo: bool = False
    unit: Optional[str] = Field(default="", validate_default=True)
    scale: Optional[float] = None
    enum: Optional[EnumData] = None
    default: Union[int, float] = 0
    access: Access = Field(default_factory=Access)
    bitfield: Optional[Bitfield] = None

    get: Optional[str] = None
    set: Optional[str] = None

    @field_validator('unit')
    @classmethod
    def validate_unit(cls, v):
        return validate_unit_string(v)

    @model_validator(mode='after')
    def update_access(self):
        inferred = False

        # 1. Si access est vide → inférer
        if not self.access.read and not self.access.write:
            read = self.get is not None
            write = self.set is not None
            self.access = Access(read=read, write=write)
            inferred = True

        # 2. Si access est défini → valider cohérence avec get/set
        if not inferred:
            if self.get is not None and not self.access.read:
                raise ValueError("Access mismatch: 'get' defined but 'read' not allowed in 'access'.")
            if self.set is not None and not self.access.write:
                raise ValueError("Access mismatch: 'set' defined but 'write' not allowed in 'access'.")

        # 3. Sinon : access est conservé tel quel

        return self

class Var(HeaderCommon, VarCommon):
    type: Literal['var'] = 'var'

    class Config:
        extra = 'forbid'

class ArrayEntry(BaseModel):
    limits: Limits = Limits()
    unit: Optional[str] = Field(default="", validate_default=True)
    enum: Optional[EnumData] = None
    default: Union[int, float] = 0
    access: Annotated[
            Optional[str],
            StringConstraints(pattern=r"^r?w?$")
        ] = ""
    get: Optional[str] = None
    set: Optional[str] = None

    @field_validator('unit')
    @classmethod
    def validate_unit(cls, v):
        return validate_unit_string(v)

class Array(HeaderCommon, VarCommon):
    type: Literal['array'] = 'array'
    length: Optional[Annotated[int, Field(ge=0, le=255)]] = None
    data: List[ArrayEntry] = []

    @model_validator(mode="after")
    def infer_length_from_data(self) -> "Array":
        if self.length is None:
            self.length = len(self.data)
        elif self.length < len(self.data):
            raise ValueError(f"Inconsistent array length: length={self.length} but data has {len(self.data)} entries.")
        return self

class RecordEntry(HeaderCommon):
    datatype: str
    limits: Limits = Limits()
    pdo: bool = False
    unit: Optional[str] = Field(default="", validate_default=True)
    scale: Optional[float] = None
    enum: Optional[EnumData] = None
    default: Union[int, float] = 0
    bitfield: Optional[Bitfield] = None
    access: Annotated[
            Optional[str],
            StringConstraints(pattern=r"^r?w?$")
        ] = ""
    get: Optional[str] = None
    set: Optional[str] = None

    @field_validator('unit')
    @classmethod
    def validate_unit(cls, v):
        return validate_unit_string(v)

    class Config:
        extra = 'forbid'

class Record(HeaderCommon):
    type: Literal['record'] = 'record'
    record: List[RecordEntry]

    @model_validator(mode='after')
    def validate_subindex_length(self):
        if len(self.record) > 255:
            raise ValueError("Subindex length must be less than 256.")
        return self

class Domain(HeaderCommon):
    type: Literal['domain'] = 'domain'
    domain: Dict[str, Annotated[int, Field(ge=1)]]


class Baudrate(set):
    valid_baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}

    def __init__(self, iterable=()):
        if not all(item in self.valid_baudrates for item in iterable):
            raise ValueError(f"Invalid Baudrate: {iterable}. Valid values are: {self.valid_baudrates}")
        super().__init__(item for item in iterable if item in self.valid_baudrates)

    def to_dict(self) -> dict[int, bool]:
        return {b: (b in self) for b in self.valid_baudrates}

    @classmethod
    def __get_pydantic_core_schema__(cls, source_type, handler: GetCoreSchemaHandler) -> core_schema.CoreSchema:
        return core_schema.no_info_after_validator_function(
            cls._validate,
            core_schema.list_schema(items_schema=core_schema.int_schema()),
            serialization=core_schema.plain_serializer_function_ser_schema(lambda v: list(v)),
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
    major: Annotated[int, Field(ge=0, le=255)]
    minor: Annotated[int, Field(ge=0, le=255)]
    patch: Annotated[int, Field(ge=0, le=255)] = 0

    @model_validator(mode='before')
    @classmethod
    def parse_revision(cls, v: Any) -> dict:
        if isinstance(v, str):
            try:
                ver = semver.VersionInfo.parse(v)
            except ValueError as e:
                raise ValueError(f"Invalid version string: {v}. {str(e)}")
            return {"major": ver.major, "minor": ver.minor, "patch": ver.patch}

        elif isinstance(v, (list, tuple)):
            if len(v) == 2:
                major, minor = v
                return {"major": major, "minor": minor, "patch": 0}
            elif len(v) == 3:
                major, minor, patch = v
                return {"major": major, "minor": minor, "patch": patch}
            else:
                raise ValueError("List/tuple must be [major, minor] or [major, minor, patch].")

        elif isinstance(v, dict):
            if 'major' in v and 'minor' in v:
                return {
                    "major": v['major'],
                    "minor": v['minor'],
                    "patch": v.get('patch', 0)
                }
            raise ValueError("Dict must have at least 'major' and 'minor'.")

        elif isinstance(v, semver.VersionInfo):
            return {"major": v.major, "minor": v.minor, "patch": v.patch}

        raise TypeError(f"Invalid input for Revision: {v} (type: {type(v)})")

class VendorProduct(BaseModel):
    name: str = Field(default="Unknown", min_length=1)
    number: int = Field(0x12345678, ge=0, le=0xFFFFFFFF)

    @model_validator(mode='after')
    def check_default_values(self):
        if self.name == "Unknown":
            warnings.warn(
                "Vendor name is set to default value 'Unknown'. Please set a valid name.",
                UserWarning,
                stacklevel=2
            )
        if self.number == 0x12345678:
            warnings.warn(
                "Vendor number is set to default value 0x12345678. Please set a valid number.",
                UserWarning,
                stacklevel=2
            )
        return self

class Device(BaseModel):
    vendor: VendorProduct = Field(default_factory=VendorProduct)
    product: VendorProduct = Field(default_factory=VendorProduct)
    revision: Revision = Field(default_factory=lambda: Revision.model_validate("1.0.0"))
    baudrate: Baudrate
    node_id: int = Field(1, ge=1, le=127)

    @field_validator('baudrate', mode='before')
    @classmethod
    def convert_baudrate(cls, v: Any) -> Baudrate:
        if isinstance(v, dict):
            # {125: True, 250: None, 300: False}
            return Baudrate(k for k, val in v.items() if val is not False)
        elif isinstance(v, (list, set)):
            return Baudrate(v)
        raise TypeError("Invalid baudrate format: expected dict, list, or set.")


ObjectType = Annotated[Union[Var, Array, Record], Field(discriminator="type")]

class Objects(MappingRootMixin[ObjectType], RootModel[Dict[int, ObjectType]]):
    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        # Étape 1 : Résolution de l’héritage
        def resolve(idx: int, visited: set[int]) -> dict:
            obj = v[idx]
            inherit_id = obj.get("inherit")
            if not inherit_id:
                return obj
            if inherit_id not in v:
                raise ValueError(f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h")
            if inherit_id in visited:
                raise ValueError(f"Circular inheritance detected at {idx:04X}h")
            base = resolve(inherit_id, visited | {idx})
            merged = {**base, **obj}
            return merged

        v = {idx: resolve(idx, set()) for idx in v}

        # Étape 2 : Inférence du type
        for idx, obj in v.items():
            if "type" not in obj:
                if "length" in obj or "data" in obj:
                    obj["type"] = "array"
                elif "record" in obj:
                    obj["type"] = "record"
                else:
                    obj["type"] = "var"

        return v


class SchemaConfig(BaseModel):
    device: Device
    profiles: List[Annotated[int, Field(ge=300, le=500)]]
    objects: Objects

# Profiles have some extra fields

class EnumDataProfile(EnumData):
    override_ranges: List[Tuple[int, int]] = Field(default_factory=list)

class ProfileExtra(BaseModel):
    category: Literal['conditional', 'optional', 'mandatory'] = Field(default='optional')
    mandatory_conditions: List[str] = Field(default_factory=list)

class VarProfile(ProfileExtra, Var):
    enum: Optional[EnumDataProfile] = None

class ArrayEntryProfile(ArrayEntry):
    enum: Optional[EnumDataProfile] = None

class ArrayProfile(ProfileExtra, Array):
    data: List[ArrayEntryProfile] = []

    @model_validator(mode="after")
    def infer_length_from_data(self) -> "ArrayProfile":
        if self.length is None:
            self.length = len(self.data)
        elif self.length < len(self.data):
            raise ValueError(f"Inconsistent array length: length={self.length} but data has {len(self.data)} entries.")
        return self

class RecordEntryProfile(ProfileExtra, RecordEntry):
    enum: Optional[EnumDataProfile] = None

class RecordProfile(ProfileExtra, Record):
    record: List[RecordEntryProfile] = []


ObjectTypeProfile = Annotated[Union[VarProfile, ArrayProfile, RecordProfile], Field(discriminator="type")]

class ObjectsProfile(MappingRootMixin[ObjectTypeProfile], RootModel[Dict[int, ObjectTypeProfile]]):
    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        # Étape 1 : Résolution de l’héritage
        def resolve(idx: int, visited: set[int]) -> dict:
            obj = v[idx]
            inherit_id = obj.get("inherit")
            if not inherit_id:
                return obj
            if inherit_id not in v:
                raise ValueError(f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h")
            if inherit_id in visited:
                raise ValueError(f"Circular inheritance detected at {idx:04X}h")
            base = resolve(inherit_id, visited | {idx})
            merged = {**base, **obj}
            return merged

        v = {idx: resolve(idx, set()) for idx in v}

        # Étape 2 : Inférence du type
        for idx, obj in v.items():
            if "type" not in obj:
                if "length" in obj or "data" in obj:
                    obj["type"] = "array"
                elif "record" in obj:
                    obj["type"] = "record"
                else:
                    obj["type"] = "var"

        return v

class Profile(BaseModel):
    name: str
    description: Markdown = Field(default=Markdown(""))
    standards: List[str] = []
    features: Dict[str, str] = {}
    objects: ObjectsProfile

    @field_validator('features', mode='before')
    @classmethod
    def validate_enum_keys(cls, v):
        return {validate_identifier(k): val for k, val in v.items()}

class SchemaProfile(BaseModel):
    profiles: Dict[Annotated[int, Field(ge=300, le=500)], Profile]



def _git_info(filepath: str, field: str) -> Optional[str]:
    try:
        with open(os.devnull, 'w') as devnull:
            if field == "author":
                return subprocess.check_output(
                    ["git", "log", "-1", "--pretty=format:%an", "--", filepath],
                    stderr=devnull
                ).decode().strip()
            elif field == "date":
                return subprocess.check_output(
                    ["git", "log", "-1", "--pretty=format:%ad", "--date=iso", "--", filepath],
                    stderr=devnull
                ).decode().strip()
            elif field == "version":
                return subprocess.check_output(
                    ["git", "describe", "--tags", "--always"],
                    stderr=devnull
                ).decode().strip()
    except Exception:
        return None

def readConfig(filename: str) -> dict:
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"Configuration file not found: {filename}")

    with filepath.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}

    info = data.setdefault("info", {})

    # created_by
    if "created_by" not in info:
        author = _git_info(filename, "author") or "unknown"
        info["created_by"] = author
        warnings.warn(f"'/info/created_by' not specified. Using Git author or fallback: {author}")

    if "created_at" not in info:
        date = _git_info(filename, "date")
        if date:
            info["created_at"] = date
        else:
            fallback = datetime.now().isoformat()
            info["created_at"] = fallback
            warnings.warn(f"'/info/created_at' not specified. Using current time: {fallback}")

    if "modified_by" not in info:
        info["modified_by"] = info.get("created_by", "unknown")

    if "modified_at" not in info:
        info["modified_at"] = datetime.now().isoformat()

    if "filename" not in info:
        info["filename"] = str(filepath.name)

    if "revision" not in info:
        rev = _git_info(filename, "version") or "0.1.0"
        info["revision"] = rev
        warnings.warn(f"'/info/revision' not specified. Using Git tag or fallback: {rev}")

    return data

def readProfile(profile: int) -> dict:
    filename = f"generator/profiles/{profile}.yaml"
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"Configuration file not found: {filename}")

    with filepath.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}

    return data


def format_validation_errors(error: ValidationError, data: dict) -> str:
    lines = []

    for err in error.errors():
        loc = err["loc"]

        path_parts = []
        for i, key in enumerate(loc):
            if i >= 1 and loc[i - 1] == "objects" and isinstance(key, int):
                path_parts.append(f"{key:04X}h")
            else:
                path_parts.append(str(key))

        path = "/" + "/".join(path_parts)
        msg = err.get("msg") or err.get("message", str(err))
        lines.append(f"- {path}: {msg}")

    return "\n".join(lines)

def merge_profiles(*schemas: SchemaProfile) -> SchemaProfile:
    merged_profiles: Dict[int, Profile] = {}

    for schema in schemas:
        for pid, profile in schema.profiles.items():
            if pid in merged_profiles:
                raise ValueError(f"Duplicate profile {pid} encountered during merge.")
            merged_profiles[pid] = profile

    return SchemaProfile(profiles=merged_profiles)

with warnings.catch_warnings(record=True) as w:
    warnings.simplefilter("always")

    try:
        data = readConfig("config2.yaml")
        config = SchemaConfig.model_validate(data)

        data = readProfile(301)
        profile301 = SchemaProfile.model_validate(data)

        data = readProfile(402)
        profile402 = SchemaProfile.model_validate(data)

        profiles = merge_profiles(profile301, profile402)

    except FileNotFoundError as e:
        print(f"Configuration file not found: {e}")
        config = None
    except ValidationError as e:
        print("Validation error(s):")
        print(format_validation_errors(e, data))
        if STRICT_VALIDATION:
            raise
        else:
            config = None  # ou retour anticipé, ou status code
    else:
        if w:
            print("\n=== WARNINGS ===")
            for warning in w:
                print(f"- {warning.message}")
            print()
