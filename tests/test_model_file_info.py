"""Unit tests for device_info models."""

import warnings

import pytest
import semver

from generator.validation.baudrate import Baudrate
from generator.validation.device_info import Device, Revision, VendorProduct


def test_vendor_product_defaults_warns():
    """VendorProduct should emit warnings if defaults are used."""
    with warnings.catch_warnings(record=True) as w:
        warnings.simplefilter("always")
        vp = VendorProduct()
        assert vp.name == "Unknown"
        assert vp.number == 0x12345678
        assert any("Vendor name is set to default" in str(warn.message) for warn in w)
        assert any("Vendor number is set to default" in str(warn.message) for warn in w)


def test_vendor_product_custom_values():
    """VendorProduct should accept custom values."""
    vp = VendorProduct(name="Acme Corp", number=1234)
    assert vp.name == "Acme Corp"
    assert vp.number == 1234


def test_revision_from_string():
    """Revision can parse from a version string."""
    rev = Revision.model_validate("2.3.4")
    assert rev.major == 2
    assert rev.minor == 3
    assert rev.patch == 4
    assert rev.to_int() == (2 << 16) | (3 << 8) | 4


def test_revision_from_list():
    """Revision can parse from a list of length 2 or 3."""
    rev2 = Revision.model_validate([1, 5])
    assert rev2.major == 1
    assert rev2.minor == 5
    assert rev2.patch == 0

    rev3 = Revision.model_validate([1, 5, 7])
    assert rev3.major == 1
    assert rev3.minor == 5
    assert rev3.patch == 7


def test_revision_from_dict():
    """Revision can parse from a dict."""
    rev = Revision.model_validate({"major": 3, "minor": 7, "patch": 2})
    assert rev.major == 3
    assert rev.minor == 7
    assert rev.patch == 2


def test_revision_from_versioninfo():
    """Revision can parse from semver.VersionInfo."""
    ver = semver.VersionInfo(4, 2, 1)
    rev = Revision.model_validate(ver)
    assert rev.major == 4
    assert rev.minor == 2
    assert rev.patch == 1


def test_revision_invalid_string():
    """Invalid string raises ValueError."""
    with pytest.raises(ValueError):
        Revision.model_validate("invalid")


def test_revision_invalid_list_length():
    """Invalid list length raises ValueError."""
    with pytest.raises(ValueError):
        Revision.model_validate([1])


def test_revision_invalid_dict_missing_keys():
    """Dict missing required keys raises ValueError."""
    with pytest.raises(ValueError):
        Revision.model_validate({"minor": 3})


def test_revision_invalid_type():
    """Invalid type raises TypeError."""
    with pytest.raises(TypeError):
        Revision.model_validate(42)


def test_device_to_eds_dict():
    """Device converts properly to EDS dictionary."""
    dev = Device(
        vendor=VendorProduct(name="VendorX", number=111),
        product=VendorProduct(name="ProdY", number=222),
        revision=Revision.model_validate("1.2.3"),
        baudrate=Baudrate([125, 500]),
        node_id=10,
        order_code="X-123",
        lss_supported=True,
        simple_bootup_slave=True,
        simple_bootup_master=False,
        granularity=8,
        dynamic_channels_supported=True,
        group_messaging=False,
        rpdo_count=5,
        tpdo_count=3,
    )
    eds = dev.to_eds_dict()

    assert eds["VendorName"] == "VendorX"
    assert eds["VendorNumber"] == 111
    assert eds["ProductName"] == "ProdY"
    assert eds["ProductNumber"] == 222
    assert eds["RevisionNumber"] == (1 << 16) | (2 << 8) | 3
    assert eds["OrderCode"] == "X-123"
    assert eds["LSS_Supported"] == 1
    assert eds["BaudRate_125"] == 1
    assert eds["BaudRate_500"] == 1
    assert eds["BaudRate_10"] == 0
    assert eds["SimpleBootUpSlave"] == 1
    assert eds["SimpleBootUpMaster"] == 0
    assert eds["Granularity"] == 8
    assert eds["DynamicChannelsSupported"] == 1
    assert eds["GroupMessaging"] == 0
    assert eds["NrOfRxPdo"] == 5
    assert eds["NrOfTxPdo"] == 3


def test_device_baudrate_dict_input():
    """Device should parse baudrate from dict input."""
    dev = Device(
        vendor=VendorProduct(name="VendorX", number=123),
        product=VendorProduct(name="ProdY", number=456),
        revision=Revision.model_validate("1.2.3"),
        baudrate={125: True, 250: None, 500: False},
    )
    assert isinstance(dev.baudrate, Baudrate)

    eds = dev.to_eds_dict()
    assert eds["BaudRate_125"] == 1
    assert eds["BaudRate_250"] == 1
    assert eds["BaudRate_500"] == 0


def test_device_invalid_baudrate_type():
    """Device should raise on invalid baudrate type."""
    with pytest.raises(TypeError):
        Device(baudrate="invalid")
