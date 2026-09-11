import re
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "examples" / "minimal.v2.yaml"
GOLDEN = ROOT / "tests" / "golden" / "minimal"

VOLATILE_LINES = [
    re.compile(r"^const uint64_t od_signature = \d+u;$"),
    re.compile(r"^ModificationTime = .*$"),
    re.compile(r"^ModificationDate = .*$"),
]


def generate(outdir: Path) -> None:
    cmd = [
        sys.executable,
        "-m",
        "generator",
        "generate",
        str(CONFIG),
        "-f",
        "--all",
        str(outdir),
    ]
    subprocess.run(cmd, cwd=ROOT, check=True, capture_output=True)


def normalize(text: str) -> str:
    lines = []
    for line in text.splitlines():
        if any(pattern.match(line) for pattern in VOLATILE_LINES):
            lines.append("<volatile>")
        else:
            lines.append(line)
    return "\n".join(lines)


def relative_files(root: Path) -> set[Path]:
    return {path.relative_to(root) for path in root.rglob("*") if path.is_file()}


@pytest.fixture(scope="module")
def generated(tmp_path_factory) -> Path:
    outdir = tmp_path_factory.mktemp("generated")
    generate(outdir)
    return outdir


def test_golden_files_are_up_to_date(generated: Path, update_golden: bool):
    if update_golden:
        shutil.rmtree(GOLDEN, ignore_errors=True)
        shutil.copytree(generated, GOLDEN)
        pytest.skip("golden files updated")

    assert relative_files(generated) == relative_files(GOLDEN)


@pytest.mark.parametrize(
    "relative",
    sorted(relative_files(GOLDEN)) if GOLDEN.exists() else [],
    ids=str,
)
def test_golden_file_matches(generated: Path, update_golden: bool, relative: Path):
    if update_golden:
        pytest.skip("golden files updated")
    expected = normalize((GOLDEN / relative).read_text())
    actual = normalize((generated / relative).read_text())
    assert actual == expected, f"{relative} differs from golden output"
