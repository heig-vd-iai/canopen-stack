from canopen.objectdictionary import Variable
from .generic import ArrayObject

class Object1010(ArrayObject):
    """Object 1010: Store parameters"""
    def __init__(self, index: int, entries: "list[Variable]") -> None:
        super().__init__(index, entries, "Object1010")
