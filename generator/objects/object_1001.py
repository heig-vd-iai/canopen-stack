from canopen.objectdictionary import Variable
from .generic import VarObject


class Object1001(VarObject):
    """Object 1001: Error register"""

    def __init__(self, index: int, entries: "list[Variable]", entriesGranularity: int = 1) -> None:
        super().__init__(index, entries, "Object1001", entriesGranularity)
