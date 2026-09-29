"""Project templates passed with --templates are rendered under --remote."""

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "examples" / "minimal.v2.yaml"

CUSTOM_TEMPLATE = """\
{% for item in modes_of_operation.enum.values %}
{% set name = item.split("__", 1)[1] %}
{{ item }} {{ name|camel }} {{ name|pascal }}
{% endfor %}
"""

EXPECTED = """\
OPERATION_MODE__NONE none None
OPERATION_MODE__PROFILE_POSITION profilePosition ProfilePosition
"""


def generate_remote(templates: Path, outdir: Path) -> None:
    cmd = [
        sys.executable,
        "-m",
        "generator",
        "generate",
        str(CONFIG),
        "-f",
        "--remote",
        str(outdir),
        "--templates",
        str(templates),
    ]
    subprocess.run(cmd, cwd=ROOT, check=True, capture_output=True)


def test_project_template_is_rendered(tmp_path: Path):
    templates = tmp_path / "templates"
    templates.mkdir()
    (templates / "od_custom.hpp.j2").write_text(CUSTOM_TEMPLATE)
    outdir = tmp_path / "cpu1"

    generate_remote(templates, outdir)

    assert (outdir / "od_custom.hpp").read_text() == EXPECTED


def test_generator_template_names_are_not_rendered(tmp_path: Path):
    templates = tmp_path / "templates"
    templates.mkdir()
    (templates / "enum.j2").write_text("override")
    outdir = tmp_path / "cpu1"

    generate_remote(templates, outdir)

    assert not (outdir / "enum").exists()
