"""The CiA profiles shipped with the generator validate and merge."""

from generator.loaders import load_profile, profile_objects, read_profile
from generator.validation import Record, Var, VarProfile


def test_read_profile_301_raw():
    raw = read_profile(301)
    assert raw["name"] == "Application layer and communication profile"
    assert 0x1000 in raw["objects"]


def test_load_profile_301():
    profile = load_profile(301)
    device_type = profile.objects[0x1000]
    assert isinstance(device_type, VarProfile)
    assert device_type.category == "mandatory"
    assert device_type.datatype.name == "uint32"
    assert str(device_type.access) == "r"

    identity = profile.objects[0x1018]
    assert identity.type == "record"
    entries = identity.subentries()
    assert [e.name for e in entries][:2] == ["Number of entries", "Vendor-ID"]
    assert entries[0].datatype.name == "uint8"
    assert entries[0].default == 4

    mapping = profile.objects[0x1600]
    sub0 = mapping.subentries()[0]
    assert str(sub0.access) == "rw"
    assert sub0.default == 0
    assert sub0.get == "node.pdo().rpdos[0].mapParameter.getData"


def test_load_profile_402():
    profile = load_profile(402)
    assert profile.objects[0x6040].category == "mandatory"

    homing = profile.objects[0x60E3]
    assert homing.type == "array"
    entries = homing.subentries()
    assert len(entries) == 256
    assert entries[0].default == 1
    assert entries[1].name == "1st suported homing method"
    assert entries[255].datatype.name == "int16"


def test_profile_objects_are_mergeable():
    objects = profile_objects(301)
    assert objects[0x1017]["datatype"] == "uint16"
    assert "record" in objects[0x1018]


def test_config_models_do_not_require_size_in_profiles():
    assert Var(name="s", datatype="string", size=4).size == 4
    assert Record(name="r", record=[]).subentries()[0].default == 0
