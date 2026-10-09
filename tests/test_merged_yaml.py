"""The merged configuration is written back as a readable YAML file."""

from pathlib import Path

import click
import pytest
import yaml

from generator.__main__ import generate_yaml, load, yaml_name
from generator.loaders import load_config, load_raw_config
from generator.migrate import dump_config

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "examples" / "minimal.v2.yaml"

PENDULUM = {
    "device": {"product": {"name": "Pendulum Mod"}},
    "objects": {
        0x3501: {
            "name": "Pendulum force consign",
            "module": "pendulum",
            "remote": "cpu1",
            "datatype": "float32",
            "access": "rw",
            "pdo": True,
            "get": "pendulum.forceConsign",
            "set": "pendulum.forceConsign = @",
        }
    },
}


@pytest.fixture
def overlay(tmp_path):
    path = tmp_path / "mod.yaml"
    path.write_text(yaml.safe_dump(PENDULUM), encoding="utf-8")
    return path


def test_yaml_name_is_the_product_slug():
    assert yaml_name({"device": {"product": {"name": "Servopresse"}}}) == (
        "servopresse.v2.yaml"
    )
    assert yaml_name({"device": {"product": {"name": "Pendulum Mod 2"}}}) == (
        "pendulum-mod-2.v2.yaml"
    )


def test_base_configuration_is_written_under_its_product_name(tmp_path):
    raw, _ = load(CONFIG, None)
    generate_yaml(raw, tmp_path, force=False)
    assert (tmp_path / "minimal.v2.yaml").exists()


def test_overlay_product_name_sets_the_file_name(tmp_path, overlay):
    raw, _ = load(CONFIG, None, [overlay])
    generate_yaml(raw, tmp_path / "od", force=False)
    assert [path.name for path in (tmp_path / "od").iterdir()] == [
        "pendulum-mod.v2.yaml"
    ]


def test_written_yaml_is_the_raw_merged_configuration(tmp_path, overlay):
    raw, _ = load(CONFIG, None, [overlay])
    generate_yaml(raw, tmp_path, force=False)
    written = yaml.safe_load((tmp_path / "pendulum-mod.v2.yaml").read_text("utf-8"))
    assert written == raw
    assert written["profiles"] == load_raw_config(CONFIG)["profiles"]


def test_written_yaml_validates_like_the_direct_pipeline(tmp_path, overlay):
    raw, schema = load(CONFIG, None, [overlay])
    generate_yaml(raw, tmp_path, force=False)
    reloaded = load_config(tmp_path / "pendulum-mod.v2.yaml")
    assert reloaded.model_dump() == schema.model_dump()
    assert reloaded.model_dump() == load_config(CONFIG, overlays=[overlay]).model_dump()


def test_object_indices_are_written_in_hexadecimal(tmp_path, overlay):
    raw, _ = load(CONFIG, None, [overlay])
    text = dump_config(raw)
    assert "\n  0x3501:\n" in text
    assert "\n  13569:" not in text


def test_string_indices_are_written_in_hexadecimal():
    text = dump_config({"objects": {"0x2000": {"name": "x"}}})
    assert "0x2000:" in text


def test_multiline_strings_are_literal_blocks():
    text = dump_config({"objects": {0x2000: {"description": "first\nsecond\n"}}})
    assert "description: |\n      first\n      second\n" in text


def test_key_order_is_preserved():
    raw = {"device": {}, "info": {}, "profiles": [301], "objects": {0x2001: {}}}
    text = dump_config(raw)
    positions = [text.index(f"{key}:") for key in raw]
    assert positions == sorted(positions)


def test_existing_file_requires_force(tmp_path):
    raw, _ = load(CONFIG, None)
    generate_yaml(raw, tmp_path, force=False)
    with pytest.raises(click.ClickException, match="--force"):
        generate_yaml(raw, tmp_path, force=False)
    generate_yaml(raw, tmp_path, force=True)
