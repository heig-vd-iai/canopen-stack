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


def test_markdown_detects_lint_error():
    bad_text = "#TitleWithoutSpace\nText"
    with pytest.raises(ValueError) as e:
        Markdown.validate(bad_text)
    assert "MD018" in str(e.value) or "MD019" in str(e.value)


class DummyScanFailure:
    def __init__(
        self,
        rule_id,
        rule_name,
        line_number,
        column_number,
        scan_file,
        extra_error_information=None,
        rule_description="Some rule description",
    ):
        self.rule_id = rule_id
        self.rule_name = rule_name
        self.line_number = line_number
        self.column_number = column_number
        self.scan_file = scan_file
        self.extra_error_information = extra_error_information
        self.rule_description = rule_description


def test_validate_with_non_string():
    """Test that validating a non-string raises TypeError."""
    with pytest.raises(TypeError, match="Markdown must be a string"):
        Markdown.validate(123)


def test_validate_with_markdown_errors(monkeypatch):
    """Test that validation raises ValueError when lint errors remain."""

    dummy_failure = DummyScanFailure(
        rule_id="MD999",
        rule_name="Dummy rule",
        line_number=1,
        column_number=1,
        scan_file="test.md",
        extra_error_information="Dummy error",
    )

    class DummyLintResult:
        scan_failures = [dummy_failure]

    # Patch PyMarkdownApi
    import generator.validation.models.markdown as md_module

    monkeypatch.setattr(
        md_module, "PyMarkdownApi", lambda: DummyPyMarkdownApi(DummyLintResult())
    )

    class DummyPyMarkdownApi:
        def __init__(self, result):
            self._result = result

        def scan_string(self, _):
            return self._result

    with pytest.raises(ValueError) as exc:
        Markdown.validate("# heading")

    assert "MD999 Dummy rule" in str(exc.value)
    assert "line 1" in str(exc.value)


def test_repr_truncation():
    """Test that __repr__ truncates long strings properly."""
    text = "A" * 100
    md = Markdown(text)
    repr_str = repr(md)
    assert repr_str.startswith("Markdown('AAAAAAAA")
    assert repr_str.endswith("...)")


def test_trailing_newline_fix(monkeypatch):
    """Test that MD047 triggers automatic trailing newline fix."""

    # Create a lint result that only includes MD047
    dummy_failure = DummyScanFailure(
        rule_id="MD047",
        rule_name="Single trailing newline",
        line_number=1,
        column_number=1,
        scan_file="test.md",
    )

    class DummyLintResult:
        scan_failures = [dummy_failure]

    class DummyPyMarkdownApi:
        def scan_string(self, _):
            return DummyLintResult()

    import generator.validation.models.markdown as md_module

    monkeypatch.setattr(md_module, "PyMarkdownApi", lambda: DummyPyMarkdownApi())

    input_md = "# heading"
    result = Markdown.validate(input_md)
    assert isinstance(result, Markdown)
    assert str(result).endswith("\n")
