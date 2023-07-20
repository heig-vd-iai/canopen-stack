from .generic import ObjectEntry, RecordObject



class TPDOCommunicationObject(RecordObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "TPDOCommunicationObject")

class TPDOMappingObject(RecordObject):
    def __init__(self, index: int, entries: list[ObjectEntry]) -> None:
        super().__init__(index, entries, "TPDOMappingObject")
