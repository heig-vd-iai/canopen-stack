"""Extracts Git information for a given file."""

import re
import warnings
from collections import Counter
from dataclasses import dataclass, field
from datetime import datetime as DateTime
from pathlib import Path
from typing import List, Optional

from git import Commit, GitCommandError, Repo
from semver import VersionInfo

SEMVER_REGEX = re.compile(r"v?(\d+\.\d+\.\d+)(?:-(\d+)-g([0-9a-f]+))?")


@dataclass(frozen=True, slots=True)
class GitInfo:
    """Extracts and provides Git information for a given file."""

    filepath: Path
    authors: List[str] = field(init=False)
    author_creation: Optional[str] = field(init=False)
    date_creation: Optional[DateTime] = field(init=False)
    date_modification: Optional[DateTime] = field(init=False)
    version: VersionInfo = field(init=False)
    digest: str = field(init=False)
    repo: Repo = field(init=False, repr=False)

    def __post_init__(self):
        object.__setattr__(
            self, "repo", Repo(self.filepath, search_parent_directories=True)
        )
        if self.repo.bare:
            raise ValueError(f"Not a Git repo: {self.filepath}")
        last = self._last_commit()
        object.__setattr__(self, "digest", last.hexsha if last else "")
        object.__setattr__(self, "authors", self._authors_by_contribution())
        object.__setattr__(self, "date_creation", self._date_created())
        object.__setattr__(self, "date_modification", self._date_modified())
        object.__setattr__(self, "version", self._version())
        object.__setattr__(self, "author_creation", self._author_creation())

    def _last_commit(self) -> Optional[Commit]:
        try:
            return next(self.repo.iter_commits(paths=str(self.filepath), max_count=1))
        except StopIteration:
            return None

    def _first_commit(self) -> Optional[Commit]:
        try:
            return next(
                self.repo.iter_commits(
                    paths=str(self.filepath), max_count=1, reverse=True
                )
            )
        except StopIteration:
            return None

    def _commit_count(self) -> int:
        try:
            return sum(1 for _ in self.repo.iter_commits(paths=str(self.filepath)))
        except GitCommandError:
            return 0

    def _author_creation(self) -> Optional[str]:
        commit = self._first_commit()
        return commit.author.name if commit and commit.author else None

    def _authors_by_contribution(self) -> List[str]:
        try:
            commits = self.repo.iter_commits(paths=str(self.filepath))
            counts = Counter(
                c.author.name for c in commits if c.author and c.author.name
            )
            return [name for name, _ in counts.most_common()]
        except GitCommandError:
            return []

    def _date_modified(self) -> Optional[DateTime]:
        commit = self._last_commit()
        return commit.committed_datetime if commit else None

    def _date_created(self) -> Optional[DateTime]:
        commit = self._first_commit()
        return commit.committed_datetime if commit else None

    def _version(self) -> VersionInfo:
        fallback = VersionInfo(0, 0, self._commit_count(), build=self.digest[:7])
        try:
            raw = self.repo.git.describe(tags=True, match="v[0-9]*.[0-9]*.[0-9]*")
        except GitCommandError:
            return fallback

        m = SEMVER_REGEX.match(raw)
        if not m:
            warnings.warn(f"Bad tag '{raw}'", stacklevel=2)
            return fallback

        base = m.group(1)
        try:
            return VersionInfo.parse(base)
        except ValueError:
            warnings.warn(f"Invalid format '{base}'", stacklevel=2)
            return fallback
