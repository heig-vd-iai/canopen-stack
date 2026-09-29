"""Case conversion filters available to the templates."""

import pytest

from generator.render.filters import camel, pascal


@pytest.mark.parametrize(
    "name, expected",
    [
        ("PROFILE_POSITION", "profilePosition"),
        ("NONE", "none"),
        ("SEARCH_LIMIT", "searchLimit"),
        ("", ""),
    ],
)
def test_camel(name, expected):
    assert camel(name) == expected


@pytest.mark.parametrize(
    "name, expected",
    [
        ("PROFILE_POSITION", "ProfilePosition"),
        ("NONE", "None"),
        ("SEARCH_LIMIT", "SearchLimit"),
        ("", ""),
    ],
)
def test_pascal(name, expected):
    assert pascal(name) == expected
