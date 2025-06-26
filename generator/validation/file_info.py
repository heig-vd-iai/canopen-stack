"""File information validation module.
This module defines classes for validating device information according to the CiA 306.

FileInfo field is present in the EDS file as follow (example):

[FileInfo]
FileName=vendor1.eds
FileVersion=1                              Uint 8
FileRevision=2                             Uint 8
EDSVersion=4.0                             3 chars [0-9].[0-9]
Description=EDS for simple I/O-device      Max 243 chars.
CreationTime=09:45AM                       hh:mm(AM|PM)
CreationDate=05-15-1995                    mm-dd-yyyy
CreatedBy=Zaphod Beeblebrox                Max 245 chars.
ModificationTime=11:30PM                   hh:mm(AM|PM)
ModificationDate=08-21-1995                mm-dd-yyyy
ModifiedBy=Zaphod Beeblebrox               Max 244 chars.

The FileInfo class encapsulates all the fields in a structured way.
"""
from datetime import datetime

from pydantic import BaseModel, Field


class FileInfo(BaseModel):
    """Information about a file, including metadata."""

    created_by: str = Field(default="unknown", max_length=245)
    created_at: datetime = Field(default_factory=datetime.now)
    modified_by: str = Field(default="unknown", max_length=244)
    modified_at: datetime = Field(default_factory=datetime.now)
    filename: str = Field(default="")
    eds_version: str = Field(default="4.0", max_length=3)
    revision: str = "0.0.0"
