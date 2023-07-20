from classes.generic import VarObject, ArrayObject, RecordObject, ObjectEntry
from classes.pdo import TPDOCommunicationObject, TPDOMappingObject
from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from typing import Union
import jinja2



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

def toEntry(entry: Union[Variable, Array, Record]):
    try:
        if isinstance(entry, Variable):
            return VarObject(entry.index, 0, entry.data_type, datatype2ctype[entry.data_type], entry.name, entry.default)
        if isinstance(entry, Array):
            objs = [ObjectEntry(0, obj.data_type, datatype2ctype[obj.data_type], obj.name, obj.default) for obj in entry.values()]
            return ArrayObject(entry.index, objs)
        if isinstance(entry, Record):
            objs = [ObjectEntry(0, obj.data_type, datatype2ctype[obj.data_type], obj.name, obj.default) for obj in entry.values()]
            if 0x1800 <= entry.index <= 0x19FF: return TPDOCommunicationObject(entry.index, objs)
            elif 0x1A00 <= entry.index <= 0x1BFF: return TPDOMappingObject(entry.index, objs)
            else: return RecordObject(entry.index, objs)
    except KeyError: pass
    return None

od: ObjectDictionary = Node(4, "example.eds").object_dictionary
objects = [toEntry(obj) for obj in od.values() if toEntry(obj) is not None]

# for object in objects:
#     print("object [%X]" % object.index)
#     print(object.renderObject())
#     print()

variables = {
    "objects": objects,
    "objectNames": [obj.varName for obj in objects]
}
env = jinja2.Environment(loader=jinja2.FileSystemLoader("templates"), trim_blocks=True, lstrip_blocks=True)
env.get_template("od2.hpp.jinja").stream(**variables).dump("od.hpp")
