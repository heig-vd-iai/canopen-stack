from typing import List, Optional

from .validation.models.config import Array, Config, Record, RecordEntry, Var
from .validation.models.types import ObjectId, TypeValue


class FlattenedObject(RecordEntry):
    """Flattened object for storing subindex data."""

    id: ObjectId
    type: TypeValue
    index: int

    # Only valid for array entries or record entries
    parent_type: Optional[TypeValue] = None


def flatten_var(index: int, var: Var) -> List[FlattenedObject]:
    return [
        FlattenedObject.model_validate(
            {
                **var.model_dump(),
                "id": ObjectId(index=index, subindex=0),
                "type": TypeValue(var.datatype),
                "index": var.index,
            }
        )
    ]


def flatten_record(record: Record) -> List[FlattenedObject]:
    objects = []
    for entry in record.record:
        objects.append(
            FlattenedObject.model_validate(
                {
                    **entry.model_dump(),
                    "name": entry.name or record.name,
                    "description": entry.description or record.description,
                    "id": ObjectId(index=record.index, subindex=entry.subindex),
                    "type": TypeValue(record.datatype),
                    "index": record.index,
                }
            )
        )
    return objects


def flatten_array(config: Config) -> Array[FlattenedObject]:
    objects = []
    for entry in config.data:
        objects.append(
            FlattenedObject.model_validate(
                {
                    **entry.model_dump(),
                    "name": entry.name or config.name,
                    "description": entry.description or config.description,
                    "id": ObjectId(index=config.index, subindex=entry.subindex),
                    "type": TypeValue(config.datatype),
                    "index": config.index,
                }
            )
        )
    return objects


def flatten_object_dictionary(config: Config) -> Array[FlattenedObject]:
    match config.type:
        case "record":
            return flatten_record(config)
        case "array":
            return flatten_array(config)
        case "var":
            return flatten_var(config)
        case _:
            raise ValueError(f"Unsupported type: {config.type}")
