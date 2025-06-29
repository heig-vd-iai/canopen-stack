"""Types definitions according to the CiA 301 specification."""

from typing import ClassVar, Dict, Optional, Union

from pydantic import BaseModel


class ObjectId(int):
    """Object identifier for CANopen objects."""

    def __new__(cls, index, subindex):
        return super().__new__(cls, (index << 8) | subindex)

    def __init__(self, index, subindex):
        self.index = index
        self.subindex = subindex
        self.eds_name = f"{index:04x}sub{subindex:x}"

    def __str__(self):
        return f"0x{self.index:04x}_{self.subindex:02d}"

    def __repr__(self):
        return f"ObjectCode(index={self.index:04x}, subindex={self.subindex})"

    def __getitem__(self, item):
        if item == "index":
            return self.index
        elif item == "subindex":
            return self.subindex
        else:
            raise KeyError(f"Invalid key: {item}")


class Limits(BaseModel):
    """Limits for variable types."""

    min: Optional[Union[int, float]] = None
    max: Optional[Union[int, float]] = None


class ObjectType:
    """Class to represent object values."""

    OBJECTS_TYPE: ClassVar[Dict[str, int]] = {
        "null": 0x0,
        "domain": 0x2,
        "deftype": 0x5,
        "defstruct": 0x6,
        "var": 0x7,
        "array": 0x8,
        "record": 0x9,
    }

    def __init__(self, name):
        if name not in self.OBJECTS_TYPE:
            raise ValueError(f"Invalid object type name: {name}")
        self.name = name
        self.code = self.OBJECTS_TYPE[name]


objects_ranges = {
    (0x0001, 0x001F): "Static data types",
    (0x0020, 0x003F): "Complex data types",
    (0x0040, 0x005F): "Manufacturer-specific complex data types",
    (0x0060, 0x025F): "Device profile specific data types",
    # Reserved range
    (0x1000, 0x1FFF): "Communication profile area",
    (0x2000, 0x5FFF): "Manufacturer-specific profile area",
    (0x6000, 0x67FF): "Standardized profile 1st logical device",
    (0x6800, 0x6FFF): "Standardized profile 2nd logical device",
    (0x7000, 0x77FF): "Standardized profile 3rd logical device",
    (0x7800, 0x7FFF): "Standardized profile 4th logical device",
    (0x8000, 0x87FF): "Standardized profile 5th logical device",
    (0x8800, 0x8FFF): "Standardized profile 6th logical device",
    (0x9000, 0x97FF): "Standardized profile 7th logical device",
    (0x9800, 0x9FFF): "Standardized profile 8th logical device",
    (0xA000, 0xAFFF): "Standardized network variable area",
    (0xB000, 0xBFFF): "Standardized system variable area",
    # Reserved range
}
