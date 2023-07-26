from .generic import ObjectEntry, RecordObject



class TPDOCommunicationObject(RecordObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "TPDOCommunicationObject")

    def verify(self, objects: dict) -> bool:
        retval = True
        if self.entries[0].defaultValue != len(self.entries) - 1:
            print(f"[Error] Entry {self.varName}: invalid value for sub 0")
            retval = False
        return retval


class TPDOMappingObject(RecordObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "TPDOMappingObject")

    def verify(self, objects: dict) -> bool:
        retval = True
        for i in range(self.entries[0].defaultValue):
            entry = self.entries[i + 1]
            index = entry.defaultValue >> 16 & 0xFFFF
            subindex = entry.defaultValue >> 8 & 0xFF
            length = entry.defaultValue & 0xFF
            if index not in objects:
                print(f"[Error] Entry {self.varName}: invalid TPDO mapping for sub {i + 1} (invalid index)")
                retval = False
            elif objects[index].subNumber <= subindex:
                print(f"[Error] Entry {self.varName}: invalid TPDO mapping for sub {i + 1} (invalid subindex)")
                retval = False
        return retval
