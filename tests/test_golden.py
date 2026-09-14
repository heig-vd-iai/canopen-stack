import re
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parent.parent
FIXTURES = {
    "minimal": ROOT / "examples" / "minimal.yaml",
    "bootloader": ROOT / "examples" / "bootloader.yaml",
}
CONFIG = FIXTURES["minimal"]
GOLDEN = ROOT / "tests" / "golden" / "minimal"

VOLATILE_LINES = [
    re.compile(r"^const uint64_t od_signature = \d+u;$"),
    re.compile(r"^ModificationTime = .*$"),
    re.compile(r"^ModificationDate = .*$"),
]


def generate(outdir: Path, config: Path = CONFIG) -> None:
    cmd = [
        sys.executable,
        "-m",
        "canopen_generator",
        str(config),
        "-f",
        "--local",
        str(outdir / "cm"),
        "--remote",
        str(outdir / "cpu1"),
        "--eds",
        str(outdir),
        "--doc",
        str(outdir / "docs"),
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


@pytest.mark.parametrize("name", sorted(FIXTURES))
def test_generated_code_never_names_the_node(tmp_path_factory, name: str):
    outdir = tmp_path_factory.mktemp(f"nodefree-{name}")
    generate(outdir, FIXTURES[name])
    offenders = [
        path.relative_to(outdir)
        for path in outdir.rglob("*")
        if path.is_file()
        and path.suffix in {".cpp", ".hpp"}
        and "node." in path.read_text()
    ]
    assert offenders == [], "generated code must not reach the stack singleton"


@pytest.fixture(scope="module")
def generated_bootloader(tmp_path_factory) -> Path:
    outdir = tmp_path_factory.mktemp("generated-bootloader")
    generate(outdir, FIXTURES["bootloader"])
    return outdir


BOOTLOADER_GOLDEN = ROOT / "tests" / "golden" / "bootloader"


def test_bootloader_golden_files_are_up_to_date(
    generated_bootloader: Path, update_golden: bool
):
    if update_golden:
        shutil.rmtree(BOOTLOADER_GOLDEN, ignore_errors=True)
        shutil.copytree(generated_bootloader, BOOTLOADER_GOLDEN)
        pytest.skip("golden files updated")

    assert relative_files(generated_bootloader) == relative_files(BOOTLOADER_GOLDEN)


@pytest.mark.parametrize(
    "relative",
    sorted(relative_files(BOOTLOADER_GOLDEN)) if BOOTLOADER_GOLDEN.exists() else [],
    ids=str,
)
def test_bootloader_golden_file_matches(
    generated_bootloader: Path, update_golden: bool, relative: Path
):
    if update_golden:
        pytest.skip("golden files updated")
    expected = normalize((BOOTLOADER_GOLDEN / relative).read_text())
    actual = normalize((generated_bootloader / relative).read_text())
    assert actual == expected, f"{relative} differs from golden output"
