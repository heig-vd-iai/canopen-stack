from typing import Literal

from pydantic import ConfigDict

from .object_common import HeaderCommon, VarCommon


class Var(HeaderCommon, VarCommon):
    """Variable object for storing subindex data."""

    type: Literal["var"] = "var"

    model_config = ConfigDict(extra="forbid")
