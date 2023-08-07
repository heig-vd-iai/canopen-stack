from abc import ABC, abstractmethod
from .entries import ObjectEntry
from canopen.objectdictionary import Variable
from .entries import datatype2entryclass, accesstypes, ObjectEntry

class ObjectBase(ABC):
    def __init__(self, index: int, objectType: int, entries: list[Variable], cppEntryName: str = "ObjectEntry", cppObjectName: str = "Object") -> None:

        self.index: int = index
        self.subNumber: int = len(entries)
        self.objectType: int = objectType
        self.cppEntryName: str = cppEntryName
        self.cppObjectName: str = cppObjectName
        self.varName: str = "x%X" % self.index
        self.entries: list[ObjectEntry] = []
        errors = False
        for i, entry in enumerate(entries):
            entryValid = True
            if entry.data_type not in datatype2entryclass:
                self.error(f"unsupported data type '{entry.data_type:X}' for sub {i}")
                entryValid = False
            if entry.access_type not in accesstypes:
                self.error(f"unknown access type '{entry.access_type}' for sub {i}")
                entryValid = False
            if entryValid: self.entries.append(datatype2entryclass[entry.data_type](entry.access_type, entry.default))
            else: errors = True
        for i, entry in enumerate(self.entries):
            if entry.size <= 0:
                self.error(f"invalid size '{entry.size}' for sub {i}")
                errors = True
        if errors: raise ValueError()
        
    def warn(self, message: str) -> None:
        return print(f"[Warning] Object {self.index:X}: " + message)

    def error(self, message: str) -> None:
        return print(f"[Error] Object {self.index:X}: " + message)

    def renderObject(self) -> str:
        return f"{self.cppObjectName} {self.varName} = {self.cppObjectName}({self.index}, {self.subNumber}, {self.objectType}, entries.{self.varName})"
    
    @abstractmethod
    def renderData(self) -> list[str]:
        return ""

    @abstractmethod
    def renderEntry(self) -> str:
        return ""


class VarObject(ObjectBase):
    def __init__(self, index: int, entries: list[Variable]) -> None:
        super().__init__(index, 0x07, entries)
        self.entry = self.entries[0]

    def renderData(self) -> list[str]:
        return [self.entry.renderData(self.varName)]
    
    def renderEntry(self) -> str:
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{self.entry.renderEntry(self.cppEntryName, self.varName)}}}"


class ArrayObject(ObjectBase):
    def __init__(self, index: int, entries: list[Variable], cppObjectName: str = "Object") -> None:
        super().__init__(index, 0x08, entries, cppObjectName=cppObjectName)
        self.sub0Name = self.varName + "sub0"
        errors = False
        if(any([self.entries[1].dataType != entry.dataType for entry in self.entries[1:]])):
            self.error(f"at least one entry has incorrect data type")
            errors = True
        if errors: raise ValueError()

    def renderData(self) -> list[str]:
        sub = self.entries[0].renderData(self.sub0Name)
        init = ", ".join([str(entry.defaultValue) for entry in self.entries[1:]])
        arr = f"{self.entries[1].ctype} {self.varName}[{self.subNumber - 1}] = {{{init}}}"
        return [sub, arr]
    
    def renderEntry(self) -> str:
        sub = self.entries[0].renderEntry(self.cppEntryName, self.sub0Name)
        init = ", ".join([sub, *[entry.renderEntry(self.cppEntryName, f"{self.varName}[{i}]") for i, entry in enumerate(self.entries[1:])]])
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{init}}}"


class RecordObject(ObjectBase):
    def __init__(self, index: int, entries: list[Variable], cppObjectName: str = "Object") -> None:
        super().__init__(index, 0x08, entries, cppObjectName=cppObjectName)
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> list[str]:
        fields = "; ".join([entry.renderData(f"sub{i}") for i, entry in enumerate(self.entries)]) + ";"
        return [f"struct {{{fields}}} {self.varName}"]
    
    def renderEntry(self) -> str:
        init = ", ".join([entry.renderEntry(self.cppEntryName, f"{self.varName}.sub{i}") for i, entry in enumerate(self.entries)])
        return f"{self.cppEntryName} {self.varName}[{self.subNumber}] = {{{init}}}"
