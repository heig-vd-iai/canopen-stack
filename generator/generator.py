from objects.generic import VarObject, ArrayObject, RecordObject
from canopen.objectdictionary import Variable, Array, Record
from objects.x1800 import TPDOCommunicationObject
from objects.x1A00 import TPDOMappingObject
from canopen import Node, ObjectDictionary
import objects.entries as ntry
from typing import Union
import jinja2



TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"
NODEID = 4                      # TODO get from cli
EDS_FILENAME = "example.eds"    # TODO get from cli

datatype2entryclass = {
    0x01: ntry.BooleanEntry,
    0x02: ntry.Integer8Entry,
    0x03: ntry.Integer16Entry,
    0x04: ntry.Integer32Entry,
    0x05: ntry.Unsigned8Entry,
    0x06: ntry.Unsigned16Entry,
    0x07: ntry.Unsigned32Entry,
    0x08: ntry.Real32Entry,
    0x09: ntry.VisibleStringEntry,
    0x0A: ntry.VisibleStringEntry,
    0x0B: ntry.VisibleStringEntry,
    0x10: ntry.Integer32Entry,
    0x11: ntry.Real64Entry,
    0x12: ntry.Integer64Entry,
    0x13: ntry.Integer64Entry,
    0x14: ntry.Integer64Entry,
    0x15: ntry.Integer64Entry,
    0x16: ntry.Unsigned32Entry,
    0x18: ntry.Unsigned64Entry,
    0x19: ntry.Unsigned64Entry,
    0x1A: ntry.Unsigned64Entry,
    0x1B: ntry.Unsigned64Entry
}

def toCANopenObject(object: Union[Variable, Array, Record]):
    if isinstance(object, Variable):
        return VarObject(object.index, datatype2entryclass[object.data_type](object.access_type, object.default))
    if isinstance(object, Array):
        entries = [datatype2entryclass[entry.data_type](entry.access_type, entry.default) for entry in object.values()]
        return ArrayObject(object.index, entries)
    if isinstance(object, Record):
        entries = [datatype2entryclass[entry.data_type](entry.access_type, entry.default) for entry in object.values()]
        if 0x1800 <= object.index <= 0x19FF: return TPDOCommunicationObject(object.index, entries)
        elif 0x1A00 <= object.index <= 0x1BFF: return TPDOMappingObject(object.index, entries)
        else: return RecordObject(object.index, entries)

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
    f"OD_TPDO_COUNT {len([obj for obj in objectsValues if isinstance(obj, TPDOCommunicationObject)])}"
]
variables = {
    "defines": defines,
    "namespace": "CANopen",
    "objects": objectsValues,
    "objectNames": [obj.varName for obj in objectsValues]
}
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
env.get_template(TEMPLATE_FILENAME).stream(**variables).dump(HEADER_FILENAME)
