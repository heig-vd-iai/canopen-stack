"""Model for CANopen data types."""
from typing import Any, Dict, NamedTuple

from pydantic import BaseModel
from pydantic_core import core_schema


class DatatypeInfo(NamedTuple):
    """Information about a CANopen datatype."""
    name: str
    code: int
    ctype: str
    field: str


DATATYPES: Dict[str, DatatypeInfo] = {
    "bool": DatatypeInfo("bool", 0x01, "bool", "b"),
    "int8": DatatypeInfo("int8", 0x02, "int8_t", "i8"),
    "int16": DatatypeInfo("int16", 0x03, "int16_t", "i16"),
    "int32": DatatypeInfo("int32", 0x04, "int32_t", "i32"),
    "uint8": DatatypeInfo("uint8", 0x05, "uint8_t", "u8"),
    "uint16": DatatypeInfo("uint16", 0x06, "uint16_t", "u16"),
    "uint32": DatatypeInfo("uint32", 0x07, "uint32_t", "u32"),
    "float32": DatatypeInfo("float32", 0x08, "float", "f32"),
    "string": DatatypeInfo("string", 0x09, "string", "str"),
    "domain": DatatypeInfo("domain", 0x0F, "domain", "domain"),
    "float64": DatatypeInfo("float64", 0x11, "double", "f64"),
    "int64": DatatypeInfo("int64", 0x15, "int64_t", "i64"),
    "uint64": DatatypeInfo("uint64", 0x1B, "uint64_t", "u64"),
}


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
