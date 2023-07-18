from canopen.objectdictionary import Record, Variable, Array
from canopen import Node, ObjectDictionary
import logging
import jinja2

## Steps for strict parsing:
# 1. load from .eds file into canopen.ObjectDictionnary
# 2. for each entry, check if object type is supported (VAR, ARRAY, RECORD, ...)
# 3. for each entry, check if data type is supported (BOOLEAN, INTEGER8, ...)
# 4. for communication profile specific objects, check if they are valid (TPDO mapping, ...)



EDS_FILENAME = "example.eds"
TEMPLATES_DIR = "templates"
HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"



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



class OD_Object:
    def __init__(self, accessType: int, dataType: int, parameterName: str, defaultValue) -> None:
        self.accessType = accessType
        self.dataType = dataType
        self.dataTypeStr = datatype2ctype[dataType]
        self.parameterName = parameterName
        self.defaultValue = 0 if defaultValue is None else defaultValue


class OD_VarEntry:
    """
    The VAR entry stores its value in a single place. It has a single object.
    """
    def __init__(self, index: int, accessType: int, dataType: int, parameterName: str, defaultValue) -> None:
        self.index = index
        self.objectType = 0x07
        self.subNumber = 1
        self.accessType = accessType
        self.dataType = dataType
        self.dataTypeStr = datatype2ctype[dataType]
        self.parameterName = parameterName
        self.defaultValue = 0 if defaultValue is None else defaultValue
        self.varName = 'x%X' % index
        self.objName = 'obj_' + self.varName
        
    def render_OD_Data_declaration(self) -> "list[str]":
        return [f"{self.dataTypeStr} {self.varName}"]
    
    def render_OD_Data_constructor(self) -> "list[str]":
        return [f"{self.varName}({self.defaultValue})"]
    
    def render_OD_Object_declaration(self) -> str:
        return f"OD_Object {self.objName}[1]"
    
    def render_OD_Object_constructor(self) -> str:
        return f"{self.objName}{{OD_Object({self.accessType}, {self.dataType}, sizeof(data.{self.varName}), &data.{self.varName})}}"
    
    def render_OD_Entry_constructor(self) -> str:
        return f"OD_ObjectEntry({self.index}, {self.objectType}, {self.subNumber}, objects.{self.objName})"


class OD_ArrayEntry:
    """
    The ARRAY entry stores an array of size subNumber - 1 for its values, plus a single value for sub0. It has subNumber objects.
    """
    def __init__(self, index: int, objects: "list[OD_Object]") -> None:
        self.index = index
        self.objectType = 0x08
        self.dataType = objects[1].dataType
        self.dataTypeStr = datatype2ctype[self.dataType]
        self.subNumber = len(objects)
        self.objects = objects
        self.varName = 'x%X' % index
        self.objName = 'obj_' + self.varName
        self.sub0Name = self.varName + 'sub0'

    def render_OD_Data_declaration(self) -> "list[str]":
        sub0 = f"{datatype2ctype[0x05]} {self.sub0Name}"
        arr = f"{self.dataTypeStr} {self.varName}[{self.subNumber - 1}]"
        return [sub0, arr]
    
    def render_OD_Data_constructor(self) -> "list[str]":
        sub0 = f"{self.sub0Name}({self.subNumber})"
        defVals = ', '.join([str(obj.defaultValue) for obj in self.objects[1:]])
        arr = f"{self.varName}{{{defVals}}}"
        return [sub0, arr]
    
    def render_OD_Object_declaration(self) -> str:
        return f"OD_Object {self.objName}[{self.subNumber}]"
    
    def render_OD_Object_constructor(self) -> str:
        sub0 = f"OD_Object({self.objects[0].accessType}, {self.objects[0].dataType}, sizeof(data.{self.sub0Name}), &data.{self.sub0Name})"
        arr = ', '.join([f"OD_Object({obj.accessType}, {obj.dataType}, sizeof(data.{self.varName}[{i}]), &data.{self.varName}[{i}])" for i, obj in enumerate(self.objects[1:])])
        return f"{self.objName}{{{sub0}, {arr}}}"
    
    def render_OD_Entry_constructor(self) -> str:
        return f"OD_ObjectEntry({self.index}, {self.objectType}, {self.subNumber}, objects.{self.objName})"


class OD_RecordEntry:
    """
    The RECORD entry stores a struct of subNumber fields for its values. It has subNumber objects. 
    """
    def __init__(self, index: int, objects: "list[OD_Object]") -> None:
        self.index = index
        self.objectType = 0x09
        self.subNumber = len(objects)
        self.objects = objects
        self.varName = 'x%X' % index
        self.objName = 'obj_' + self.varName
        self.sub0Name = self.varName + 'sub0'

    def render_OD_Data_declaration(self) -> "list[str]":
        fields = '; '.join([f"{obj.dataTypeStr} sub{i}" for i, obj in enumerate(self.objects)]) + ";"
        return [f"struct {{{fields}}} {self.varName}"]
    
    def render_OD_Data_constructor(self) -> "list[str]":
        defVals = ', '.join([str(obj.defaultValue) for obj in self.objects])
        return [f"{self.varName}{{{defVals}}}"]

    def render_OD_Object_declaration(self) -> str:
        return f"OD_Object {self.objName}[{self.subNumber}]"
    
    def render_OD_Object_constructor(self) -> str:
        arr = ', '.join([f"OD_Object({obj.accessType}, {obj.dataType}, sizeof(data.{self.varName}.sub{i}), &data.{self.varName}.sub{i})" for i, obj in enumerate(self.objects)])
        return f"{self.objName}{{{arr}}}"
    
    def render_OD_Entry_constructor(self) -> str:
        return f"OD_ObjectEntry({self.index}, {self.objectType}, {self.subNumber}, objects.{self.objName})"



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

def toEntry(entry):
    try:
        if isinstance(entry, Variable):
            return OD_VarEntry(entry.index, getAccessType(entry.access_type), entry.data_type, entry.name, entry.default)
        if isinstance(entry, Array):
            objs = [OD_Object(getAccessType(obj.access_type), obj.data_type, obj.name, obj.default) for obj in list(entry.values())]
            return OD_ArrayEntry(entry.index, objs)
        if isinstance(entry, Record):
            objs = [OD_Object(getAccessType(obj.access_type), obj.data_type, obj.name, obj.default) for obj in list(entry.values())]
            return OD_RecordEntry(entry.index, objs)
    except KeyError: logging.warning(f"Skipping entry [{'0x%X' % entry.index}]")
    return None
    

od: ObjectDictionary = Node(1, EDS_FILENAME).object_dictionary
objectEntries = [toEntry(entry) for entry in od.values() if toEntry(entry) is not None]
allDataDeclarations = [declaration for sublist in objectEntries for declaration in sublist.render_OD_Data_declaration()]
allDataConstructors = [constructor for sublist in objectEntries for constructor in sublist.render_OD_Data_constructor()]
allObjectDeclarations = [entry.render_OD_Object_declaration() for entry in objectEntries]
allObjectConstructors = [entry.render_OD_Object_constructor() for entry in objectEntries]
allEntryConstructors = [entry.render_OD_Entry_constructor() for entry in objectEntries]
rpdoMappings = [entry for entry in objectEntries if 0x1600 <= entry.index <= 0x17FF]
tpdoMappings = [entry for entry in objectEntries if 0x1A00 <= entry.index <= 0x1BFF]
tpdoLongestMapping = max([entry.objects[0].defaultValue for entry in tpdoMappings])
defines = [
    f"OD_OBJECT_COUNT {len(objectEntries)}",
    f"OD_RPDO_COUNT {len(rpdoMappings)}",
    f"OD_TPDO_COUNT {len(tpdoMappings)}",
    f"OD_TPDO_LONGEST_MAP {tpdoLongestMapping}",
    "OD_RPDO_COMMUNICATION_INDEX 0x1400",
    "OD_RPDO_MAPPING_INDEX 0x1600",
    "OD_TPDO_COMMUNICATION_INDEX 0x1800",
    "OD_TPDO_MAPPING_INDEX 0x1A00"
]
variables = {
    "object_entries": objectEntries,
    "all_data_declarations": allDataDeclarations,
    "all_data_constructors": allDataConstructors,
    "all_object_declarations": allObjectDeclarations,
    "all_object_constructors": allObjectConstructors,
    "all_entry_constructors": allEntryConstructors,
    "defines": defines
}
env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
env.get_template(TEMPLATE_FILENAME).stream(**variables).dump(HEADER_FILENAME)
