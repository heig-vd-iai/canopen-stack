from canopen.objectdictionary import Variable
from .generic import RecordObject


class Object1400(RecordObject):
    """Object 1400: RPDO communication parameter"""

    def __init__(self, index: int, entries: "list[Variable]", entriesGranularity: int = 1) -> None:
        super().__init__(index, entries, "Object1400", entriesGranularity)
        # Check for valid sub number
        errors = False
        if self.subNumber < 3:
            self.error(f"at least 3 entries should be present")
            errors = True
        if errors:
            raise ValueError()
