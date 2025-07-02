from typing import ClassVar, Dict

from pydantic import BaseModel, model_validator


class ObjectType(BaseModel):
    """Pydantic model representing object types."""

    OBJECTS_TYPE: ClassVar[Dict[str, int]] = {
        "null": 0x0,
        "domain": 0x2,
        "deftype": 0x5,
        "defstruct": 0x6,
        "var": 0x7,
        "array": 0x8,
        "record": 0x9,
    }

    name: str
    cia_id: int

    @model_validator(mode="before")
    def resolve_name_and_code(cls, values):
        """Resolve name and cia_id regardless of input form."""
        name = values.get("name")
        cia_id = values.get("cia_id")

        if name is not None:
            if name not in cls.OBJECTS_TYPE:
                raise ValueError(f"Invalid object type name: {name}")
            return {"name": name, "cia_id": cls.OBJECTS_TYPE[name]}
        elif cia_id is not None:
            for key, value in cls.OBJECTS_TYPE.items():
                if value == cia_id:
                    return {"name": key, "cia_id": cia_id}
            raise ValueError(f"Invalid cia_id: {cia_id:#x}")
        else:
            raise ValueError("Either 'name' or 'cia_id' must be provided.")

    class Config:
        frozen = True
