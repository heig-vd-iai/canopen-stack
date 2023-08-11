from abc import ABC

accesstypes = ["rw", "rww", "rwr", "ro", "wo", "const"]

class AccessType:
    """Represents an entrie's access type + ability do be PDO mapped value in a convenient way"""
    def __init__(self, accessType: str, PDOMappable: bool) -> None:
        self._READ = 1 << 0
        self._WRITE = 1 << 1
        self._MAPPABLE = 1 << 2
        self._value: int = 0
        self._mappable: bool = PDOMappable
        self.set(accessType)

    @property
    def value(self) -> int:
        return self._value
    
    @property
    def readable(self) -> bool:
        return bool(self._value & self._READ)
    
    @property
    def writeable(self) -> bool:
        return bool(self._value & self._WRITE)
    
    @property
    def mappable(self) -> bool:
        return self._mappable
    
    def set(self, name: str) -> None:
        if name in ["rw", "rww", "rwr"]: self._value = self._WRITE | self._READ
        elif name in ["ro", "const"]: self._value = self._READ
        elif name == "wo": self._value = self._WRITE
        if self._mappable: self._value |= self._MAPPABLE


class ObjectEntry(ABC):
    """This class serves as the base class for all object entries, that must be subclassed for each data type"""
    def __init__(self, dataType: int, ctype: str, size: int, accessType: str, PDOMappable: bool, defaultDefaultValue, defaultValue) -> None:
        self.dataType: int = dataType
        self.ctype: str = ctype
        self.size: int = size
        self.defaultValue = defaultDefaultValue if defaultValue is None else defaultValue
        self.accessType = AccessType(accessType, PDOMappable)

    def renderData(self, name: str) -> str:
        return f"{self.ctype} {name} = {self.defaultValue}"
    
    def renderEntry(self, entryClassName: str, entryVarName: str) -> str:
        return f"{entryClassName}(&data.{entryVarName}, {self.accessType.value}, {self.size})"

class BooleanEntry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: bool) -> None:
        super().__init__(0x01, "bool", 1, accessType, PDOMappable, 0, defaultValue)

class Integer8Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x02, "int8_t", 1, accessType, PDOMappable, 0, defaultValue)

class Integer16Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x03, "int16_t", 2, accessType, PDOMappable, 0, defaultValue)

class Integer32Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x04, "int32_t", 4, accessType, PDOMappable, 0, defaultValue)

class Integer64Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x15, "int64_t", 8, accessType, PDOMappable, 0, defaultValue)

class Unsigned8Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x05, "uint8_t", 1, accessType, PDOMappable, 0, defaultValue)

class Unsigned16Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x06, "uint16_t", 2, accessType, PDOMappable, 0, defaultValue)

class Unsigned32Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x07, "uint32_t", 4, accessType, PDOMappable, 0, defaultValue)

class Unsigned64Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: int) -> None:
        super().__init__(0x1B, "uint64_t", 8, accessType, PDOMappable, 0, defaultValue)

class Real32Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: float) -> None:
        super().__init__(0x08, "float", 4, accessType, PDOMappable, 0.0, defaultValue)

class Real64Entry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: float) -> None:
        super().__init__(0x11, "double", 8, accessType, PDOMappable, 0.0, defaultValue)

class VisibleStringEntry(ObjectEntry):
    def __init__(self, accessType: str, PDOMappable: bool, defaultValue: str) -> None:
        default = "".encode() if defaultValue is None else str(defaultValue).encode()
        super().__init__(0x09, "uint8_t", len(default), accessType, PDOMappable, "", default)

    def renderData(self, name: str) -> str:
        return f"{self.ctype} {name}[{self.size}] = {{{', '.join([str(b) for b in self.defaultValue])}}}"

datatype2entryclass = {
    0x01: BooleanEntry,
    0x02: Integer8Entry,
    0x03: Integer16Entry,
    0x04: Integer32Entry,
    0x05: Unsigned8Entry,
    0x06: Unsigned16Entry,
    0x07: Unsigned32Entry,
    0x08: Real32Entry,
    0x09: VisibleStringEntry,
    0x0A: VisibleStringEntry,
    0x0B: VisibleStringEntry,
    0x0C: Unsigned64Entry,
    0x0D: Unsigned64Entry,
    0x10: Integer32Entry,
    0x11: Real64Entry,
    0x12: Integer64Entry,
    0x13: Integer64Entry,
    0x14: Integer64Entry,
    0x15: Integer64Entry,
    0x16: Unsigned32Entry,
    0x18: Unsigned64Entry,
    0x19: Unsigned64Entry,
    0x1A: Unsigned64Entry,
    0x1B: Unsigned64Entry
}
