"""YAML reading with structured errors."""

from pathlib import Path
from typing import Union, cast

import yaml


def read_yaml_file(filename: Union[str, Path]) -> dict:
    """Read a YAML file and return its content as a dictionary."""
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"YAML file not found: {filename}")

    try:
        with filepath.open("r", encoding="utf-8") as f:
            data = yaml.safe_load(f) or {}
    except yaml.YAMLError as e:
        msg = f"YAML parsing error in {filename}"
        if hasattr(e, "problem_mark"):
            mark = cast(object, e).problem_mark  # type: ignore[attr-defined]
            msg += f" at line {mark.line + 1}, column {mark.column + 1}"
        raise ValueError(msg) from e

    if not isinstance(data, dict):
        raise ValueError(f"YAML root must be a mapping, got {type(data).__name__}")
    return data
