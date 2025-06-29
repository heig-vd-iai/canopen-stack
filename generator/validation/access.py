"""Model for CANopen-style access rights."""
from typing import Any

from pydantic import BaseModel, model_validator


class Access(BaseModel):
    """Object access rights (CANopen-style: r/w/rw)."""

    read: bool = False
    write: bool = False

    def __str__(self):
        return (
            "rw"
            if self.read and self.write
            else "r"
            if self.read
            else "w"
            if self.write
            else ""
        )

    def __repr__(self):
        return f"Access(read={self.read}, write={self.write})"

    def to_eds(self) -> str:
        """Convert access rights to EDS format."""
        return (
            "rw"
            if self.read and self.write
            else "ro"
            if self.read
            else "wo"
            if self.write
            else ""
        )

    @model_validator(mode="before")
    @classmethod
    def _coerce(cls, v: Any) -> Any:
        if isinstance(v, cls):
            return v
        if isinstance(v, str):
            return {"read": "r" in v, "write": "w" in v}
        if isinstance(v, dict):
            return {
                "read": bool(v.get("read", False)),
                "write": bool(v.get("write", False)),
            }
        raise TypeError(f"Cannot parse Access from {v!r}")
