"""EDS [FileInfo] section, CiA 306."""

from datetime import datetime
from typing import Optional

from pydantic import BaseModel, ConfigDict, Field


class Info(BaseModel):
    """Information about the configuration file itself."""

    model_config = ConfigDict(extra="forbid")

    description: str = Field(default="", max_length=243)
    file_version: int = Field(default=1, ge=0, le=255)
    file_revision: int = Field(default=0, ge=0, le=255)
    created_by: str = Field(default="", max_length=245)
    created_at: Optional[datetime] = None
    modified_by: str = Field(default="", max_length=244)
