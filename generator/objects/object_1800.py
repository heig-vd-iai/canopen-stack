from .generic import ObjectEntry, RecordObject

class Object1800(RecordObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "Object1800")

    def verify(self, objects: dict) -> bool:
        retval = True
        if self.entries[0].defaultValue != len(self.entries) - 1:
            print(f"[Error] Object {self.index:X}: invalid value for sub 0")
            retval = False
        return retval
