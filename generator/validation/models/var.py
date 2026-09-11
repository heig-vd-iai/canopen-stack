"""CANopen variable object model."""

from typing import Literal

from pydantic import ConfigDict

from .object_common import (
    HeaderCommon,
    HeaderCommonProfile,
    VarCommon,
    VarCommonProfile,
)


class Var(HeaderCommon, VarCommon):
    """Variable object for storing subindex data."""

    type: Literal["var"] = "var"

    model_config = ConfigDict(extra="forbid")


class VarProfile(HeaderCommonProfile, VarCommonProfile):
    """Variable object for storing subindex data."""

    type: Literal["var"] = "var"

    model_config = ConfigDict(extra="forbid")
