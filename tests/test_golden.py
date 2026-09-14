import re
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parent.parent
FIXTURES = {
    "minimal": ROOT / "examples" / "minimal.v2.yaml",
    "bootloader": ROOT / "examples" / "bootloader.v2.yaml",
}
GOLDEN = ROOT / "tests" / "golden"

VOLATILE_LINES = [
    re.compile(r"^const uint64_t od_signature = \d+u;$"),
    re.compile(r"^ModificationTime = .*$"),
    re.compile(r"^ModificationDate = .*$"),
]


def generate(outdir: Path, config: Path) -> None:
    cmd = [
        sys.executable,
        "-m",
        "generator",
        "generate",
        str(config),
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


def golden_cases() -> list:
    cases = []
    for name in sorted(FIXTURES):
        golden = GOLDEN / name
        files = sorted(relative_files(golden)) if golden.exists() else []
        cases.extend(
            pytest.param(name, relative, id=f"{name}/{relative}") for relative in files
        )
    return cases


@pytest.fixture(scope="module")
def generated(tmp_path_factory) -> dict:
    outputs = {}
    for name, config in FIXTURES.items():
        outdir = tmp_path_factory.mktemp(f"generated-{name}")
        generate(outdir, config)
        outputs[name] = outdir
    return outputs


@pytest.mark.parametrize("name", sorted(FIXTURES))
def test_golden_files_are_up_to_date(generated: dict, update_golden: bool, name: str):
    golden = GOLDEN / name
    if update_golden:
        shutil.rmtree(golden, ignore_errors=True)
        shutil.copytree(generated[name], golden)
        pytest.skip("golden files updated")

    assert relative_files(generated[name]) == relative_files(golden)


@pytest.mark.parametrize("name, relative", golden_cases())
def test_golden_file_matches(
    generated: dict, update_golden: bool, name: str, relative: Path
):
    if update_golden:
        pytest.skip("golden files updated")
    expected = normalize((GOLDEN / name / relative).read_text())
    actual = normalize((generated[name] / relative).read_text())
    assert actual == expected, f"{name}/{relative} differs from golden output"


@pytest.mark.parametrize("name", sorted(FIXTURES))
def test_generated_code_never_names_the_node(generated: dict, name: str):
    outdir = generated[name]
    offenders = [
        path.relative_to(outdir)
        for path in outdir.rglob("*")
        if path.is_file()
        and path.suffix in {".cpp", ".hpp"}
        and "node." in path.read_text()
    ]
    assert offenders == [], "generated code must not reach the stack singleton"
