"""Flatten a validated configuration into what the templates consume."""

import re
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Union

from ..validation import Enum, Record, RecordEntry, SchemaConfig, Var
from ..validation.models.array import Array
from ..validation.models.object_common import NODE_ID_DEFAULT
from ..validation.models.object_type import ObjectType

RPDO_RANGE = range(0x1400, 0x1600)
TPDO_RANGE = range(0x1800, 0x1A00)
DEVICE_TYPE_INDEX = 0x1000
MODES_OF_OPERATION = "Modes of operation"
DEFAULT_MODULE = "manufacturer"
VALUE_PLACEHOLDER = "@"

TYPE_CODES = [
    "bool",
    "i8",
    "i16",
    "i32",
    "i64",
    "u8",
    "u16",
    "u32",
    "u64",
    "f32",
    "f64",
    "str",
]

CALL = re.compile(r"\)\s*$")


def eds_access(access) -> str:
    text = access.to_eds()
    if not text:
        raise ValueError("an object needs an access right or an accessor")
    return text


def format_scale(value: float) -> str:
    if value == int(value):
        return str(int(value))
    return f"{value:.6g}f"


def render_default(value, node_id: int):
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, str):
        match = NODE_ID_DEFAULT.match(value)
        if match:
            return int(match.group(1), 0) + node_id
    return value


@dataclass
class SubObjectView:
    """One sub-index as seen by the templates."""

    index: int
    subindex: int
    name: str
    type: str
    type_value: str
    ctype_name: str
    type_code: str
    access: str
    pdo_mapping: int
    default: Union[int, float, str]
    low_limit: Optional[Union[int, float]]
    high_limit: Optional[Union[int, float]]
    length: Optional[int]
    remote: bool
    get: Optional[str]
    set: Optional[str]
    scale: Optional[float]
    enum: Optional[Enum]

    @property
    def index_hex(self) -> str:
        return f"{self.index:x}"

    @property
    def cpp_instance_name(self) -> str:
        return f"object{self.index_hex}sub{self.subindex}"

    @property
    def parameter_name(self) -> str:
        return self.name

    @property
    def limited(self) -> bool:
        return self.low_limit is not None and self.high_limit is not None

    @property
    def readable(self) -> bool:
        return self.access != "wo"

    @property
    def writable(self) -> bool:
        return self.access != "ro"

    @property
    def meta_data(self) -> str:
        mappable = bool(self.pdo_mapping)
        return (
            f"0b{int(self.remote)}{int(self.limited)}{int(mappable)}"
            f"{int(self.writable)}{int(self.readable)}"
        )

    @property
    def local_getter(self) -> Optional[str]:
        return self.get if self.get and not self.remote else None

    @property
    def local_setter(self) -> Optional[str]:
        return self.set if self.set and not self.remote else None

    @property
    def setter_function(self) -> bool:
        return self.limited or bool(self.local_setter and self.writable)

    def _scaled(self, expression: str, factor: Optional[float]) -> str:
        if factor is None or factor == 1:
            return expression
        return f"{expression} * {format_scale(factor)}"

    @property
    def remote_getter(self) -> Optional[str]:
        """C++ expression producing the value on the remote core."""
        if not (self.remote and self.get):
            return None
        inverse = None if self.scale is None else 1 / self.scale
        return self._scaled(self.get, inverse)

    @property
    def remote_setter(self) -> Optional[str]:
        """C++ statement writing the value on the remote core."""
        if not (self.remote and self.set):
            return None
        value = self._scaled(f"data.{self.type_code}", self.scale)
        if VALUE_PLACEHOLDER in self.set:
            return self.set.replace(VALUE_PLACEHOLDER, value)
        if CALL.search(self.set):
            return self.set
        cast = f"({self.enum.typedef})" if self.enum else ""
        return f"{self.set} = {cast}{value}"


@dataclass
class ObjectView:
    """One object dictionary entry as seen by the templates."""

    index: int
    name: str
    module: str
    description: str
    category: str
    remote: bool
    object_type: str
    subobjects: List[SubObjectView] = field(default_factory=list)
    enum: Optional[Enum] = None

    @property
    def index_hex(self) -> str:
        return f"{self.index:x}"

    @property
    def subNumber(self) -> int:
        return len(self.subobjects)

    @property
    def data(self) -> List[SubObjectView]:
        return self.subobjects


@dataclass
class RenderContext:
    config: SchemaConfig
    file_name: str
    objects: List[ObjectView]

    @property
    def node_id(self) -> int:
        return self.config.device.node_id

    @property
    def subindex_count(self) -> int:
        return sum(len(obj.subobjects) for obj in self.objects)

    @property
    def type_count(self) -> Dict[str, int]:
        counts = {code: 0 for code in TYPE_CODES}
        for obj in self.objects:
            for sub in obj.subobjects:
                if sub.type_code in counts:
                    counts[sub.type_code] += 1
        return counts

    @property
    def nrOfRXPDO(self) -> int:
        return sum(1 for obj in self.objects if obj.index in RPDO_RANGE)

    @property
    def nrOfTXPDO(self) -> int:
        return sum(1 for obj in self.objects if obj.index in TPDO_RANGE)

    @property
    def mandatoryObjects(self) -> List[ObjectView]:
        return [obj for obj in self.objects if obj.category == "mandatory"]

    @property
    def optionalObjects(self) -> List[ObjectView]:
        return [obj for obj in self.objects if obj.category == "optional"]

    @property
    def modules(self) -> List[str]:
        modules: List[str] = []
        for obj in self.objects:
            if obj.module not in modules:
                modules.append(obj.module)
        return modules

    @property
    def modes_of_operation(self) -> Optional[ObjectView]:
        return next(
            (
                obj
                for obj in self.objects
                if obj.name == MODES_OF_OPERATION and obj.enum
            ),
            None,
        )


def _subobject(
    index: int, subindex: int, entry, remote: bool, node_id: int
) -> SubObjectView:
    datatype = entry.datatype
    return SubObjectView(
        index=index,
        subindex=subindex,
        name=entry.name,
        type=datatype.name,
        type_value=f"0x{datatype.code:02X}",
        ctype_name=datatype.ctype,
        type_code=datatype.field,
        access=eds_access(entry.access),
        pdo_mapping=int(entry.pdo),
        default=render_default(entry.default, node_id),
        low_limit=entry.limits.min,
        high_limit=entry.limits.max,
        length=entry.size,
        remote=remote,
        get=entry.get,
        set=entry.set,
        scale=entry.scale,
        enum=entry.enum,
    )


def _object_type(obj) -> str:
    if isinstance(obj, Var):
        name = "domain" if obj.datatype_name == "domain" else "var"
    else:
        name = obj.type
    return f"0x{ObjectType(name=name).cia_id:02X}"


def _entries(obj) -> List[RecordEntry]:
    if isinstance(obj, (Record, Array)):
        return obj.subentries()
    return [obj]


def _device_type(config: SchemaConfig) -> str:
    return f"0x0000{config.device_profile:04X}"


def build_context(config: SchemaConfig, file_name: str) -> RenderContext:
    """Flatten the configuration; objects are sorted by index."""
    node_id = config.device.node_id
    objects = []
    for index in sorted(config.objects):
        obj = config.objects[index]
        remote = obj.is_remote
        subobjects = [
            _subobject(index, subindex, entry, remote, node_id)
            for subindex, entry in enumerate(_entries(obj))
        ]
        if index == DEVICE_TYPE_INDEX:
            subobjects[0].default = _device_type(config)
        objects.append(
            ObjectView(
                index=index,
                name=obj.name,
                module=obj.module or DEFAULT_MODULE,
                description=str(obj.description),
                category=obj.category,
                remote=remote,
                object_type=_object_type(obj),
                subobjects=subobjects,
                enum=obj.enum if isinstance(obj, Var) else None,
            )
        )
    return RenderContext(config=config, file_name=file_name, objects=objects)
