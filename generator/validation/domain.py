"""Model for CANopen objects with common attributes and validation."""

from typing import Annotated, Dict, Literal

from pydantic import Field

from .object_common import HeaderCommon


class Domain(HeaderCommon):
    """Domain object for storing domain-specific data."""

    type: Literal["domain"] = "domain"
    domain: Dict[str, Annotated[int, Field(ge=1)]]
