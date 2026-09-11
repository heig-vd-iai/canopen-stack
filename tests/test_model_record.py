# pylint: disable=missing-function-docstring

import pytest

from generator.validation import Datatype, Record, RecordEntry, Sub0


def test_record_synthesizes_sub0():
    rec = Record(
        name="MyRecord",
        record=[
            RecordEntry(
                name="Sub1",
                datatype=Datatype.from_name("uint32"),
                default=10,
            ),
            RecordEntry(
                name="Sub2",
                datatype=Datatype.from_name("uint16"),
                default=20,
            ),
        ],
    )
    assert rec.type == "record"
    assert len(rec.record) == 2

    entries = rec.subentries()
    assert entries[0].name == "Highest sub-index supported"
    assert entries[0].datatype.name == "uint8"
    assert entries[0].default == 2
    assert str(entries[0].access) == "r"

    assert entries[1].name == "Sub1"
    assert entries[1].datatype.name == "uint32"
    assert entries[1].default == 10

    assert entries[2].name == "Sub2"
    assert entries[2].datatype.name == "uint16"


def test_record_sub0_override():
    rec = Record(
        name="MyRecord",
        sub0=Sub0(name="Number of entries", access="rw", default=0),
        record=[
            RecordEntry(
                name="Sub1",
                datatype=Datatype.from_name("uint8"),
                default=10,
            ),
        ],
    )
    sub0 = rec.subentries()[0]
    assert sub0.name == "Number of entries"
    assert str(sub0.access) == "rw"
    assert sub0.default == 0


def test_record_accessors_are_inherited():
    rec = Record(
        name="MyRecord",
        get="obj.getData",
        set="obj.setData",
        record=[
            RecordEntry(name="Own", datatype="uint8", get="own.get"),
            RecordEntry(name="Inherited", datatype="uint8"),
        ],
    )
    entries = rec.subentries()
    assert entries[0].get == "obj.getData"
    assert entries[1].get == "own.get"
    assert entries[1].set == "obj.setData"
    assert entries[2].get == "obj.getData"
    assert rec.record[1].get is None


def test_record_too_large_raises():
    with pytest.raises(ValueError):
        Record(
            name="BigRecord",
            record=[
                RecordEntry(
                    name=f"Sub{i}",
                    datatype=Datatype.from_name("uint8"),
                    default=0,
                )
                for i in range(300)
            ],
        )
