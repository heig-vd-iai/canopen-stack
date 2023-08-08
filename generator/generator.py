from objects.generic import VarObject, ArrayObject, RecordObject
from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from objects.object_1003 import Object1003
from objects.object_1010 import Object1010
from objects.object_1011 import Object1011
from objects.object_1800 import Object1800
from objects.object_1A00 import Object1A00
from datetime import datetime
from typing import Union
import jinja2



TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"
NODEID = 4                      # TODO get from cli
EDS_FILENAME = "example.eds"    # TODO get from cli

def toCANopenObject(object: Union[Variable, Array, Record]):
    """This function converts canopen.Variable, canopen.Array and canopen.Record to VarObject, ArrayObject and RecordObject, or any specific object subclass"""
    if isinstance(object, Variable):
        return VarObject(object.index, [object])
    if isinstance(object, Array):
        entries = list(object.values())
        if object.index == 0x1003: return Object1003(object.index, entries)
        if object.index == 0x1010: return Object1010(object.index, entries)
        if object.index == 0x1011: return Object1011(object.index, entries)
        return ArrayObject(object.index, entries)
    if isinstance(object, Record):
        entries = list(object.values())
        if 0x1800 <= object.index <= 0x19FF: return Object1800(object.index, entries)
        if 0x1A00 <= object.index <= 0x1BFF: return Object1A00(object.index, entries)
        return RecordObject(object.index, entries)



od: ObjectDictionary = Node(NODEID, EDS_FILENAME).object_dictionary
objectsDict = {}
for index, object in od.items():
    try: objectsDict[index] = toCANopenObject(object)
    except: pass
objectsDict = {index: object for index, object in objectsDict.items() if not isinstance(object, Object1A00) or isinstance(object, Object1A00) and object.verify(objectsDict)}
objectsValues = list(objectsDict.values())
failedObjects = set(od.keys()) - set(objectsDict.keys())
if len(failedObjects) > 0:
    print("Please make sure these objects are valid:")
    for index in failedObjects: print(f"- {index:X}")
    exit(1)
defines = [
    f"OD_TPDO_COUNT {len([obj for obj in objectsValues if isinstance(obj, Object1800)])}",
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
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
env.get_template(TEMPLATE_FILENAME).stream(**variables).dump(HEADER_FILENAME)
