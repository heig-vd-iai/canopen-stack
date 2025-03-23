import semver
from pathlib import Path
from datetime import datetime
from collections import Counter
from git import Repo, GitCommandError


class GitInfo:
    """ Get information about a file from a Git repository. """
    def __init__(self, filename: str):
        self.file_path = Path(filename)

        if not self.file_path.exists():
            raise FileNotFoundError(f"File '{filename}' not found.")

        self.repo = Repo(self.file_path, search_parent_directories=True)

        if not self._is_tracked():
            raise RuntimeError("File not tracked by Git.")

        self._version = self._get_version()
        self._authors = self._get_authors()
        self._created_at = self._get_creation_date()
        self._modified_at = self._get_last_modified_date()

    def _is_tracked(self) -> bool:
        return str(self.file_path) in self.repo.git.ls_files(
            self.file_path, error_unmatch=True
        )

    def _get_version(self) -> str | None:
        try:
            tags = self.repo.git.tag("--contains", "HEAD").splitlines()
            for tag in tags:
                try:
                    return str(semver.VersionInfo.parse(tag.strip()))
                except ValueError:
                    continue
        except GitCommandError:
            pass
        return None

    def _get_authors(self) -> list[str]:
        commits = list(self.repo.iter_commits(paths=str(self.file_path)))
        authors = [commit.author.name for commit in commits]
        counter = Counter(authors)
        return [author for author, _ in counter.most_common()]

    def _get_creation_date(self) -> datetime:
        commits = list(self.repo.iter_commits(paths=str(self.file_path), reverse=True))
        if commits:
            return datetime.fromtimestamp(commits[0].committed_date)
        raise RuntimeError("No creation date found.")

    def _get_last_modified_date(self) -> datetime:
        commits = list(self.repo.iter_commits(paths=str(self.file_path)))
        if commits:
            return datetime.fromtimestamp(commits[0].committed_date)
        raise RuntimeError("No last modified date found.")

    @property
    def version(self) -> str | None:
        """ Get the version of the file. """
        return self._version

    @property
    def authors(self) -> list[str]:
        """ Get the authors of the file. """
        return self._authors

    @property
    def created_at(self) -> datetime:
        """ Get the creation date of the file. """
        return self._created_at

    @property
    def modified_at(self) -> datetime:
        """ Get the last modified date of the file. """
        return self._modified_at
