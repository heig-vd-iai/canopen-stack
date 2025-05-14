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
        if item == 'index':
            return self.index
        elif item == 'subindex':
            return self.subindex
        else:
            raise KeyError(f"Invalid key: {item}")

TypeValues = namedtuple('TypeValues', ['name', 'code', 'ctype', 'field'])

datatypes = {
    'bool': TypeValues('bool', 0x01, 'bool', 'b'),
    'int8': TypeValues('int8', 0x02, 'int8_t', 'i8'),
    'int16': TypeValues('int16', 0x03, 'int16_t', 'i16'),
    'int32': TypeValues('int32', 0x04, 'int32_t', 'i32'),
    'uint8': TypeValues('uint8', 0x05, 'uint8_t', 'u8'),
    'uint16': TypeValues('uint16', 0x06, 'uint16_t', 'u16'),
    'uint32': TypeValues('uint32', 0x07, 'uint32_t', 'u32'),
    'float32': TypeValues('float32', 0x08, 'float', 'f32'),
    'string': TypeValues('string', 0x09, 'string', 'str'),
    'domain': TypeValues('domain', 0x0F, 'domain', 'domain'),
    'float64': TypeValues('float64', 0x11, 'double', 'f64'),
    'int64': TypeValues('int64', 0x15, 'int64_t', 'i64'),
    'uint64': TypeValues('uint64', 0x1B, 'uint64_t', 'u64'),
}

object_types = {
    'null': 0x0,
    'domain': 0x2,
    'deftype': 0x5,
    'defstruct': 0x6,
    'var': 0x7,
    'array': 0x8,
    'record': 0x9,
}
