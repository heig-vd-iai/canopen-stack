from canopen.objectdictionary import Variable
from .generic import ArrayObject


class Object1003(ArrayObject):
    """Object 1003: Pre-defined error field"""

    def __init__(self, index: int, entries: "list[Variable]", entriesGranularity: int = 1) -> None:
        super().__init__(index, entries, "Object1003", entriesGranularity)
        # Check for valid sub number
        errors = False
        if self.subNumber < 2:
            self.error(f"at least 2 entries are required")
            errors = True
        if errors:
            raise ValueError()
