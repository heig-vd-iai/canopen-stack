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
        """Validate the input value as Markdown and run linter, allowing some relaxed rules."""
        if isinstance(value, cls):
            return value

        if not isinstance(value, str):
            raise TypeError("Markdown must be a string.")

        # Ensure it parses as Markdown
        mistune.create_markdown()(value)

        api = PyMarkdownApi()
        lint_result = api.scan_string(value)

        # We will store whether we modified the string
        modified_value = value

        # Ignore MD041 (first-line-heading) entirely
        filtered_scan_failures = []
        for sf in lint_result.scan_failures:
            if sf.rule_id == "MD041":
                continue # Ignore first-line-heading issues
            elif sf.rule_id == "MD013":
                continue # Ignore line length issues
            elif sf.rule_id == "MD047":
                # Automatically fix missing trailing newline
                if not modified_value.endswith("\n"):
                    modified_value += "\n"
            else:
                filtered_scan_failures.append(sf)

        if filtered_scan_failures:
            errors = [
                f"{sf.rule_id} {sf.rule_name} "
                f"(line {sf.line_number}, col {sf.column_number}) in {sf.scan_file}: "
                f"{sf.extra_error_information or sf.rule_description}"
                for sf in filtered_scan_failures
            ]
            raise ValueError("Markdown lint errors detected:\n" + "\n".join(errors))

        # If we modified the value (e.g. added trailing newline), check that it parses again
        if modified_value != value:
            mistune.create_markdown()(modified_value)

        return cls(modified_value)

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
