# pylint: disable=missing-function-docstring

import pytest

from generator.validation import Datatype, Record, RecordEntry


def test_record_injects_size_entry():
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
    # Subindex 0 injected
    assert rec.record[0].name == Record.SIZE_ENTRY_NAME
    assert isinstance(rec.record[0].datatype, Datatype)
    assert rec.record[0].datatype.name == "uint8"
    assert rec.record[0].default == 2

    assert rec.record[1].name == "Sub1"
    assert isinstance(rec.record[1].datatype, Datatype)
    assert rec.record[1].datatype.name == "uint32"
    assert rec.record[1].default == 10

    assert rec.record[2].name == "Sub2"
    assert isinstance(rec.record[2].datatype, Datatype)
    assert rec.record[2].datatype.name == "uint16"


def test_record_updates_size_if_present():
    # Provide subindex 0 explicitly
    rec = Record(
        name="MyRecord",
        record=[
            RecordEntry(
                name=Record.SIZE_ENTRY_NAME,
                datatype=Datatype.from_name("uint8"),
                default=0,
            ),
            RecordEntry(
                name="Sub1",
                datatype=Datatype.from_name("uint8"),
                default=10,
            ),
        ],
    )
    assert rec.record[0].default == 2


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
