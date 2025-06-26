from typing import Any, Dict, Optional, Tuple, Union
from pydantic import BaseModel, Field, field_validator, model_validator


class BitfieldEntry(BaseModel):
    """Bitfield entry for storing bit ranges and their names."""
    name: str
    values: Dict[int, str] = {}
    _width: Optional[int] = None

    @model_validator(mode="after")
    def validate_value_width(self) -> "BitfieldEntry":
        if self._width is not None:
            max_val = (1 << self._width) - 1
            for k in self.values:
                if k > max_val:
                    raise ValueError(
                        f"Value {k} exceeds width limit {self._width} (max={max_val})"
                    )
        return self


class Bitfield(BaseModel):
    """Bitfield wrapper mapping ranges to entries."""
    entries: Dict[Tuple[int, int], BitfieldEntry]

    @field_validator("entries", mode="before")
    @classmethod
    def parse_keys_and_entries(cls, v: Dict[Union[str, int], Any]) -> Dict[Tuple[int, int], BitfieldEntry]:
        result = {}
        for raw_key, val in v.items():
            if isinstance(raw_key, int):
                start = end = raw_key
            elif isinstance(raw_key, str):
                if ".." in raw_key:
                    parts = raw_key.split("..")
                    if len(parts) != 2 or not all(p.isdigit() for p in parts):
                        raise ValueError(f"Invalid bit range format: '{raw_key}'")
                    start, end = map(int, parts)
                    if start < end:
                        raise ValueError(f"Invalid bit range '{raw_key}': start must be >= end")
                elif raw_key.isdigit():
                    start = end = int(raw_key)
                else:
                    raise ValueError(f"Invalid bitfield key: {raw_key}")
            else:
                raise ValueError(f"Unsupported key type: {raw_key}")

            width = start - end + 1
            if isinstance(val, str):
                entry = BitfieldEntry(name=val)
            elif isinstance(val, dict):
                entry = BitfieldEntry(**val)
            else:
                raise ValueError(f"Invalid bitfield value for key {raw_key}")

            entry._width = width
            result[(start, end)] = entry
        return result

    def to_serializable(self) -> Dict[str, Any]:
        """Serialize keys as strings again for export."""
        output = {}
        for (start, end), entry in self.entries.items():
            key = f"{start}" if start == end else f"{start}..{end}"
            output[key] = entry.model_dump()
        return output
