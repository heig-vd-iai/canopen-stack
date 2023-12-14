from canopen.objectdictionary import Variable
from .generic import VarObject


class Object1019(VarObject):
    """Object 1019: Synchronous counter overflow value"""

    def __init__(self, index: int, entries: "list[Variable]", entriesGranularity: int = 1) -> None:
        super().__init__(index, entries, "Object1019", entriesGranularity)
