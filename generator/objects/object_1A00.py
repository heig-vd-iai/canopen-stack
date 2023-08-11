from canopen.objectdictionary import Variable
from .generic import RecordObject

class Object1A00(RecordObject):
    """Object 1A00: TPDO mapping parameter"""
    def __init__(self, index: int, entries: list[Variable]) -> None:
        super().__init__(index, entries, "Object1A00")
        self._MAX_SIZE = 8

    def verify(self, objects: dict) -> bool:
        ## Check that mapped objects exist and size is valid
        retval = True
        sizeSum = 0
        for i in range(self.entries[0].defaultValue):
            entry = self.entries[i + 1]
            index = entry.defaultValue >> 16 & 0xFFFF
            subindex = entry.defaultValue >> 8 & 0xFF
            sizeSum += (entry.defaultValue & 0xFF) // 8
            if index not in objects:
                self.error(f"invalid TPDO mapping for sub {i + 1} (invalid index)")
                retval = False
            elif objects[index].subNumber <= subindex:
                self.error(f"invalid TPDO mapping for sub {i + 1} (invalid subindex)")
                retval = False
        if sizeSum > self._MAX_SIZE:
            self.error(f"invalid size ({sizeSum})")
            retval = False
        return retval
