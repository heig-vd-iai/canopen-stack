"""Device information validation module.
This module defines classes for validating device information according to the CiA 306.

DeviceInfo field is present in the EDS file as follow (example):

[DeviceInfo]
VendorName=Nepp Ltd.                  Max 244 chars.
VendorNumber=156678                   Uint 32
ProductName=E/A 64                    Max 243 chars.
ProductNumber=45570                   Uint 32
RevisionNumber=1                      Uint 32
OrderCode=BUY ME - 177/65/0815        Max 245 chars.
LSS_Supported=0                       Bool
BaudRate_50=1
BaudRate_250=1
BaudRate_500=1
BaudRate_1000=1
SimpleBootUpSlave=1                   Bool
SimpleBootUpMaster=0                  Bool
Granularity=8                         Uint 8
DynamicChannelsSupported=1            Bool
GroupMessaging=0                      Bool
NrOfRxPdo=1                           Uint 16
NrOfTxPdo=2                           Uint 16

The Device class encapsulates all the fields in a structured way.
"""

from typing import Annotated, Any
from warnings import warn

import semver
from pydantic import BaseModel, Field, field_validator, model_validator
from pydantic_core import core_schema


class Baudrate(set):
    """Set of supported baudrates."""

    valid_baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}

    def __init__(self, iterable=()):
        if not all(item in self.valid_baudrates for item in iterable):
            raise ValueError(
                f"Invalid Baudrate: {iterable}. Valid values are: {self.valid_baudrates}"
            )
        super().__init__(item for item in iterable if item in self.valid_baudrates)

    def to_dict(self) -> dict[int, bool]:
        """Convert the Baudrate set to a dictionary."""
        return {b: (b in self) for b in self.valid_baudrates}

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        return core_schema.no_info_after_validator_function(
            cls._validate,
            core_schema.list_schema(items_schema=core_schema.int_schema()),
            serialization=core_schema.plain_serializer_function_ser_schema(list),
        )

    @classmethod
    def _validate(cls, v):
        if isinstance(v, dict):
            # e.g. {125: True, 250: None, 300: False}
            v = [k for k, val in v.items() if val is not False]
        elif not isinstance(v, (list, set, tuple)):
            raise TypeError(f"Invalid Baudrate input: {v}")
        return cls(v)

    def __repr__(self):
        return f"Baudrate({', '.join(map(str, sorted(self)))})"


class VendorProduct(BaseModel):
    """Vendor and product information."""

    name: str = Field(default="Unknown", min_length=1, max_length=244)
    number: int = Field(0x12345678, ge=0, le=0xFFFFFFFF)

    @model_validator(mode="after")
    def check_default_values(self):
        """Check if default values are set and warn the user."""
        if self.name == "Unknown":
            warn(
                "Vendor name is set to default value 'Unknown'. Please set a valid name.",
                UserWarning,
                stacklevel=2,
            )
        if self.number == 0x12345678:
            warn(
                "Vendor number is set to default value 0x12345678. Please set a valid number.",
                UserWarning,
                stacklevel=2,
            )
        return self

class Revision(BaseModel):
    """Revision number of the device."""

    major: Annotated[int, Field(ge=0, le=255)]
    minor: Annotated[int, Field(ge=0, le=255)]
    patch: Annotated[int, Field(ge=0, le=255)] = 0

    @model_validator(mode="before")
    @classmethod
    def parse_revision(cls, v: Any) -> dict:
        """Parse the revision number from various formats."""
        if isinstance(v, str):
            try:
                ver = semver.VersionInfo.parse(v)
            except ValueError as e:
                raise ValueError(f"Invalid version string: {v}. {str(e)}") from e
            return {"major": ver.major, "minor": ver.minor, "patch": ver.patch}

        elif isinstance(v, (list, tuple)):
            if len(v) == 2:
                major, minor = v
                return {"major": major, "minor": minor, "patch": 0}
            elif len(v) == 3:
                major, minor, patch = v
                return {"major": major, "minor": minor, "patch": patch}
            else:
                raise ValueError(
                    "List/tuple must be [major, minor] or [major, minor, patch]."
                )

        elif isinstance(v, dict):
            if "major" in v and "minor" in v:
                return {
                    "major": v["major"],
                    "minor": v["minor"],
                    "patch": v.get("patch", 0),
                }
            raise ValueError("Dict must have at least 'major' and 'minor'.")

        elif isinstance(v, semver.VersionInfo):
            return {"major": v.major, "minor": v.minor, "patch": v.patch}

        raise TypeError(f"Invalid input for Revision: {v} (type: {type(v)})")

    def to_int(self) -> int:
        """Convert the revision to a single integer."""
        return (self.major << 16) | (self.minor << 8) | self.patch


class Device(BaseModel):
    """Device information according to the CiA 306 and CiA 301 standards."""

    vendor: VendorProduct = Field(default_factory=lambda: VendorProduct(number=0))
    product: VendorProduct = Field(default_factory=lambda: VendorProduct(number=0))
    revision: Revision = Field(default_factory=lambda: Revision.model_validate("1.0.0"))
    baudrate: Baudrate
    node_id: int = Field(1, ge=1, le=127)
    order_code: str = Field(default="", max_length=245)
    lss_supported: bool = Field(default=False)
    simple_bootup_slave: bool = Field(default=False)
    simple_bootup_master: bool = Field(default=False)
    granularity: int = Field(8, ge=1, le=255)
    dynamic_channels_supported: bool = Field(default=False)
    group_messaging: bool = Field(default=False)
    rpdo_count: int = Field(1, ge=0, le=255)
    tpdo_count: int = Field(2, ge=0, le=255)

    @field_validator("baudrate", mode="before")
    @classmethod
    def convert_baudrate(cls, v: Any) -> Baudrate:
        """Convert various formats to Baudrate."""
        if isinstance(v, dict):
            # {125: True, 250: None, 300: False}
            return Baudrate(k for k, val in v.items() if val is not False)
        elif isinstance(v, (list, set)):
            return Baudrate(v)
        raise TypeError("Invalid baudrate format: expected dict, list, or set.")

    def to_eds_dict(self) -> dict:
        """Convert the device information to a dictionary suitable for EDS."""
        return {
            "VendorName": self.vendor.name,
            "VendorNumber": self.vendor.number,
            "ProductName": self.product.name,
            "ProductNumber": self.product.number,
            "RevisionNumber": self.revision.to_int(),
            "OrderCode": self.order_code,
            "LSS_Supported": int(self.lss_supported),
            **self.baudrate.to_dict(),
            "SimpleBootUpSlave": int(self.simple_bootup_slave),
            "SimpleBootUpMaster": int(self.simple_bootup_master),
            "Granularity": self.granularity,
            "DynamicChannelsSupported": int(self.dynamic_channels_supported),
            "GroupMessaging": int(self.group_messaging),
            "NrOfRxPdo": self.rpdo_count,
            "NrOfTxPdo": self.tpdo_count,
        }
