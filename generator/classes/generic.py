from abc import ABC, abstractmethod



class ObjectEntry:
    def __init__(self, accessType: int, dataType: int, ctype: str, parameterName: str, defaultValue) -> None:
        self.accessType = accessType
        self.dataType = dataType
        self.ctype = ctype
        self.parameterName = parameterName
        self.defaultValue = 0 if defaultValue is None else defaultValue


class ObjectBase(ABC):
    def __init__(self, index: int, subNumber: int, objectType: int, cppEntryName: str = "ObjectEntry", cppObjectName: str = "Object") -> None:
        self.index = index
        self.subNumber = subNumber
        self.objectType = objectType
        self.cppEntryName = cppEntryName
        self.cppObjectName = cppObjectName
        self.varName = "x%X" % self.index

    def renderObject(self) -> str:
        return f"{self.cppObjectName} object_{self.varName} = {self.cppObjectName}({self.index}, {self.subNumber}, {self.objectType}, entries.entries_{self.varName})"
    
    @abstractmethod
    def renderData(self) -> list[str]:
        return ""

    @abstractmethod
    def renderEntry(self) -> str:
        return ""
    
    def verify(self, objects: dict) -> bool:
        return True


class VarObject(ObjectBase, ObjectEntry):
    def __init__(self, index: int, accessType: int, dataType: int, ctype: str, parameterName: str, defaultValue) -> None:
        ObjectBase.__init__(self, index, 1, 0x07)
        ObjectEntry.__init__(self, accessType, dataType, ctype, parameterName, defaultValue)

    def renderData(self) -> list[str]:
        return [f"{self.ctype} {self.varName} = {self.defaultValue}"]
    
    def renderEntry(self) -> str:
        init = f"{self.cppEntryName}(&data.{self.varName}, {self.accessType}, {self.dataType}, sizeof(data.{self.varName}))"
        return f"{self.cppEntryName} entries_{self.varName}[{self.subNumber}] = {{{init}}}"


class ArrayObject(ObjectBase):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, len(entries), 0x08)
        self.entries = entries
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> list[str]:
        sub = f"uint8_t {self.sub0Name} = {self.entries[0].defaultValue}"
        init = ", ".join([str(entry.defaultValue) for entry in self.entries])
        arr = f"{self.entries[1].ctype} {self.varName}[{self.subNumber}] = {{{init}}}"
        return [sub, arr]
    
    def renderEntry(self) -> str:
        sub = f"{self.cppEntryName}(&data.{self.sub0Name}, {self.entries[0].accessType}, {self.entries[0].dataType}, sizeof(data.{self.sub0Name}))"
        init = ", ".join([sub, *[f"{self.cppEntryName}(&data.{self.varName}[{i}], {entry.accessType}, {entry.dataType}, sizeof(data.{self.varName}[{i}]))" for i, entry in enumerate(self.entries[1:])]])
        return f"{self.cppEntryName} entries_{self.varName}[{self.subNumber}] = {{{init}}}"


class RecordObject(ObjectBase):
    def __init__(self, index: int, entries: list[ObjectEntry], cppObjectName: str = "Object") -> None:
        super().__init__(index, len(entries), 0x08, cppObjectName=cppObjectName)
        self.entries = entries
        self.sub0Name = self.varName + "sub0"

    def renderData(self) -> list[str]:
        fields = "; ".join([f"{entry.ctype} sub{i} = {entry.defaultValue}" for i, entry in enumerate(self.entries)]) + ";"
        return [f"struct {{{fields}}} {self.varName}"]
    
    def renderEntry(self) -> str:
        init = ", ".join([f"{self.cppEntryName}(&data.{self.varName}.sub{i}, {entry.accessType}, {entry.dataType}, sizeof(data.{self.varName}.sub{i}))" for i, entry in enumerate(self.entries)])
        return f"{self.cppEntryName} entries_{self.varName}[{self.subNumber}] = {{{init}}}"
