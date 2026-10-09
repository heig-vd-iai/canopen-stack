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


ENUM_GET_TEMPLATE = """\
{% for name, entry in enums.FixtureLevel.items() if entry.get %}
{{ name }} {{ entry.value }} {{ entry.get }}
{% endfor %}
"""

ENUM_GET_EXPECTED = """\
FIXTURE_LEVEL__HIGH 1 remote.levels[1].high
"""


def generate_remote(templates: Path, outdir: Path, config: Path = CONFIG) -> None:
    cmd = [
        sys.executable,
        "-m",
        "generator",
        "generate",
        str(config),
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


def test_project_template_sees_enum_entries_with_get(tmp_path: Path):
    config = tmp_path / "config.yaml"
    config.write_text(
        CONFIG.read_text().replace(
            "FIXTURE_LEVEL__HIGH: 1",
            "FIXTURE_LEVEL__HIGH: {value: 1, get: 'remote.levels[1].high'}",
        )
    )
    templates = tmp_path / "templates"
    templates.mkdir()
    (templates / "od_traces.hpp.j2").write_text(ENUM_GET_TEMPLATE)
    outdir = tmp_path / "cpu1"

    generate_remote(templates, outdir, config)

    assert (outdir / "od_traces.hpp").read_text() == ENUM_GET_EXPECTED
