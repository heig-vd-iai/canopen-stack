"""Bitfield validation and parsing module."""

from typing import Any, Dict, Optional, Tuple, Union

from pydantic import BaseModel, Field, model_validator


class BitRange:
    """
    Helper class to parse and store a bit range like:
        "29" → (29, 29)
        "31..16" → (31, 16)
    """

    def __init__(self, raw_key: Union[str, int]):
        if isinstance(raw_key, int):
            self.start = self.end = raw_key
        elif isinstance(raw_key, str):
            if ".." in raw_key:
                parts = raw_key.split("..")
                if len(parts) != 2 or not all(p.strip().isdigit() for p in parts):
                    raise ValueError(f"Invalid bit range format: '{raw_key}'")
                start, end = map(int, parts)
                if start < end:
                    raise ValueError(
                        f"Invalid bit range '{raw_key}': start must be >= end"
                    )
                self.start = start
                self.end = end
            elif raw_key.strip().isdigit():
                val = int(raw_key.strip())
                self.start = self.end = val
            else:
                raise ValueError(f"Invalid bitfield key: {raw_key}")
        else:
            raise ValueError(f"Invalid bitfield key: {raw_key}")

    @property
    def width(self) -> int:
        """Calculate the width of the bit range."""
        return self.start - self.end + 1

    def as_tuple(self) -> Tuple[int, int]:
        """Return the bit range as a tuple (start, end)."""
        return (self.start, self.end)

    def as_string(self) -> str:
        """Return the bit range as a human-readable string."""
        return (
            f"{self.start}" if self.start == self.end else f"{self.start}..{self.end}"
        )

    def __repr__(self) -> str:
        return f"BitRange({self.as_string()})"


class BitfieldEntry(BaseModel):
    """
    One bitfield entry:
        - name: mandatory string
        - values: optional mapping int → str
    """

    name: str
    values: Dict[int, str] = Field(default_factory=dict)
    width: Optional[int] = Field(default=None, exclude=True)

    @model_validator(mode="after")
    def validate_value_width(self) -> "BitfieldEntry":
        """Validate that all values fit within the specified width."""
        if self.width is not None and self.values:
            max_val = (1 << self.width) - 1
            for k in self.values:
                if k > max_val:
                    raise ValueError(
                        f"Value {k} exceeds width limit {self.width} (max={max_val})"
                    )
        return self


class Bitfield(BaseModel):
    """
    Bitfield mapping bit ranges to entries.
    """

    entries: Dict[Tuple[int, int], BitfieldEntry]

    @model_validator(mode="before")
    @classmethod
    def parse_input(cls, data: Any):
        if isinstance(data, dict):
            if "entries" in data:
                # Si entries est déjà un dict avec des clés tuple, on ne touche à rien
                entries = data["entries"]
                if isinstance(entries, dict):
                    if all(isinstance(k, tuple) for k in entries.keys()):
                        return data
                    # Sinon il faut parser
                    return {"entries": cls._parse_bitfield(entries)}
                else:
                    raise TypeError("entries must be a dict")
            else:
                # dict directement donné, à parser
                return {"entries": cls._parse_bitfield(data)}
        raise TypeError("Bitfield input must be a dict")

    @staticmethod
    def _parse_bitfield(
        data: Dict[Union[str, int], Any],
    ) -> Dict[Tuple[int, int], BitfieldEntry]:
        result = {}
        for raw_key, val in data.items():
            br = BitRange(raw_key)
            width = br.width

            if isinstance(val, str):
                entry = BitfieldEntry(name=val)
            elif isinstance(val, dict):
                entry = BitfieldEntry(**val)
            else:
                raise ValueError(f"Invalid bitfield value for key {raw_key}")

            entry.width = width
            result[br.as_tuple()] = entry
        return result

    def __getitem__(self, key: Tuple[int, int]) -> BitfieldEntry:
        return self.entries[key]

    def __contains__(self, key: Tuple[int, int]) -> bool:
        return key in self.entries

    def items(self):
        return self.entries.items()

    def keys(self):
        return self.entries.keys()

    def values(self):
        return self.entries.values()

    def to_serializable(self) -> Dict[str, Any]:
        output = {}
        for (start, end), entry in self.entries.items():
            key = f"{start}" if start == end else f"{start}..{end}"
            output[key] = entry.model_dump()
        return output

    @model_validator(mode="after")
    def check_no_overlap(self):
        occupied_bits = set()
        for start, end in self.entries.keys():
            for bit in range(end, start + 1):
                if bit in occupied_bits:
                    raise ValueError(f"Bit {bit} is overlapping in multiple ranges.")
                occupied_bits.add(bit)
        return self
