"""CANopen record object model."""

from typing import List, Literal, Optional, Union

from pydantic import ConfigDict, model_validator

from .access import Access
from .datatype import Datatype
from .enum import Enum, EnumProfile
from .object_common import (
    HeaderCommon,
    HeaderCommonProfile,
    Sub0,
    VarCommon,
    VarCommonProfile,
)

MAX_SUBENTRIES = 255


class RecordEntry(VarCommon, HeaderCommon):
    """One sub-index of a record."""

    model_config = ConfigDict(extra="forbid")


class RecordEntryProfile(VarCommonProfile, HeaderCommonProfile):
    """One sub-index of a record defined in a CiA profile."""

    enum: Optional[Union[Enum, EnumProfile]] = None
    model_config = ConfigDict(extra="forbid")


def size_entry(
    sub0: Sub0, count: int, get: Optional[str] = None, set: Optional[str] = None
) -> RecordEntry:
    """Build the sub-index 0 entry announcing the highest sub-index."""
    return RecordEntry(
        name=sub0.name,
        datatype=Datatype.from_name("uint8"),
        access=Access(read=sub0.access.read, write=sub0.access.write),
        default=count if sub0.default is None else sub0.default,
        get=get,
        set=set,
    )


class RecordBase(HeaderCommon):
    """Record object: heterogeneous entries at sub-indices 1..n.

    Object-level `get`/`set` apply to every entry that has none of its own.
    """

    type: Literal["record"] = "record"
    get: Optional[str] = None
    set: Optional[str] = None
    sub0: Sub0 = Sub0()
    record: list

    @model_validator(mode="after")
    def check_length(self):
        if len(self.record) > MAX_SUBENTRIES:
            raise ValueError(f"a record holds at most {MAX_SUBENTRIES} entries")
        return self

    def subentries(self) -> List[RecordEntry]:
        """Every sub-index in order, sub-index 0 included."""
        entries = [size_entry(self.sub0, len(self.record), self.get, self.set)]
        for entry in self.record:
            entries.append(
                entry.model_copy(
                    update={
                        "get": entry.get if entry.get is not None else self.get,
                        "set": entry.set if entry.set is not None else self.set,
                    }
                )
            )
        return entries


class Record(RecordBase):
    """Record object from a device configuration."""

    record: List[RecordEntry]

    model_config = ConfigDict(extra="forbid")


class RecordProfile(RecordBase, HeaderCommonProfile):
    """Record object defined in a CiA profile."""

    record: List[RecordEntryProfile] = []

    model_config = ConfigDict(extra="forbid")
