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
    """Variable object: a single value at sub-index 0."""

    type: Literal["var"] = "var"

    model_config = ConfigDict(extra="forbid")


class VarProfile(HeaderCommonProfile, VarCommonProfile):
    """Variable object defined in a CiA profile."""

    type: Literal["var"] = "var"

    model_config = ConfigDict(extra="forbid")
