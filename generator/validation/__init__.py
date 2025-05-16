"""Schema validation for configuration files."""
import warnings
from datetime import datetime
from pathlib import Path
from typing import Dict

import yaml

from .git import GitInfo
from .profile import Profile, SchemaProfile


def read_config(filename: str) -> dict:
    """Read a configuration file and return its content as a dictionary."""
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"Configuration file not found: {filename}")

    with filepath.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}

    info = data.setdefault("info", {})

    git_info = GitInfo(filename)

    # created_by
    if "created_by" not in info:
        author = git_info.author or "unknown"
        info["created_by"] = author
        warnings.warn(
            f"'/info/created_by' not specified. Using Git author or fallback: {author}",
            stacklevel=2,
        )

    if "created_at" not in info:
        if git_info.date is not None:
            info["created_at"] = git_info.date
        else:
            fallback = datetime.now().isoformat()
            info["created_at"] = fallback
            warnings.warn(
                f"'/info/created_at' not specified. Using current time: {fallback}",
                stacklevel=2,
            )

    if "modified_by" not in info:
        info["modified_by"] = info.get("created_by", "unknown")

    if "modified_at" not in info:
        info["modified_at"] = datetime.now().isoformat()

    if "filename" not in info:
        info["filename"] = str(filepath.name)

    if "revision" not in info:
        rev = git_info.version or "0.1.0"
        info["revision"] = rev
        warnings.warn(
            f"'/info/revision' not specified. Using Git tag or fallback: {rev}",
            stacklevel=2,
        )

    return data


def read_profile(profile: int) -> dict:
    """Read a profile configuration file and return its content as a dictionary."""
    filename = f"generator/profiles/{profile}.yaml"
    filepath = Path(filename)

    if not filepath.exists():
        raise FileNotFoundError(f"Configuration file not found: {filename}")

    with filepath.open("r", encoding="utf-8") as f:
        data = yaml.safe_load(f) or {}

    return data


def merge_profiles(*schemas: SchemaProfile) -> SchemaProfile:
    """Merge multiple SchemaProfile instances into one."""
    merged_profiles: Dict[int, Profile] = {}

    for schema in schemas:
        for pid, profile in schema.profiles.items():
            if pid in merged_profiles:
                raise ValueError(f"Duplicate profile {pid} encountered during merge.")
            merged_profiles[pid] = profile

    return SchemaProfile(profiles=merged_profiles)
