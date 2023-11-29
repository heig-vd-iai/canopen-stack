from abc import ABC, abstractmethod
from .entries import ObjectEntry
from canopen.objectdictionary import Variable
from .entries import datatype2entryclass, accesstypes, ObjectEntry
ENTRY_MAX_SIZE = 1024

class ObjectBase(ABC):
    """This is the base class for OD's objects"""
    def __init__(self, index: int, entries: "list[Variable]", cppObjectName: str = "Object", cppBaseEntryName: str = "ObjectEntryBase") -> None:
        self.index: int = index
        self.subNumber: int = len(entries)
        self.cppObjectName: str = cppObjectName
        self.cppBaseEntryName: str = cppBaseEntryName
        self.varName: str = "x%X" % self.index
        self.entries: "list[ObjectEntry]" = []
        ## Check for valid data type, access type and data size (for strings)
        errors = False
        for i, entry in enumerate(entries):
            entryValid = True
            if entry.data_type not in datatype2entryclass:
                self.error(f"unsupported data type '{entry.data_type:X}' for sub {i}")
                entryValid = False
            if entry.access_type not in accesstypes:
                self.error(f"unknown access type '{entry.access_type}' for sub {i}")
                entryValid = False
            if (entry.min is None) != (entry.max is None):
                self.error(f"LowLimit and HighLimit must be both declared or not at all for sub {i}")
                entryValid = False
            if entryValid: self.entries.append(datatype2entryclass[entry.data_type](entry.access_type, entry.pdo_mappable, entry.default, entry.min, entry.max))
            else: errors = True
        for i, entry in enumerate(self.entries):
            if entry.size <= 0:
                self.error(f"invalid size '{entry.size}' for sub {i}, must be greater than 0")
                errors = True
            if entry.size > ENTRY_MAX_SIZE:
                self.error(f"invalid size '{entry.size}' for sub {i}, must be lower than {ENTRY_MAX_SIZE}")
                errors = True
        if errors: raise ValueError()
        
    def warn(self, message: str) -> None:
        return print(f"[Warning] Object {self.index:X}: " + message)

    def error(self, message: str) -> None:
        return print(f"[Error] Object {self.index:X}: " + message)

    def renderObject(self) -> str:
        """Returns the C++ object declaration, ex. Object x1003 = Object(...)"""
        return f"{self.cppObjectName} {self.varName} = {self.cppObjectName}({self.index}, {self.subNumber}, entries.{self.varName})"
    
    def renderEntryList(self) -> str:
        """Returns the C++ entry list, ex. ObjectEntryBase *x1003[] = {&x1003sub0, ...}"""
        subs = [f"&{self.varName}sub{sub}" for sub in range(len(self.entries))]
        return f"{self.cppBaseEntryName} *{self.varName}[{len(self.entries)}] = {{{', '.join(subs)}}}"
    
    @abstractmethod
    def renderData(self) -> "list[str]": return []

    @abstractmethod
    def renderEntry(self) -> "list[str]": return []


class VarObject(ObjectBase):
    def __init__(self, index: int, entries: "list[Variable]", cppObjectName: str = "Object") -> None:
        super().__init__(index, entries, cppObjectName=cppObjectName)
        self.entry = self.entries[0]

    def renderData(self) -> "list[str]":
        """Returns the C++ data declaration, ex. uint16_t x1003 = 42"""
        return [self.entry.renderData(self.varName)]
    
    def renderEntry(self) -> "list[str]":
        """Returns the C+++ object entry declaration, ex. ObjectEntry<T> x1003sub0 = ObjectEntry<T>(...)"""
        return [self.entry.renderEntry(f"{self.varName}sub0", self.varName)]


class ArrayObject(ObjectBase):
    def __init__(self, index: int, entries: "list[Variable]", cppObjectName: str = "Object") -> None:
        super().__init__(index, entries, cppObjectName=cppObjectName)
        self.sub0Name = self.varName + "sub0"
        ## Check for data type consistency based on first entry
        errors = False
        if(any([self.entries[1].dataType != entry.dataType for entry in self.entries[1:]])):
            self.error(f"at least one entry has incorrect data type")
            errors = True
        if errors: raise ValueError()

    def renderData(self) -> "list[str]":
        """Returns the C++ data declaration, ex. uint8_t x1003sub0 = 3; uint16_t x1003[3] = {42, 84, 126}"""
        sub = self.entries[0].renderData(self.sub0Name)
        init = ", ".join([str(entry.value) for entry in self.entries[1:]])
        arr = f"{self.entries[1].ctype} {self.varName}[{self.subNumber - 1}] = {{{init}}}"
        return [sub, arr]
    
    def renderEntry(self) -> "list[str]":
        """Returns the C+++ object entry declaration, ex. ObjectEntry<T> x1003sub0 = ObjectEntry<T>(...)"""
        sub0 = self.entries[0].renderEntry(self.sub0Name, self.sub0Name)
        subs = [entry.renderEntry(f"{self.varName}sub{sub + 1}", f"{self.varName}[{sub}]") for sub, entry in enumerate(self.entries[1:])]
        return [sub0, *subs]


class RecordObject(ObjectBase):
    def __init__(self, index: int, entries: "list[Variable]", cppObjectName: str = "Object") -> None:
        super().__init__(index, entries, cppObjectName=cppObjectName)
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> "list[str]":
        """Returns the C++ data declaration, ex. struct{uint8_t sub0 = 2; uint32_t sub1 = 42; uint32_t sub2 = 84;} x1003;"""
        fields = "; ".join([entry.renderData(f"sub{i}") for i, entry in enumerate(self.entries)]) + ";"
        return [f"struct {{{fields}}} {self.varName}"]
    
    def renderEntry(self) -> "list[str]":
        """Returns the C+++ object entry declaration, ex. ObjectEntry<T> x1003sub0 = ObjectEntry<T>(...)"""
        return [entry.renderEntry(f"{self.varName}sub{sub}", f"{self.varName}.sub{sub}") for sub, entry in enumerate(self.entries)]
