from pytest import raises

from generator.validation.models.validators import validate_identifier


def test_validate_identifier():
    """Test that validate_identifier correctly validates valid identifiers."""
    assert validate_identifier("valid_name") == "valid_name"
    assert validate_identifier("another_valid_name123") == "another_valid_name123"

    with raises(ValueError, match="Invalid enum name: 123invalid"):
        validate_identifier("123invalid")
