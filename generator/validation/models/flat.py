from typing import Optional

from .object_common import HeaderCommon, VarCommon
from .object_id import ObjectId
from .object_type import ObjectType


class FlattenedObject(VarCommon, HeaderCommon):
    """Flattened object for storing subindex data."""

    id: ObjectId
    type: ObjectType
    index: int

    # Only valid for array entries or record entries
    parent_type: Optional[ObjectType] = None
