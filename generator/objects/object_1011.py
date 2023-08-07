from .generic import ObjectEntry, ArrayObject

class Object1011(ArrayObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "Object1011")

    def verify(self, objects: dict) -> bool:
        return super().verify(objects)
