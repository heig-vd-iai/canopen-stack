"""CANopen array object model."""

from typing import Annotated, List, Literal, Optional, Union

from pydantic import BaseModel, ConfigDict, Field, field_validator, model_validator

from .enum import Enum, EnumProfile
from .limits import Limits
from .mixins import AccessorMixin, UnitMixin
from .object_common import (
    HeaderCommon,
    HeaderCommonProfile,
    Sub0,
    VarCommon,
    VarCommonProfile,
)
from .record import MAX_SUBENTRIES, RecordEntry, size_entry

INDEX_PLACEHOLDER = "#"


class ArrayEntry(AccessorMixin, UnitMixin, HeaderCommon, BaseModel):
    """One sub-index of an array; the datatype comes from the array."""

    limits: Limits = Limits()
    enum: Optional[Enum] = None
    default: Union[int, float] = 0


class ArrayEntryProfile(ArrayEntry):
    """One sub-index of an array defined in a CiA profile."""

    enum: Optional[Union[Enum, EnumProfile]] = None


def ensure_array_entries(v, info, model_cls):
    """Coerce raw entries to models and name the anonymous ones."""
    if v is None:
        return []
    if not isinstance(v, list):
        raise TypeError(f"array must be a list, got {type(v)}")

    result = []
    array_name = info.data.get("name", "Array")

    for i, item in enumerate(v, start=1):
        if isinstance(item, model_cls):
            result.append(item)
        else:
            item_data = dict(item)
            if "name" not in item_data:
                item_data["name"] = f"{array_name}_{i}"
            result.append(model_cls(**item_data))

    return result


def substitute_index(text: Optional[str], subindex: int) -> Optional[str]:
    if text is None:
        return None
    return text.replace(INDEX_PLACEHOLDER, str(subindex))


class ArrayBase(HeaderCommon, VarCommon):
    """Array object: entries of one datatype at sub-indices 1..length."""

    type: Literal["array"] = "array"
    length: Optional[Annotated[int, Field(ge=0, le=MAX_SUBENTRIES)]] = None
    sub0: Sub0 = Sub0()
    array: list

    @model_validator(mode="after")
    def infer_length(self):
        entry_count = len(self.array)
        if self.length is None:
            self.length = entry_count
        elif self.length < entry_count:
            raise ValueError(
                f"Inconsistent array length: length={self.length} "
                f"but data has {entry_count} entries."
            )
        return self

    def subentries(self) -> List[RecordEntry]:
        """Every sub-index in order, sub-index 0 included.

        Entries beyond the declared ones repeat the last declared entry.
        The '#' placeholder in names and accessors becomes the sub-index.
        """
        entries = [size_entry(self.sub0, self.length or 0, self.get, self.set)]
        for subindex in range(1, (self.length or 0) + 1):
            template = (
                self.array[subindex - 1]
                if subindex <= len(self.array)
                else (self.array[-1] if self.array else None)
            )
            entries.append(self._entry(template, subindex))
        return entries

    def _entry(self, template: Optional[ArrayEntry], subindex: int) -> RecordEntry:
        explicit = template.model_fields_set if template else set()

        def pick(field: str):
            if template is not None and field in explicit:
                return getattr(template, field)
            return getattr(self, field)

        name = template.name if template else f"{self.name} {subindex}"
        return RecordEntry(
            name=substitute_index(name, subindex),
            datatype=self.datatype,
            remote=self.remote,
            description=template.description if template else self.description,
            module=self.module,
            category=self.category,
            access=pick("access"),
            get=substitute_index(pick("get"), subindex),
            set=substitute_index(pick("set"), subindex),
            unit=pick("unit"),
            scale=pick("scale"),
            limits=pick("limits"),
            enum=pick("enum"),
            default=pick("default"),
            pdo=self.pdo,
            bitfield=self.bitfield,
            size=self.size,
        )


class Array(ArrayBase):
    """Array object from a device configuration."""

    array: List[ArrayEntry] = []

    model_config = ConfigDict(extra="forbid")

    @field_validator("array", mode="before")
    @classmethod
    def ensure_array_entry(cls, v, info):
        return ensure_array_entries(v, info, ArrayEntry)


class ArrayProfile(ArrayBase, HeaderCommonProfile, VarCommonProfile):
    """Array object defined in a CiA profile."""

    array: List[ArrayEntryProfile] = []
    max_length: Annotated[int, Field(ge=0, le=MAX_SUBENTRIES)] = Field(
        default=MAX_SUBENTRIES
    )

    model_config = ConfigDict(extra="forbid")

    @field_validator("array", mode="before")
    @classmethod
    def ensure_array_entry(cls, v, info):
        return ensure_array_entries(v, info, ArrayEntryProfile)


BaseArray = ArrayBase
