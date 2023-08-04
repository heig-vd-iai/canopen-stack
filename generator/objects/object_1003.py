from .generic import ObjectEntry, ArrayObject

class Object1003(ArrayObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "Object1003")

    def verify(self, objects: dict) -> bool:
        retval = True
        for i, entry in enumerate(self.entries[2:]):
            if entry.accessTypeStr not in ["ro", "const"]:
                print(f"[Error] Entry {self.varName}: access type of sub {i + 2} is not 'ro' ({entry.accessTypeStr})")
                retval = False
        return retval
