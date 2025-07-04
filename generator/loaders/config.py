import warnings
from pathlib import Path
from typing import Dict, List, Union

from pydantic import ValidationError
from pydantic_core import ErrorDetails

from ..validation.models.config import SchemaConfig
from .yaml_config import read_config_file


class ConfigLoader:
    """
    Loads all CANopen profiles from a local directory, validates them,
    and caches the result using pickle if up-to-date.
    """

    def __init__(self, file_name: Path):
        if not file_name.is_file():
            raise FileNotFoundError(f"Configuration file not found: {file_name}")
        self.file_name = file_name
        self.warnings: Dict[int, List[str]] = {}
        self.errors: Dict[int, Union[List[ErrorDetails], str]] = {}

        raw = read_config_file(self.file_name)
        try:
            with warnings.catch_warnings(record=True) as w:
                warnings.simplefilter("always")
                config = SchemaConfig.model_validate(raw)

            self.warnings = [str(warn.message) for warn in w]

        except ValidationError as e:
            self.errors = e.errors()

        if self.errors:
            return self.errors

        self._save_to_cache()
        return {}

    def reload(self) -> dict[int, list[ErrorDetails] | str]:
        """Load config file and validate it."""
        self.profiles.clear()
        self.warnings.clear()
        self.errors.clear()
