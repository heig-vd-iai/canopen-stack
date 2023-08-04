from .generic import ObjectEntry, ArrayObject

class Object1010(ArrayObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "Object1010")

    def verify(self, objects: dict) -> bool:
        return super().verify(objects)
