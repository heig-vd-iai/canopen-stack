from abc import ABC, abstractmethod
from .entry_types import Entry


class Object(ABC):
    index: int
    parameter_name: str
    type_value: int
    type_name: str
    getter: str
    setter: str
    entries: "list[Entry]"

    @classmethod
    def get_instance(cls, type_name: str, data: dict, node_id: int = 0):
        return next(c(data) for c in cls.__subclasses__() if c.type_name == type_name)

    def __init__(self, data: dict) -> None:
        self.index = int(data.get("Index"), 16)
        self.parameter_name: str = data.get("ParameterName", "")
        self.getter: str = data.get("Getter", "none")
        self.setter: str = data.get("Setter", "none") ##TODO: add remote getter and setter
        self.entries: list[Entry] = [Entry.get_instance(subdata.get("DataType"), subdata, i, self.getter, self.setter) for i, subdata in enumerate(data.get("SubEntries", [data]))]

    @abstractmethod
    def __str__(self) -> str:
        ...

    @property
    def index_hexstr(self) -> str:
        return f"{self.index:X}"

    @property
    def index_hexstr2(self) -> str:
        return f"0x{self.index:X}"

    @property
    def type_hexstr(self) -> str:
        return f"0x{self.type_value:02X}"

    @property
    def sub_number(self) -> int:
        return len(self.entries)

    @property
    def cpp_class_name(self) -> str:
        if 0x1400 <= self.index <= 0x15FF:
            return "Object1400"
        if 0x1600 <= self.index <= 0x17FF:
            return "Object1600"
        if 0x1800 <= self.index <= 0x19FF:
            return "Object1800"
        if 0x1A00 <= self.index <= 0x1BFF:
            return "Object1A00"
        return "Object" + self.index_hexstr

    @property
    def cpp_instance_name(self) -> str:
        return "object" + self.index_hexstr


class VarObject(Object):
    type_name: str = "VAR"
    type_value: int = 0x07

    def __init__(self, data: dict) -> None:
        super().__init__(data)

    def __str__(self) -> str:
        return f"[{self.index_hexstr}]\n{str(self.entries[0])}"


class ArrayObject(Object):
    type_name: str = "ARRAY"
    type_value: int = 0x08

    def __init__(self, data: dict) -> None:
        super().__init__(data)

    def __str__(self) -> str:
        subs = '\n\n'.join([f"[{self.index_hexstr}sub{sub.subindex}]\n{str(sub)}" for sub in self.entries])
        return f"""[{self.index_hexstr}]
ParameterName={self.parameter_name}
ObjectType={self.type_hexstr}
SubNumber={str(self.sub_number)}
\n{subs}"""


class RecordObject(Object):
    type_name: str = "RECORD"
    type_value: int = 0x09

    def __init__(self, data: dict) -> None:
        super().__init__(data)

    def __str__(self) -> str:
        subs = '\n\n'.join([f"[{self.index_hexstr}sub{sub.subindex}]\n{str(sub)}" for sub in self.entries])
        return f"""[{self.index_hexstr}]
ParameterName={self.parameter_name}
ObjectType={self.type_hexstr}
SubNumber={str(self.sub_number)}
\n{subs}"""