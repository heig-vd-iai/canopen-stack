from collections import namedtuple


class ObjectCode(int):
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


TypeValues = namedtuple("TypeValues", ["name", "code", "ctype", "field"])

datatypes = {
    "bool": TypeValues("bool", 0x01, "bool", "b"),
    "int8": TypeValues("int8", 0x02, "int8_t", "i8"),
    "int16": TypeValues("int16", 0x03, "int16_t", "i16"),
    "int32": TypeValues("int32", 0x04, "int32_t", "i32"),
    "uint8": TypeValues("uint8", 0x05, "uint8_t", "u8"),
    "uint16": TypeValues("uint16", 0x06, "uint16_t", "u16"),
    "uint32": TypeValues("uint32", 0x07, "uint32_t", "u32"),
    "float32": TypeValues("float32", 0x08, "float", "f32"),
    "string": TypeValues("string", 0x09, "string", "str"),
    "domain": TypeValues("domain", 0x0F, "domain", "domain"),
    "float64": TypeValues("float64", 0x11, "double", "f64"),
    "int64": TypeValues("int64", 0x15, "int64_t", "i64"),
    "uint64": TypeValues("uint64", 0x1B, "uint64_t", "u64"),
}

object_types = {
    "null": 0x0,
    "domain": 0x2,
    "deftype": 0x5,
    "defstruct": 0x6,
    "var": 0x7,
    "array": 0x8,
    "record": 0x9,
}

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

units = {
    "m": 0x1,
    "kg": 0x2,
    "s": 0x3,
    "A": 0x4,
    "K": 0x5,
    "mol": 0x6,
    "cd": 0x7,
    "rad": 0x10,
    "sr": 0x11,
    "Hz": 0x20,
    "N": 0x21,
    "Pa": 0x22,
    "J": 0x23,
    "W": 0x24,
    "C": 0x25,
    "V": 0x26,
    "F": 0x27,
    "Ohm": 0x28,
    "S": 0x29,
    "Wb": 0x2A,
    "T": 0x2B,
    "H": 0x2C,
    "°C": 0x2D,
    "lm": 0x2E,
    "lx": 0x2F,
    "Bq": 0x30,
    "Gy": 0x31,
    "Sv": 0x32,
}

# prefix = 10**(int8)prefix
