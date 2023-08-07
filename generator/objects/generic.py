from abc import ABC, abstractmethod
from .entries import ObjectEntry

class ObjectBase(ABC):
    def __init__(self, index: int, subNumber: int, objectType: int, cppEntryName: str = "ObjectEntry", cppObjectName: str = "Object") -> None:
        self.index = index
        self.subNumber = subNumber
        self.objectType = objectType
        self.cppEntryName = cppEntryName
        self.cppObjectName = cppObjectName
        self.varName = "x%X" % self.index

    def warn(self, message: str) -> None:
        return print(f"[Warning] Object {self.index:X}: " + message)

    def error(self, message: str) -> None:
        return print(f"[Error] Object {self.index:X}: " + message)
    
    @staticmethod
    def warn_static(index: int, message: str) -> None:
        return print(f"[Warning] Object {index:X}: " + message)

    @staticmethod
    def error_static(index: int, message: str) -> None:
        return print(f"[Error] Object {index:X}: " + message)

    def renderObject(self) -> str:
        return f"{self.cppObjectName} {self.varName} = {self.cppObjectName}({self.index}, {self.subNumber}, {self.objectType}, entries.{self.varName})"
    
    @abstractmethod
    def renderData(self) -> list[str]:
        return ""

    @abstractmethod
    def renderEntry(self) -> str:
        return ""
    
    @abstractmethod
    def verify(self, objects: dict) -> bool:
        return True


class VarObject(ObjectBase):
    def __init__(self, index: int, entry: ObjectEntry) -> None:
        super().__init__(index, 1, 0x07)
        self.entry = entry
        self.entries = [self.entry]

    def renderData(self) -> list[str]:
        return [self.entry.renderData(self.varName)]
    
    def renderEntry(self) -> str:
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{self.entry.renderEntry(self.cppEntryName, self.varName)}}}"
    
    def verify(self, objects: dict) -> bool:
        return True


class ArrayObject(ObjectBase):
    def __init__(self, index: int, entries: list[ObjectEntry], cppObjectName: str = "Object") -> None:
        super().__init__(index, len(entries), 0x08, cppObjectName=cppObjectName)
        self.entries = entries
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> list[str]:
        sub = self.entries[0].renderData(self.sub0Name)
        init = ", ".join([str(entry.defaultValue) for entry in self.entries[1:]])
        arr = f"{self.entries[1].ctype} {self.varName}[{self.subNumber - 1}] = {{{init}}}"
        return [sub, arr]
    
    def renderEntry(self) -> str:
        sub = self.entries[0].renderEntry(self.cppEntryName, self.sub0Name)
        init = ", ".join([sub, *[entry.renderEntry(self.cppEntryName, f"{self.varName}[{i}]") for i, entry in enumerate(self.entries[1:])]])
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{init}}}"
    
    def verify(self, objects: dict) -> bool:
        retval = True
        if(any([self.entries[1].dataType != entry.dataType for entry in self.entries[1:]])):
            print(f"[Error] Entry {self.varName}: at least one entry has incorrect data type")
            retval = False
        return retval


class RecordObject(ObjectBase):
    def __init__(self, index: int, entries: list[ObjectEntry], cppObjectName: str = "Object") -> None:
        super().__init__(index, len(entries), 0x08, cppObjectName=cppObjectName)
        self.entries = entries
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> list[str]:
        fields = "; ".join([entry.renderData(f"sub{i}") for i, entry in enumerate(self.entries)]) + ";"
        return [f"struct {{{fields}}} {self.varName}"]
    
    def renderEntry(self) -> str:
        init = ", ".join([entry.renderEntry(self.cppEntryName, f"{self.varName}.sub{i}") for i, entry in enumerate(self.entries)])
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{init}}}"
    
    def verify(self, objects: dict) -> bool:
        return True
