"""Tests for the Markdown model and its methods."""

# pylint: disable=missing-function-docstring
import pytest

from generator.validation import Markdown


def test_markdown_valid_parses_html():
    md = Markdown("# Title\n\nSome text.")
    html = md.to_html()
    assert "<h1>" in html
    assert "Some text." in html


def test_markdown_validates_and_lints():
    text = "# Good Title\n\nThis is valid markdown.\n"
    md = Markdown.validate(text)
    assert isinstance(md, Markdown)


def test_markdown_invalid_type():
    with pytest.raises(TypeError):
        Markdown.validate(123)


def test_validate_with_non_string():
    """Test that validating a non-string raises TypeError."""
    with pytest.raises(TypeError, match="Markdown must be a string"):
        Markdown.validate(123)


def test_repr_truncation():
    """Test that __repr__ truncates long strings properly."""
    text = "A" * 100
    md = Markdown(text)
    repr_str = repr(md)
    assert repr_str.startswith("Markdown('AAAAAAAA")
    assert repr_str.endswith("...)")
