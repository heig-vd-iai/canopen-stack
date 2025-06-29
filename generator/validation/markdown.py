"""Model for Markdown validation and conversion to HTML."""

from typing import cast

import mistune
from pydantic_core import core_schema
from pymarkdown.api import PyMarkdownApi


class Markdown(str):
    """
    String subclass representing Markdown with validation and linting.
    """

    def to_html(self) -> str:
        """Convert Markdown to HTML using mistune."""
        renderer = mistune.HTMLRenderer()
        parser = mistune.create_markdown(renderer=renderer)
        return cast(str, parser(self))

    @classmethod
    def validate(cls, value):
        """Validate the input value as Markdown and run linter."""
        if isinstance(value, cls):
            return value

        if not isinstance(value, str):
            raise TypeError("Markdown must be a string.")

        # Check that it parses correctly with mistune
        mistune.create_markdown()(value)

        # Run pymarkdown linter
        api = PyMarkdownApi()
        lint_result = api.scan_string(value)

        if lint_result.pragma_errors:
            errors = [
                f"PRAGMA error in {pe.file_path} at line {pe.line_number}: {pe.pragma_error}"
                for pe in lint_result.pragma_errors
            ]
            raise ValueError("Markdown pragma errors detected:\n" + "\n".join(errors))

        if lint_result.scan_failures:
            errors = [
                f"{sf.rule_id} {sf.rule_name} "
                f"(line {sf.line_number}, col {sf.column_number}) in {sf.scan_file}: "
                f"{sf.extra_error_information or sf.rule_description}"
                for sf in lint_result.scan_failures
            ]
            raise ValueError("Markdown lint errors detected:\n" + "\n".join(errors))

        return cls(value)

    @classmethod
    def __get_pydantic_core_schema__(cls, *_):
        return core_schema.no_info_after_validator_function(
            cls.validate,
            core_schema.str_schema(),
            serialization=core_schema.plain_serializer_function_ser_schema(str),
        )

    def __repr__(self):
        preview = str(self).replace("\n", " ")[:40]
        return f"Markdown({preview!r}...)"
