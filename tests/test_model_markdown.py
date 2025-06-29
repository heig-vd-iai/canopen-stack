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


def test_markdown_detects_lint_error():
    bad_text = "#TitleWithoutSpace\nText"
    with pytest.raises(ValueError) as e:
        Markdown.validate(bad_text)
    assert "MD018" in str(e.value) or "MD019" in str(e.value)
