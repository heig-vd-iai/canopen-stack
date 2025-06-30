from warnings import catch_warnings, simplefilter

import pytest
import semver

from generator.validation.models.device_info import (
    Baudrate,
    Device,
    Revision,
    VendorProduct,
)


def test_baudrate_valid():
    b = Baudrate([125, 250])
    assert 125 in b
    assert 500 not in b
    assert b.to_dict()[125] is True
    assert b.to_dict()[500] is False


def test_baudrate_invalid():
    with pytest.raises(ValueError):
        Baudrate([300, 700])  # Not valid in CiA 301


def test_baudrate_from_dict():
    d = {125: True, 250: None, 300: False}
    b = Baudrate._validate(d)
    assert isinstance(b, Baudrate)
    assert 125 in b
    assert 300 not in b


def test_baudrate_repr():
    b = Baudrate([250, 125])
    assert repr(b) == "Baudrate(125, 250)"


# ---------- VendorProduct Tests ----------


def test_vendor_product_defaults_warn():
    with catch_warnings(record=True) as w:
        simplefilter("always")
        vp = VendorProduct()
        assert vp.name == "Unknown"
        assert vp.number == 0x12345678
        assert len(w) == 2
        assert "Vendor name is set to default" in str(w[0].message)


def test_vendor_product_custom():
    vp = VendorProduct(name="CiA", number=0xABCD)
    assert vp.name == "CiA"
    assert vp.number == 0xABCD


# ---------- Revision Tests ----------


def test_revision_from_string():
    r = Revision.model_validate("1.2.3")
    assert r.major == 1 and r.minor == 2 and r.patch == 3


def test_revision_from_list():
    r = Revision.model_validate([2, 4])
    assert r.major == 2 and r.minor == 4 and r.patch == 0


def test_revision_from_dict():
    r = Revision.model_validate({"major": 1, "minor": 9})
    assert r.major == 1 and r.minor == 9 and r.patch == 0


def test_revision_from_semver():
    s = semver.VersionInfo.parse("3.1.4")
    r = Revision.model_validate(s)
    assert r.major == 3 and r.minor == 1 and r.patch == 4


def test_revision_invalid_format():
    with pytest.raises(TypeError):
        Revision.model_validate(1234)


def test_device_full_init():
    device = Device.model_validate(
        {
            "vendor": {"name": "Company", "number": 1},
            "product": {"name": "Product", "number": 2},
            "revision": "1.0.1",
            "baudrate": [125, 250, 500],
            "node_id": 32,
        }
    )

    assert device.vendor.name == "Company"
    assert device.product.number == 2
    assert device.revision.patch == 1
    assert 125 in device.baudrate
    assert device.node_id == 32


def test_device_invalid_baudrate_type():
    with pytest.raises(TypeError):
        Device.model_validate(
            {
                "vendor": {},
                "product": {},
                "revision": "1.0.0",
                "baudrate": "wrong_format",
                "node_id": 1,
            }
        )


def test_device_default_values():
    device = Device.model_validate(
        {
            "baudrate": [125],
        }
    )
    assert device.vendor.name == "Unknown"
    assert device.product.number == 0
    assert isinstance(device.revision, Revision)
    assert 125 in device.baudrate
    assert device.node_id == 1
