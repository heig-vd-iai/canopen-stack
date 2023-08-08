from canopen.objectdictionary import Variable
from .generic import RecordObject

class Object1800(RecordObject):
    """Object 1800: TPDO communication parameter"""
    def __init__(self, index: int, entries: list[Variable]) -> None:
        super().__init__(index, entries, "Object1800")
        ## Check for valid sub number, set entries 0 and 1 to "const"
        errors = False
        if self.subNumber < 3:
            self.error(f"at least 3 entries should be present")
            errors = True
        for i in [0, 1]:
            if self.entries[i].accessType.writeable:
                self.warn(f"sub {i} will be set to 'const'")
                self.entries[i].accessType.set("const")
        if errors: raise ValueError()
