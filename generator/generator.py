from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from typing import Union
import jinja2
from objects.generic import VarObject, ArrayObject, RecordObject
from objects.object_1003 import Object1003
from objects.object_1010 import Object1010
from objects.object_1800 import Object1800
from objects.object_1A00 import Object1A00
from objects.entries import *



TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"
NODEID = 4                      # TODO get from cli
EDS_FILENAME = "example.eds"    # TODO get from cli

datatype2entryclass = {
    0x01: BooleanEntry,
    0x02: Integer8Entry,
    0x03: Integer16Entry,
    0x04: Integer32Entry,
    0x05: Unsigned8Entry,
    0x06: Unsigned16Entry,
    0x07: Unsigned32Entry,
    0x08: Real32Entry,
    0x09: VisibleStringEntry,
    0x0A: VisibleStringEntry,
    0x0B: VisibleStringEntry,
    0x0C: Unsigned64Entry,
    0x0D: Unsigned64Entry,
    0x10: Integer32Entry,
    0x11: Real64Entry,
    0x12: Integer64Entry,
    0x13: Integer64Entry,
    0x14: Integer64Entry,
    0x15: Integer64Entry,
    0x16: Unsigned32Entry,
    0x18: Unsigned64Entry,
    0x19: Unsigned64Entry,
    0x1A: Unsigned64Entry,
    0x1B: Unsigned64Entry
}

def toCANopenObject(object: Union[Variable, Array, Record]):
    if isinstance(object, Variable):
        return VarObject(object.index, datatype2entryclass[object.data_type](object.access_type, object.default))
    if isinstance(object, Array):
        entries = [datatype2entryclass[entry.data_type](entry.access_type, entry.default) for entry in object.values()]
        if object.index == 0x1003: return Object1003(object.index, entries)
        if object.index == 0x1010: return Object1010(object.index, entries)
        return ArrayObject(object.index, entries)
    if isinstance(object, Record):
        entries = [datatype2entryclass[entry.data_type](entry.access_type, entry.default) for entry in object.values()]
        if 0x1800 <= object.index <= 0x19FF: return Object1800(object.index, entries)
        if 0x1A00 <= object.index <= 0x1BFF: return Object1A00(object.index, entries)
        return RecordObject(object.index, entries)

def preFilter(object: Union[Variable, Array, Record]) -> bool:
    retval = True
    entries = [object] if isinstance(object, Variable) else list(object.subindices.values())
    for i, var in enumerate(entries):
        if var.data_type not in datatype2entryclass:
            print(f"[Warning] Entry x{'%X' % object.index}: unsupported data type '{hex(var.data_type).upper()}' for sub {i}")
            retval = False
    return retval

def postFilter(object: Union[VarObject, ArrayObject, RecordObject]) -> bool:
    retval = True
    for i, var in enumerate(object.entries):
        if var.accessType == 0:
            print(f"[Warning] Entry x{'%X' % object.index}: unknown access type '{var.accessTypeStr}' for sub {i}")
            retval = False
        if var.size <= 0:
            print(f"[Warning] Entry x{'%X' % object.index}: invalid size '{var.size}' for sub {i}")
            retval = False
    return retval



od: ObjectDictionary = Node(NODEID, EDS_FILENAME).object_dictionary
objectsDict = {index: toCANopenObject(object) for (index, object) in od.items() if preFilter(object)}
objectsDict = {index: object for (index, object) in objectsDict.items() if postFilter(object)}
objectsValues = list(objectsDict.values())
if(any([not object.verify(objectsDict) for object in objectsValues])): exit(1)
defines = [
    f"OD_TPDO_COUNT {len([obj for obj in objectsValues if isinstance(obj, Object1800)])}"
]
variables = {
    "defines": defines,
    "namespace": "CANopen",
    "objects": objectsValues,
    "objectNames": [obj.varName for obj in objectsValues]
}
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
env.get_template(TEMPLATE_FILENAME).stream(**variables).dump(HEADER_FILENAME)
