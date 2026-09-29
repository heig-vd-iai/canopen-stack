"""Jinja filters shared by the generator and project templates."""


def pascal(name: str) -> str:
    """Convert an UPPER_SNAKE name to PascalCase."""
    return "".join(word.capitalize() for word in name.split("_") if word)


def camel(name: str) -> str:
    """Convert an UPPER_SNAKE name to camelCase."""
    converted = pascal(name)
    return converted[:1].lower() + converted[1:]
