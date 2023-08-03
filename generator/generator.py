from objects.generic import VarObject, ArrayObject, RecordObject, ObjectEntry
from objects.x1800 import TPDOCommunicationObject
from objects.x1A00 import TPDOMappingObject
from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from typing import Union
import datatypes as dt
import jinja2


EDS_FILENAME = "example.eds"
TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"

datatype2class = {
    0x01: dt.BooleanType,
    0x02: dt.Integer8Type,
    0x03: dt.Integer16Type,
    0x04: dt.Integer32Type,
    0x05: dt.Unsigned16Type,
    0x06: dt.Unsigned32Type,
    0x07: dt.Unsigned32Type,
    0x08: dt.Real32Type,
    0x10: dt.Integer32Type,
    0x11: dt.Real64Type,
    0x12: dt.Integer64Type,
    0x13: dt.Integer64Type,
    0x14: dt.Integer64Type,
    0x15: dt.Integer64Type,
    0x16: dt.Unsigned32Type,
    0x18: dt.Unsigned64Type,
    0x19: dt.Unsigned64Type,
    0x1A: dt.Unsigned64Type,
    0x1B: dt.Unsigned64Type
}

datatype2ctype = {
    0x01: "bool",
    0x02: "int8_t",
    0x03: "int16_t",
    0x04: "int32_t",
    0x05: "uint8_t",
    0x06: "uint16_t",
    0x07: "uint32_t",
    0x08: "float",
    # 0x09: None,     # "VISIBLE_STRING",
    # 0x0A: None,     # "OCTET_STRING",
    # 0x0B: None,     # "UNICODE_STRING",
    # 0x0C: None,     # "TIME_OF_DAY",
    # 0x0D: None,     # "TIME_DIFFERENCE",
    # 0x0F: None,     # "DOMAIN"
    0x10: "int32_t",
    0x11: "double",
    0x12: "int64_t",
    0x13: "int64_t",
    0x14: "int64_t",
    0x15: "int64_t",
    0x16: "uint32_t",
    0x18: "uint64_t",
    0x19: "uint64_t",
    0x1A: "uint64_t",
    0x1B: "uint64_t"
}

datatype2size = {
    0x01: 1,
    0x02: 1,
    0x03: 2,
    0x04: 4,
    0x05: 1,
    0x06: 2,
    0x07: 4,
    0x08: 4,
    # 0x09: None,     # "VISIBLE_STRING",
    # 0x0A: None,     # "OCTET_STRING",
    # 0x0B: None,     # "UNICODE_STRING",
    # 0x0C: None,     # "TIME_OF_DAY",
    # 0x0D: None,     # "TIME_DIFFERENCE",
    # 0x0F: None,     # "DOMAIN"
    0x10: 4,
    0x11: 8,
    0x12: 8,
    0x13: 8,
    0x14: 8,
    0x15: 8,
    0x16: 4,
    0x18: 8,
    0x19: 8,
    0x1A: 8,
    0x1B: 8
}

def getAccessType(value: str) -> int:
    value = value.lower()
    read_bit = 1 << 0
    write_bit = 1 << 1
    const_bit = 1 << 2
    if value in ["rw", "rww", "rwr"]: return write_bit | read_bit
    if value == "wo": return write_bit
    if value == "ro": return read_bit
    if value == "const": return const_bit | read_bit
    raise Exception(f"Access type not supported: '{value}'")

def toCANopenObject(object: Union[Variable, Array, Record]):
    if isinstance(object, Variable):
        return VarObject(object.index, getAccessType(object.access_type), object.data_type, datatype2ctype[object.data_type], object.name, object.default)
    if isinstance(object, Array):
        objs = [ObjectEntry(getAccessType(obj.access_type), obj.data_type, datatype2ctype[obj.data_type], obj.name, obj.default) for obj in object.values()]
        return ArrayObject(object.index, objs)
    if isinstance(object, Record):
        objs = [ObjectEntry(getAccessType(obj.access_type), obj.data_type, datatype2ctype[obj.data_type], obj.name, obj.default) for obj in object.values()]
        if 0x1800 <= object.index <= 0x19FF: return TPDOCommunicationObject(object.index, objs)
        elif 0x1A00 <= object.index <= 0x1BFF: return TPDOMappingObject(object.index, objs)
        else: return RecordObject(object.index, objs)

def dataTypeFilter(object: Union[Variable, Array, Record]) -> bool:
    retval = True
    entries = [object] if isinstance(object, Variable) else list(object.subindices.values())
    for i, var in enumerate(entries):
        if var.data_type not in datatype2ctype:
            print(f"[Warning] Entry x{'%X' % object.index}: unsupported data type '{hex(var.data_type).upper()}' for sub {i}")
            retval = False
    return retval



od: ObjectDictionary = Node(4, EDS_FILENAME).object_dictionary
objectsDict = dict([(object.index, toCANopenObject(object)) for object in od.values() if dataTypeFilter(object)])
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
