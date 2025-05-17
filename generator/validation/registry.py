from typing import NamedTuple, Dict


class DatatypeInfo(NamedTuple):
    name: str
    code: int
    ctype: str
    field: str

DATATYPES: Dict[str, DatatypeInfo] = {
    "bool":    DatatypeInfo("bool",    0x01, "bool",      "b"),
    "int8":    DatatypeInfo("int8",    0x02, "int8_t",    "i8"),
    "int16":   DatatypeInfo("int16",   0x03, "int16_t",   "i16"),
    "int32":   DatatypeInfo("int32",   0x04, "int32_t",   "i32"),
    "uint8":   DatatypeInfo("uint8",   0x05, "uint8_t",   "u8"),
    "uint16":  DatatypeInfo("uint16",  0x06, "uint16_t",  "u16"),
    "uint32":  DatatypeInfo("uint32",  0x07, "uint32_t",  "u32"),
    "float32": DatatypeInfo("float32", 0x08, "float",     "f32"),
    "string":  DatatypeInfo("string",  0x09, "string",    "str"),
    "domain":  DatatypeInfo("domain",  0x0F, "domain",    "domain"),
    "float64": DatatypeInfo("float64", 0x11, "double",    "f64"),
    "int64":   DatatypeInfo("int64",   0x15, "int64_t",   "i64"),
    "uint64":  DatatypeInfo("uint64",  0x1B, "uint64_t",  "u64"),
}