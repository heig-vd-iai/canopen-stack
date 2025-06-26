"""Schema validation for configuration files."""

from datetime import datetime
from pathlib import Path
from typing import cast

import yaml

from ..validation.file_info import FileInfo
from .git import GitInfo


def get_file_info_from_scm(filename: str) -> FileInfo:
    """Get file information including creation and modification details."""
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"File not found: {filename}")

    git_info = GitInfo(filepath)

    return FileInfo(
        created_by=git_info.author_creation or "unknown",
        created_at=(
            git_info.date_creation if git_info.date_creation else datetime.now()
        ),
        modified_by=git_info.authors[0] if git_info.authors else "unknown",
        modified_at=datetime.now(),
        filename=filepath.name,
        revision=str(git_info.version),
    )


def read_yaml_file(filename: str) -> dict:
    """Read a YAML file and return its content as a dictionary.
    Raises a structured error if parsing fails.
    """
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"YAML file not found: {filename}")

    try:
        with filepath.open("r", encoding="utf-8") as f:
            data = yaml.safe_load(f) or {}
            if not isinstance(data, dict):
                raise ValueError(
                    f"YAML root must be a mapping, got {type(data).__name__}"
                )
            return data
    except yaml.YAMLError as e:
        msg = f"YAML parsing error in {filename}"
        if hasattr(e, "problem_mark"):
            mark = cast(object, e).problem_mark  # type: ignore[attr-defined]
            msg += f" at line {mark.line + 1}, column {mark.column + 1}"
        raise ValueError(msg) from e


def read_config_file(filename: str) -> dict:
    """Read a configuration file and return its content as a dictionary."""
    data = read_yaml_file(filename)
    info = get_file_info_from_scm(filename)

    if "info" in data:
        raise ValueError(f"File '{filename}' already contains 'info' section.")

    data["info"] = dict(info)
    return data


def read_profile(profile: int) -> dict:
    """Read a profile configuration file and return its content as a dictionary."""
    filename = f"generator/profiles/{profile}.yaml"
    return read_config_file(filename)
