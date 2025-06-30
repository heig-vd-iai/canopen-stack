"""Various helper functions for validation of objects and enums."""


def validate_identifier(value: str) -> str:
    """Validate that the value is a valid Python identifier.
    Raises ValueError if the value is not a valid identifier.

    >>> validate_identifier("valid_name")
    'valid_name'
    """
    if not value.isidentifier():
        raise ValueError(f"Invalid enum name: {value}. Must be a valid identifier.")
    return value
