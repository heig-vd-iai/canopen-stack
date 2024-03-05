from enum import Enum

class Access(Enum):
    rw = "rw"
    ro = "ro"
    wo = "wo"
    const = "const"
    none = "none"

class Type(Enum):
    bool = "bool"
    int8 = "int8"
    int16 = "int16"
    int32 = "int32"
    int64 = "int64"
    uint8 = "uint8"
    uint16 = "uint16"
    uint32 = "uint32"
    uint64 = "uint64"
    float32 = "float32"
    float64 = "float64"
    string = "string"

class DataType(Enum):
    bool = "0x01"
    int8 = "0x02"
    int16 = "0x03"
    int32 = "0x04"
    int64 = "0x15"
    uint8 = "0x05"
    uint16 = "0x06"
    uint32 = "0x07"
    uint64 = "0x1B"
    float32 = "0x08"
    float64 = "0x11"
    string = "0x09"

class ObjectType(Enum):
    null = "0x00"
    domain = "0x02"
    deftype = "0x05"
    defstruct = "0x06"
    var = "0x07"
    array = "0x08"
    record = "0x09"
