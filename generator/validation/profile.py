"""Validation schema for the CiA profiles definitions."""

import pickle
import re
import warnings
from pathlib import Path
from typing import Annotated, Any, Dict, List, Literal, Optional, Tuple, Union

import yaml
from pydantic import (
    BaseModel,
    Field,
    RootModel,
    ValidationError,
    field_validator,
    model_validator,
)

from .config import (
    ArrayEntry,
    BaseArray,
    EnumData,
    MappingRootMixin,
    Markdown,
    Record,
    RecordEntry,
    Var,
)
from .helpers import validate_identifier

PROFILE_DIR = Path(__file__).parent.parent / "profiles"

class EnumDataProfile(EnumData):
    """Enum data with additional profile information."""

    override_ranges: List[Tuple[int, int]] = Field(default_factory=list)


class ProfileExtra(BaseModel):
    """Base class for profile extra information."""

    category: Literal["conditional", "optional", "mandatory"] = Field(
        default="optional"
    )
    mandatory_conditions: List[str] = Field(default_factory=list)


class VarProfile(ProfileExtra, Var):
    """Variable profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class ArrayEntryProfile(ArrayEntry):
    """Array entry profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class ArrayProfile(BaseArray):
    """Array profile with additional information."""

    data: List[ArrayEntryProfile] = []


class RecordEntryProfile(ProfileExtra, RecordEntry):
    """Record entry profile with additional information."""

    enum: Optional[EnumDataProfile] = None


class RecordProfile(ProfileExtra, Record):
    """Record profile with additional information."""

    record: List[RecordEntryProfile] = []


ObjectTypeProfile = Annotated[
    Union[VarProfile, ArrayProfile, RecordProfile], Field(discriminator="type")
]


class ObjectsProfile(
    MappingRootMixin[ObjectTypeProfile], RootModel[Dict[int, ObjectTypeProfile]]
):
    """Profile for objects with inheritance and type resolution."""

    @model_validator(mode="before")
    @classmethod
    def resolve_inheritance_and_types(cls, v: Any) -> Any:
        """Resolve inheritance and infer types for objects."""
        if not isinstance(v, dict):
            raise TypeError("Expected a dict for objects")

        # Étape 1 : Résolution de l’héritage
        def resolve(idx: int, visited: set[int]) -> dict:
            obj = v[idx]
            inherit_id = obj.get("inherit")
            if not inherit_id:
                return obj
            if inherit_id not in v:
                raise ValueError(
                    f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h"
                )
            if inherit_id in visited:
                raise ValueError(f"Circular inheritance detected at {idx:04X}h")
            base = resolve(inherit_id, visited | {idx})
            merged = {**base, **obj}
            return merged

        v = {idx: resolve(idx, set()) for idx in v}

        for obj in v.values():
            if "type" not in obj:
                if "length" in obj or "data" in obj:
                    obj["type"] = "array"
                elif "record" in obj:
                    obj["type"] = "record"
                else:
                    obj["type"] = "var"

        return v


class Profile(BaseModel):
    """Profile for a specific CiA profile."""

    name: str
    description: Markdown = Field(default=Markdown(""))
    standards: List[str] = []
    features: Dict[str, str] = {}
    objects: ObjectsProfile

    @field_validator("features", mode="before")
    @classmethod
    def validate_enum_keys(cls, v):
        """Validate that all keys in the features dictionary are valid identifiers."""
        return {validate_identifier(k): val for k, val in v.items()}


class Profiles(BaseModel):
    """Schema for the CiA profiles."""

    profiles: Dict[Annotated[int, Field(ge=300, le=500)], Profile]


class ProfileLoader:
    """
    Loads all CANopen profiles from a local directory, validates them,
    and caches the result using pickle if up-to-date.
    """

    def __init__(self, profile_dir: Path = None, cache_path: Optional[Path] = None):
        self.profile_dir = profile_dir or PROFILE_DIR
        self.cache_path = cache_path or self.profile_dir / ".cache_profiles.pkl"

        self.profiles: Dict[int, "Profiles"] = {}
        self.warnings: Dict[int, List[str]] = {}
        self.errors: Dict[int, str] = {}

    def load_all(self, use_cache: bool = True) -> None:
        """Load and validate all profiles. Use cache if possible."""
        if use_cache and self._load_from_cache():
            return

        yaml_pattern = re.compile(r"^(\d+)\.yaml$")
        self.profiles.clear()
        self.warnings.clear()
        self.errors.clear()

        for file in self.profile_dir.glob("*.yaml"):
            if not file.is_file():
                continue
            match = yaml_pattern.match(file.name)
            if not match:
                continue

            profile_id = int(match.group(1))

            try:
                with file.open("r", encoding="utf-8") as f:
                    raw = yaml.safe_load(f) or {}

                with warnings.catch_warnings(record=True) as w:
                    warnings.simplefilter("always")
                    profile = Profiles.model_validate(raw)

                self.profiles[profile_id] = profile
                self.warnings[profile_id] = [str(warn.message) for warn in w]

            except ValidationError as e:
                self.errors[profile_id] = f"Validation error: {e}"
            except Exception as e:
                self.errors[profile_id] = f"Exception while loading: {e}"

        self._save_to_cache()

    def _load_from_cache(self) -> bool:
        """Try to load from cache if up-to-date."""
        if not self.cache_path.exists():
            return False

        cache_mtime = self.cache_path.stat().st_mtime
        for file in self.profile_dir.glob("*.yaml"):
            if file.stat().st_mtime > cache_mtime:
                return False  # A file is newer than cache

        try:
            with self.cache_path.open("rb") as f:
                cache = pickle.load(f)
            self.profiles = cache["profiles"]
            self.warnings = cache["warnings"]
            self.errors = cache["errors"]
            return True
        except Exception:
            return False

    def _save_to_cache(self) -> None:
        """Save validated data to cache."""
        data = {
            "profiles": self.profiles,
            "warnings": self.warnings,
            "errors": self.errors,
        }
        try:
            with self.cache_path.open("wb") as f:
                pickle.dump(data, f)
        except Exception as e:
            print(f"Warning: failed to write profile cache: {e}")

    def summary(self) -> str:
        """Return a textual summary of all loaded profiles."""
        lines = [f"Loaded {len(self.profiles)} profile(s)."]

        if self.warnings:
            lines.append("\n=== WARNINGS ===")
            for pid, warns in self.warnings.items():
                lines.append(f"- Profile {pid}:")
                for w in warns:
                    lines.append(f"    * {w}")

        if self.errors:
            lines.append("\n=== ERRORS ===")
            for pid, err in self.errors.items():
                lines.append(f"- Profile {pid}: {err}")

        return "\n".join(lines)
