"""Types definitions according to the CiA 301 specification."""

import re
from typing import Any, Dict, Optional, Tuple

from pydantic import BaseModel, PrivateAttr, model_validator
from pydantic_core import core_schema


class BitfieldEntry(BaseModel):
    """Bitfield entry for storing bit ranges and their names."""

    name: str
    values: Dict[int, str] = {}
    _width: Optional[int] = PrivateAttr(default=None)

    @model_validator(mode="before")
    @classmethod
    def check_values_fit_width(cls, data):
        width = data.get("_width")
        if width is not None and "values" in data:
            max_val = (1 << width) - 1
            for k in data["values"]:
                if k > max_val:
                    raise ValueError(
                        f"Value {k} exceeds width limit {width} (max={max_val})"
                    )
        return data


class Bitfield(Dict[Tuple[int, int], BitfieldEntry]):
    """Bitfield representation for storing bit ranges and their names."""

    _width: Optional[int] = PrivateAttr(default=None)

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        def validate(v: Any) -> "Bitfield":
            if not isinstance(v, dict):
                raise TypeError("Bitfield must be a dict")
            result = {}
            for key, val in v.items():
                # Normalize key
                if isinstance(key, int):
                    start, end = key, key
                elif isinstance(key, str) and re.match(r"^\d+\.\.\d+$", key):
                    start, end = map(int, key.split(".."))
                    if start < end:
                        raise ValueError(
                            f"Invalid bit range '{key}': start must be >= end"
                        )
                else:
                    raise ValueError(f"Invalid bitfield key: {key}")

                width = start - end + 1

                # Normalize value
                if isinstance(val, str):
                    entry = BitfieldEntry(name=val, values={})
                elif isinstance(val, dict):
                    entry = BitfieldEntry(**val)
                else:
                    raise TypeError(
                        f"Invalid bitfield entry value for key {key}: {val}"
                    )

                # Inject width dynamically to perform value checks
                entry._width = width
                entry = BitfieldEntry.model_validate(entry.model_dump())  # re-validate
                result[(start, end)] = entry
            return cls(result)

        return core_schema.no_info_after_validator_function(
            validate,
            core_schema.dict_schema(
                keys_schema=core_schema.any_schema(),
                values_schema=core_schema.any_schema(),
            ),
            serialization=core_schema.plain_serializer_function_ser_schema(
                lambda v: {
                    f"{k[0] if k[0] == k[1] else f'{k[0]}..{k[1]}'}": e.model_dump()
                    for k, e in v.items()
                }
            ),
        )
