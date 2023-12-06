from canopen.objectdictionary import Variable
from .generic import RecordObject


class Object1800(RecordObject):
    """Object 1800: TPDO communication parameter"""

    def __init__(self, index: int, entries: "list[Variable]") -> None:
        super().__init__(index, entries, "Object1800")
        # Check for valid sub number
        errors = False
        if self.subNumber < 3:
            self.error(f"at least 3 entries should be present")
            errors = True
        if errors:
            raise ValueError()
