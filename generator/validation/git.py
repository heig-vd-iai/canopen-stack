from pathlib import Path
from typing import Optional

from git import GitCommandError, Repo


class GitInfo:
    """Class to retrieve Git information for a given file."""
    def __init__(self, filepath: str):
        self.filepath = Path(filepath).resolve()
        self.repo = Repo(self.filepath, search_parent_directories=True)

    def author(self) -> Optional[str]:
        """Get the author of the last commit for the file."""
        try:
            commit = next(self.repo.iter_commits(paths=self.filepath, max_count=1))
            return commit.author.name
        except (StopIteration, GitCommandError):
            return None

    def date(self) -> Optional[str]:
        """Get the date of the last commit for the file."""
        try:
            commit = next(self.repo.iter_commits(paths=self.filepath, max_count=1))
            return commit.committed_datetime.isoformat()
        except (StopIteration, GitCommandError):
            return None

    def version(self) -> Optional[str]:
        """Get the version (tag) of the last commit for the file."""
        try:
            return self.repo.git.describe(tags=True, always=True)
        except GitCommandError:
            return None
