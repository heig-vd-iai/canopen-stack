from abc import ABC

class ObjectEntry(ABC):
    def __init__(self, dataType: int, ctype: str, size: int, accessType: str, defaultDefaultValue, defaultValue) -> None:
        self.dataType: int = dataType
        self.ctype: str = ctype
        self.size: int = size
        self.defaultValue = defaultDefaultValue if defaultValue is None else defaultValue
        self.accessTypeStr = accessType
        ## Get access type from string
        read_bit = 1 << 0
        write_bit = 1 << 1
        const_bit = 1 << 2
        if accessType in ["rw", "rww", "rwr"]: self.accessType: int = write_bit | read_bit
        elif accessType == "wo": self.accessType: int = write_bit
        elif accessType == "ro": self.accessType: int = read_bit
        elif accessType == "const": self.accessType: int = const_bit | read_bit
        else: self.accessType: int = 0

    def renderData(self, name: str) -> str:
        return f"{self.ctype} {name} = {self.defaultValue}"
    
    def renderEntry(self, entryClassName: str, entryVarName: str) -> str:
        return f"{entryClassName}(&data.{entryVarName}, {self.accessType}, {self.dataType}, {self.size})"

class BooleanEntry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: bool) -> None:
        super().__init__(0x01, "bool", 1, accessType, 0, defaultValue)

class Integer8Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x02, "int8_t", 1, accessType, 0, defaultValue)

class Integer16Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x03, "int16_t", 2, accessType, 0, defaultValue)

class Integer32Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x04, "int32_t", 4, accessType, 0, defaultValue)

class Integer64Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x15, "int64_t", 8, accessType, 0, defaultValue)

class Unsigned8Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x05, "uint8_t", 1, accessType, 0, defaultValue)

class Unsigned16Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x06, "uint16_t", 2, accessType, 0, defaultValue)

class Unsigned32Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x07, "uint32_t", 4, accessType, 0, defaultValue)

class Unsigned64Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: int) -> None:
        super().__init__(0x1B, "uint64_t", 8, accessType, 0, defaultValue)

class Real32Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: float) -> None:
        super().__init__(0x08, "float", 4, accessType, 0.0, defaultValue)

class Real64Entry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: float) -> None:
        super().__init__(0x11, "double", 8, accessType, 0.0, defaultValue)

class VisibleStringEntry(ObjectEntry):
    def __init__(self, accessType: str, defaultValue: str) -> None:
        default = "".encode() if defaultValue is None else str(defaultValue).encode()
        super().__init__(0x09, "uint8_t", len(default), accessType, "", default)

    def renderData(self, name: str) -> str:
        return f"{self.ctype} {name}[{self.size}] = {{{', '.join([str(b) for b in self.defaultValue])}}}"
