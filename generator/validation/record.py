from typing import ClassVar, List, Literal

from pydantic import model_validator

from . import Access, Datatype
from .object_common import HeaderCommon, VarCommon


class RecordEntry(VarCommon, HeaderCommon):
    """Record entry object for storing subindex data."""

    class Config:
        """Configuration for RecordEntry model."""

        extra = "forbid"


class Record(HeaderCommon):
    """Record object for storing subindex data."""

    type: Literal["record"] = "record"
    record: List[RecordEntry]

    SIZE_ENTRY_NAME: ClassVar[str] = "Number of records"

    @model_validator(mode="after")
    def validate_and_inject_size_entry(self):
        """Validate the length of the record and inject subindex 0 if needed."""

        if len(self.record) > 255:
            raise ValueError("Subindex length must be less than 256.")

        # Check if subindex 0 is present
        if not self.record or self.record[0].name != self.SIZE_ENTRY_NAME:
            size_entry = RecordEntry(
                name=self.SIZE_ENTRY_NAME,
                datatype=Datatype.from_name("uint8"),
                access=Access.model_validate("r"),
                default=len(self.record),
            )
            self.record.insert(0, size_entry)

        else:
            # Update default if already present
            self.record[0].default = len(self.record)

        return self
