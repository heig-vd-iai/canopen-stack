"""Tests for object models"""

# pylint: disable=missing-function-docstring

from generator.validation import Domain


def test_domain_creation():
    dom = Domain(
        name="FirmwareDomain",
        domain={
            "StartAddress": 100,
            "EndAddress": 200,
        },
    )
    assert dom.type == "domain"
    assert "StartAddress" in dom.domain
    assert dom.domain["EndAddress"] == 200
