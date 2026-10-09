"""Overlays extend a configuration before validation."""

import copy
from pathlib import Path

import pytest
import yaml

from generator.loaders import apply_overlays, load_config

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "examples" / "minimal.v2.yaml"

PENDULUM_FORCE = {
    "name": "Pendulum force consign",
    "module": "pendulum",
    "remote": "cpu1",
    "datatype": "float32",
    "access": "rw",
    "pdo": True,
    "get": "pendulum.forceConsign",
    "set": "pendulum.forceConsign = @",
}


def base_config() -> dict:
    return {
        "device": {"product": {"name": "Servopresse", "number": 0}, "node_id": 1},
        "objects": {
            0x6060: {
                "enum": {
                    "typedef": "OperationModes",
                    "values": {
                        "OPERATION_MODE__NONE": 0,
                        "OPERATION_MODE__SEARCH_LIMIT": {
                            "name": "OPERATION_MODE__SEARCH_LIMIT",
                            "value": -6,
                        },
                    },
                }
            },
            0x2000: {"name": "Existing", "datatype": "uint8"},
        },
    }


@pytest.fixture
def write_overlay(tmp_path):
    def write(content: dict, name: str = "overlay.yaml") -> Path:
        path = tmp_path / name
        path.write_text(yaml.safe_dump(content), encoding="utf-8")
        return path

    return write


def enum_extension(values: dict) -> dict:
    return {"objects": {0x6060: {"enum": {"values": values}}}}


def with_pendulum(content: dict) -> dict:
    content["objects"][0x3501] = PENDULUM_FORCE
    return content


def test_new_object_is_added(write_overlay):
    overlay = write_overlay({"objects": {0x3501: PENDULUM_FORCE}})
    merged = apply_overlays(base_config(), [overlay])
    assert merged["objects"][0x3501] == PENDULUM_FORCE


def test_hexadecimal_string_index_is_normalized(write_overlay):
    overlay = write_overlay({"objects": {"0x3501": PENDULUM_FORCE}})
    merged = apply_overlays(base_config(), [overlay])
    assert merged["objects"][0x3501] == PENDULUM_FORCE


def test_enum_merge_with_int_values(write_overlay):
    overlay = write_overlay(enum_extension({"OPERATION_MODE__PENDULUM": -5}))
    enum = apply_overlays(base_config(), [overlay])["objects"][0x6060]["enum"]
    assert enum["typedef"] == "OperationModes"
    assert enum["values"]["OPERATION_MODE__PENDULUM"] == -5
    assert enum["values"]["OPERATION_MODE__NONE"] == 0


def test_enum_merge_with_dict_values(write_overlay):
    entry = {"name": "OPERATION_MODE__PENDULUM", "value": -5, "description": "P"}
    overlay = write_overlay(enum_extension({"OPERATION_MODE__PENDULUM": entry}))
    enum = apply_overlays(base_config(), [overlay])["objects"][0x6060]["enum"]
    assert enum["values"]["OPERATION_MODE__PENDULUM"] == entry


def test_index_collision_is_rejected(write_overlay):
    overlay = write_overlay({"objects": {0x2000: PENDULUM_FORCE}})
    with pytest.raises(ValueError, match="0x2000 already defined"):
        apply_overlays(base_config(), [overlay])


def test_extra_keys_on_existing_enum_object_are_rejected(write_overlay):
    content = enum_extension({"OPERATION_MODE__PENDULUM": -5})
    content["objects"][0x6060]["name"] = "Modes"
    overlay = write_overlay(content)
    with pytest.raises(ValueError, match="0x6060 already defined"):
        apply_overlays(base_config(), [overlay])


def test_enum_name_collision_is_rejected(write_overlay):
    overlay = write_overlay(enum_extension({"OPERATION_MODE__NONE": 9}))
    with pytest.raises(ValueError, match="OPERATION_MODE__NONE already defined"):
        apply_overlays(base_config(), [overlay])


def test_enum_value_collision_is_rejected(write_overlay):
    overlay = write_overlay(enum_extension({"OPERATION_MODE__PENDULUM": -6}))
    with pytest.raises(ValueError, match="-6 already used by"):
        apply_overlays(base_config(), [overlay])


def test_enum_value_collision_with_dict_entry_is_rejected(write_overlay):
    entry = {"name": "OPERATION_MODE__OTHER", "value": 0}
    overlay = write_overlay(enum_extension({"OPERATION_MODE__OTHER": entry}))
    with pytest.raises(ValueError, match="OPERATION_MODE__NONE"):
        apply_overlays(base_config(), [overlay])


def test_error_names_the_overlay_file(write_overlay):
    overlay = write_overlay({"objects": {0x2000: PENDULUM_FORCE}}, "pendulum.yaml")
    with pytest.raises(ValueError, match="pendulum.yaml"):
        apply_overlays(base_config(), [overlay])


def test_device_product_is_overridden(write_overlay):
    overlay = write_overlay({"device": {"product": {"name": "Pendule"}}})
    device = apply_overlays(base_config(), [overlay])["device"]
    assert device["product"] == {"name": "Pendule", "number": 0}
    assert device["node_id"] == 1


def test_two_overlays_are_merged(write_overlay):
    first = write_overlay(with_pendulum(enum_extension({"A__B": -5})), "first.yaml")
    second = write_overlay(enum_extension({"A__C": -4}), "second.yaml")
    merged = apply_overlays(base_config(), [first, second])
    values = merged["objects"][0x6060]["enum"]["values"]
    assert (values["A__B"], values["A__C"]) == (-5, -4)
    assert 0x3501 in merged["objects"]


def test_two_overlays_defining_the_same_index_are_rejected(write_overlay):
    first = write_overlay({"objects": {0x3501: PENDULUM_FORCE}}, "first.yaml")
    second = write_overlay({"objects": {0x3501: PENDULUM_FORCE}}, "second.yaml")
    with pytest.raises(ValueError, match="0x3501 already defined.*second.yaml"):
        apply_overlays(base_config(), [first, second])


def test_two_overlays_with_the_same_enum_value_are_rejected(write_overlay):
    first = write_overlay(enum_extension({"A__B": -5}), "first.yaml")
    second = write_overlay(enum_extension({"A__C": -5}), "second.yaml")
    with pytest.raises(ValueError, match="already used by A__B"):
        apply_overlays(base_config(), [first, second])


def test_raw_is_not_mutated(write_overlay):
    raw = base_config()
    snapshot = copy.deepcopy(raw)
    content = with_pendulum(enum_extension({"OPERATION_MODE__PENDULUM": -5}))
    content["device"] = {"product": {"name": "Pendule"}}
    overlay = write_overlay(content)
    apply_overlays(raw, [overlay])
    assert raw == snapshot


def test_load_config_validates_the_overlay(write_overlay):
    content = with_pendulum(enum_extension({"OPERATION_MODE__PENDULUM": -5}))
    overlay = write_overlay(content)
    config = load_config(CONFIG, overlays=[overlay])
    assert config.objects[0x3501].name == "Pendulum force consign"
    assert "OPERATION_MODE__PENDULUM" in config.objects[0x6060].enum.values


TRACE_VALUES = {
    "TRACE_SIGNAL__NONE": 0,
    "TRACE_SIGNAL__POSITION": {"value": 1, "get": "axis.position"},
}

PENDULUM_TRACES = {
    "TRACE_SIGNAL__PENDULUM_ANGLE": {"value": 9, "get": "pendulum.angle"},
    "TRACE_SIGNAL__PENDULUM_VELOCITY": {"value": 10, "get": "pendulum.velocity"},
}


def trace_entry(name: str) -> dict:
    return {
        "name": name,
        "datatype": "uint16",
        "access": "rw",
        "enum": {"typedef": "TraceSignal", "values": dict(TRACE_VALUES)},
    }


def trace_record() -> dict:
    return {
        "name": "Trace signal",
        "module": "fixture",
        "record": [
            trace_entry("Trace 0 signal"),
            trace_entry("Trace 1 signal"),
            {"name": "Trace length", "datatype": "uint16", "access": "rw"},
        ],
    }


def record_config() -> dict:
    config = base_config()
    config["objects"][0x2850] = trace_record()
    return config


def record_extension(*names: str, values: dict = PENDULUM_TRACES) -> dict:
    entries = [{"name": name, "enum": {"values": values}} for name in names]
    return {"objects": {0x2850: {"record": entries}}}


def trace_values(merged: dict, position: int) -> dict:
    return merged["objects"][0x2850]["record"][position]["enum"]["values"]


def test_record_entry_enum_merge(write_overlay):
    overlay = write_overlay(record_extension("Trace 0 signal"))
    merged = apply_overlays(record_config(), [overlay])
    assert trace_values(merged, 0) == {**TRACE_VALUES, **PENDULUM_TRACES}
    assert trace_values(merged, 1) == TRACE_VALUES
    assert merged["objects"][0x2850]["record"][0]["enum"]["typedef"] == "TraceSignal"


def test_record_two_entries_enum_merge(write_overlay):
    overlay = write_overlay(record_extension("Trace 0 signal", "Trace 1 signal"))
    merged = apply_overlays(record_config(), [overlay])
    assert trace_values(merged, 0) == {**TRACE_VALUES, **PENDULUM_TRACES}
    assert trace_values(merged, 1) == {**TRACE_VALUES, **PENDULUM_TRACES}
    assert merged["objects"][0x2850]["name"] == "Trace signal"


def test_record_unknown_entry_name_is_rejected(write_overlay):
    overlay = write_overlay(record_extension("Trace 2 signal"), "pendulum.yaml")
    with pytest.raises(
        ValueError, match="0x2850 has no record entry named 'Trace 2 signal'.*pendulum"
    ):
        apply_overlays(record_config(), [overlay])


def test_record_entry_without_enum_is_rejected(write_overlay):
    overlay = write_overlay(record_extension("Trace length"))
    with pytest.raises(ValueError, match="'Trace length' has no enum"):
        apply_overlays(record_config(), [overlay])


def test_record_entry_extra_keys_are_rejected(write_overlay):
    content = record_extension("Trace 0 signal")
    content["objects"][0x2850]["record"][0]["datatype"] = "uint16"
    overlay = write_overlay(content)
    with pytest.raises(ValueError, match="0x2850 already defined"):
        apply_overlays(record_config(), [overlay])


def test_record_object_extra_keys_are_rejected(write_overlay):
    content = record_extension("Trace 0 signal")
    content["objects"][0x2850]["name"] = "Trace signal"
    overlay = write_overlay(content)
    with pytest.raises(ValueError, match="0x2850 already defined"):
        apply_overlays(record_config(), [overlay])


def test_record_entry_enum_value_collision_is_rejected(write_overlay):
    values = {"TRACE_SIGNAL__OTHER": {"value": 1, "get": "other"}}
    overlay = write_overlay(record_extension("Trace 1 signal", values=values))
    with pytest.raises(ValueError, match="1 already used by TRACE_SIGNAL__POSITION"):
        apply_overlays(record_config(), [overlay])


def test_record_raw_is_not_mutated(write_overlay):
    raw = record_config()
    snapshot = copy.deepcopy(raw)
    overlay = write_overlay(record_extension("Trace 0 signal", "Trace 1 signal"))
    apply_overlays(raw, [overlay])
    assert raw == snapshot


def test_load_config_validates_a_record_overlay(write_overlay, tmp_path):
    raw = yaml.safe_load(CONFIG.read_text(encoding="utf-8"))
    raw["objects"][0x2850] = trace_record()
    config_path = tmp_path / "config.yaml"
    config_path.write_text(yaml.safe_dump(raw), encoding="utf-8")
    overlay = write_overlay(record_extension("Trace 0 signal", "Trace 1 signal"))
    config = load_config(config_path, overlays=[overlay])
    for entry in config.objects[0x2850].record[:2]:
        assert entry.enum.values["TRACE_SIGNAL__PENDULUM_ANGLE"].value == 9
        assert entry.enum.values["TRACE_SIGNAL__PENDULUM_VELOCITY"].value == 10
