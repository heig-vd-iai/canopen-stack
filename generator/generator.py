#!/usr/bin/python3
from objects.generic import VarObject, ArrayObject, RecordObject, ENTRY_MAX_SIZE
from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from objects.object_1001 import Object1001
from objects.object_1003 import Object1003
from objects.object_1010 import Object1010
from objects.object_1011 import Object1011
from objects.object_1019 import Object1019
from objects.object_1400 import Object1400
from objects.object_1600 import Object1600
from objects.object_1800 import Object1800
from objects.object_1A00 import Object1A00
from datetime import datetime
from typing import Union
import argparse
import jinja2
import os


TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"
MANDATORY_OBJECTS = [0x1000, 0x1001, 0x1018]

def toCANopenObject(object: Union[Variable, Array, Record]):
    """This function converts canopen.Variable, canopen.Array and canopen.Record to VarObject, ArrayObject and RecordObject, or any specific object subclass"""
    if isinstance(object, Variable):
        if object.index == 0x1001: return Object1001(object.index, [object])
        if object.index == 0x1019: return Object1019(object.index, [object])
        return VarObject(object.index, [object])
    if isinstance(object, Array):
        entries = list(object.values())
        if object.index == 0x1003: return Object1003(object.index, entries)
        if object.index == 0x1010: return Object1010(object.index, entries)
        if object.index == 0x1011: return Object1011(object.index, entries)
        return ArrayObject(object.index, entries)
    if isinstance(object, Record):
        entries = list(object.values())
        if 0x1400 <= object.index <= 0x15FF: return Object1400(object.index, entries)
        if 0x1600 <= object.index <= 0x17FF: return Object1600(object.index, entries)
        if 0x1800 <= object.index <= 0x19FF: return Object1800(object.index, entries)
        if 0x1A00 <= object.index <= 0x1BFF: return Object1A00(object.index, entries)
        return RecordObject(object.index, entries)


parser = argparse.ArgumentParser(description="This program converts a valid EDS file into an C++ header file.")
parser.add_argument("filename", type=str, help="Name of the EDS file", metavar="<filename>")
parser.add_argument("id", type=int, help="Node ID", metavar="<node id>")
args = parser.parse_args()
if(not 0 < args.id < 128): 
    print("Node id must be in range [1, 127]")
    exit(1)
if not os.path.exists(args.filename):
    print(f"No such file: '{args.filename}'")
    exit(1)
od: ObjectDictionary = Node(args.id, args.filename).object_dictionary
objectsDict = {}
for index, object in od.items():
    try: objectsDict[index] = toCANopenObject(object)
    except: pass
objectsDict = {index: object for index, object in objectsDict.items() if not isinstance(object, (Object1600, Object1A00)) or isinstance(object, (Object1600, Object1A00)) and object.verify(objectsDict)}
objectsValues = list(objectsDict.values())
failedObjects = set(od.keys()) - set(objectsDict.keys())
missingObjects = set(MANDATORY_OBJECTS) - set(objectsDict.keys())
ret = False
if len(failedObjects) > 0:
    print("Please make sure these objects are valid:")
    for index in failedObjects: print(f"- {index:X}")
    ret = True
if len(missingObjects) > 0:
    print("Some mandatory objects are missing:")
    for index in missingObjects: print(f"- {index:X}")
    ret = True
if ret: exit(1)
defines = [
    f"OD_NODE_ID {args.id}",
    f"OD_OBJECTS_COUNT {len(objectsValues)}",
    f"OD_ENTRY_SIZE_MAX {ENTRY_MAX_SIZE}",
    f"OD_TPDO_COUNT {len([obj for obj in objectsValues if isinstance(obj, Object1800)])}",
    f"OD_RPDO_COUNT {len([obj for obj in objectsValues if isinstance(obj, Object1400)])}",
    *[f"OD_OBJECT_{obj.index:X} {i}" for i, obj in enumerate(objectsValues)]
]
variables = {
    "defines": defines,
    "namespace": "CANopen",
    "objects": objectsValues,
    "objectNames": [obj.varName for obj in objectsValues],
    "date": datetime.now(),
    "filename": HEADER_FILENAME
}
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True, keep_trailing_newline=True)
env.get_template(TEMPLATE_FILENAME).stream(**variables).dump(HEADER_FILENAME)
print(f"Header file '{HEADER_FILENAME}' was successfully generated")
