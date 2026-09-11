"""Model for Markdown text."""

from typing import cast

import mistune
from pydantic_core import core_schema


class Markdown(str):
    """String subclass carrying Markdown text."""

    def to_html(self) -> str:
        """Convert Markdown to HTML using mistune."""
        renderer = mistune.HTMLRenderer()
        parser = mistune.create_markdown(renderer=renderer)
        return cast(str, parser(self))

    @classmethod
    def validate(cls, value):
        """Validate that the value is a string and parses as Markdown."""
        if isinstance(value, cls):
            return value

        if not isinstance(value, str):
            raise TypeError("Markdown must be a string.")

        mistune.create_markdown()(value)
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
