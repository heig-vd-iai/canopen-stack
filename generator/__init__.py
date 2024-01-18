#!/usr/bin/python3
from datetime import datetime
from typing import Union
import os
import jinja2
import click
from canopen.objectdictionary import Variable, Array, Record
from canopen import Node, ObjectDictionary
from .objects.generic import VarObject, ArrayObject, RecordObject, ENTRY_MAX_SIZE
from .objects.object_1001 import Object1001
from .objects.object_1003 import Object1003
from .objects.object_1010 import Object1010
from .objects.object_1011 import Object1011
from .objects.object_1019 import Object1019
from .objects.object_1400 import Object1400
from .objects.object_1600 import Object1600
from .objects.object_1800 import Object1800
from .objects.object_1A00 import Object1A00
from .object_types import Object

script_dir = os.path.dirname(__file__)

TEMPLATES_DIR = script_dir + "/templates"

HEADER_FILENAME = "od.hpp"
TEMPLATE_FILENAME = HEADER_FILENAME + ".jinja"
EDS_TEMPLATE = "eds.jinja"
MANDATORY_OBJECTS = [0x1000, 0x1001, 0x1018]


#TODO: test yaml
class ObjectDictionary:
    def __init__(self, data: dict) -> None:
        now = datetime.now()
        self.node_id: int = data.get("NodeId", 1)
        ## get file info from yaml
        dico: dict = data.get("FileInfo", {})
        self.file_info = {
            "FileName": dico.get("FileName", ""),
            "FileVersion": dico.get("FileVersion", ""),
            "FileRevision": dico.get("FileRevision", ""),
            "EDSVersion": "4.0",
            "Description": dico.get("Description", ""),
            "CreationTime": now.strftime("%H:%M"),
            "CreationDate": now.strftime("%Y-%m-%d"),
            "CreatedBy": dico.get("CreatedBy", ""),
            "ModificationTime": now.strftime("%H:%M"),
            "ModificationDate": now.strftime("%Y-%m-%d"),
            "ModifiedBy": dico.get("ModifiedBy", "")
        }
        ## get device info from yaml
        dico: dict = data.get("DeviceInfo", {})
        self.device_info = {
            "VendorName": dico.get("VendorName", ""),
            "VendorNumber": dico.get("VendorNumber", ""),
            "ProductName": dico.get("ProductName", ""),
            "ProductNumber": dico.get("ProductNumber", ""),
            "RevisionNumber": dico.get("RevisionNumber", ""),
            "OrderCode": dico.get("OrderCode", ""),
            "BaudRate_10": "1",
            "BaudRate_20": "1",
            "BaudRate_50": "1",
            "BaudRate_125": "1",
            "BaudRate_250": "1",
            "BaudRate_500": "1",
            "BaudRate_800": "1",
            "BaudRate_1000": "1",
            "SimpleBootUpMaster": "0",
            "SimpleBootUpSlave": "1",
            "Granularity": "8",
            "DynamicChannelsSupported": "0",
            "CompactPDO": "0",
            "GroupMessaging": "0",
            "NrOfRXPDO": "4",
            "NrOfTXPDO": "4",
            "LSS_Supported": "0"
        }
        ### use for debug yaml
        if(0):
            for obj in data.get("OptionalObjects", []):
                try:
                    Object.get_instance(obj.get("ObjectType"), obj)
                except Exception as e:
                    print(obj.get("ParameterName"))
        ###

        ## get objects from yaml
        self.mandatory_objects = sorted([Object.get_instance(obj_data.get("ObjectType"), obj_data) for obj_data in data.get("MandatoryObjects", [])], key=self._get_key)
        self.optional_objects = sorted([Object.get_instance(obj_data.get("ObjectType"), obj_data) for obj_data in data.get("OptionalObjects", [])], key=self._get_key)
        self.all_objects = sorted(self.mandatory_objects + self.optional_objects, key=self._get_key)
    
    def to_eds(self) -> str:
        """Converts the object dictionary to an EDS file"""
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
        return env.get_template(EDS_TEMPLATE).render(
            file_info=self.file_info,
            device_info=self.device_info,
            mandatory_objects=self.mandatory_objects,
            optional_objects=self.optional_objects
        )
    
    def _get_key(self, object: Object) -> int:
        """Returns the index of the object"""
        return object.index

class ObjectGenerator:
    """ Generates the header file from the EDS file """

    def __init__(self, filename: str, id: int, entriesGranularity: int = 1):
        self.filename: str = filename
        self.id: int = id
        self.granularity: int = entriesGranularity
        self.objects_values = []
        self._parse()

    def _to_canopen_object(self, object: Union[Variable, Array, Record]):
        """ Converts canopen. Variable, canopen.
        Array and canopen.Record to VarObject,
        ArrayObject and RecordObject, or any specific object subclass """
        if isinstance(object, Variable):
            if object.index == 0x1001:
                return Object1001(object.index, [object], self.granularity)
            if object.index == 0x1019:
                return Object1019(object.index, [object], self.granularity)
            return VarObject(object.index, [object], entriesGranularity=self.granularity)
        if isinstance(object, Array):
            entries = list(object.values())
            if object.index == 0x1003:
                return Object1003(object.index, entries, self.granularity)
            if object.index == 0x1010:
                return Object1010(object.index, entries, self.granularity)
            if object.index == 0x1011:
                return Object1011(object.index, entries, self.granularity)
            return ArrayObject(object.index, entries, entriesGranularity=self.granularity)
        if isinstance(object, Record):
            entries = list(object.values())
            if 0x1400 <= object.index <= 0x15FF:
                return Object1400(object.index, entries, self.granularity)
            if 0x1600 <= object.index <= 0x17FF:
                return Object1600(object.index, entries, self.granularity)
            if 0x1800 <= object.index <= 0x19FF:
                return Object1800(object.index, entries, self.granularity)
            if 0x1A00 <= object.index <= 0x1BFF:
                return Object1A00(object.index, entries, self.granularity)
            return RecordObject(object.index, entries, entriesGranularity=self.granularity)

    def _parse(self):
        """ Parses the EDS file and returns a list of objects """

        # Parse the EDS file
        od: ObjectDictionary = Node(self.id, self.filename).object_dictionary
        objects_dict = {}

        # read object dictionnary and convert to canopen objects
        for index, object in od.items():
            try:
                objects_dict[index] = self._to_canopen_object(object)
            except:
                click.echo(f"Failed to parse object {index:X}", err=True)

        # check if objects are valid
        objects_dict = {index: object
                        for index, object in objects_dict.items()
                        if not isinstance(object, (Object1600, Object1A00)) ## add to dic if not 1600 or 1A00
                        or isinstance(object, (Object1600, Object1A00)) and object.verify(objects_dict)} ## or if 1600 or 1A00 and verify
        objects_values = list(objects_dict.values())
        failed_objects = set(od.keys()) - set(objects_dict.keys())
        missing_objects = set(MANDATORY_OBJECTS) - set(objects_dict.keys())

        if len(failed_objects) > 0:
            click.echo("Please make sure these objects are valid:", err=True)
            for index in failed_objects:
                click.echo(f"- {index:X}", err=True)
            raise Exception("Failed to parse some objects")

        if len(missing_objects) > 0:
            click.echo("Some mandatory objects are missing:", err=True)
            for index in missing_objects:
                click.echo(f"- {index:X}", err=True)
            raise Exception("Some mandatory objects are missing")

        uid = 0
        self.objects_values = objects_values
        for obj in objects_values:
            for entry in obj.entries:
                entry.set_uid(uid)
                uid += 1

    def generate_od_header(self):
        """Generates the .hpp header file"""
        if len(self.objects_values) == 0:
            raise Exception("No objects to generate")

        tpdo = [obj for obj in self.objects_values
                if isinstance(obj, Object1800)]
        rpdo = [obj for obj in self.objects_values
                if isinstance(obj, Object1400)]

        objects_index = [f"OD_OBJECT_{obj.index:X} {i}"
                         for i, obj in enumerate(self.objects_values)]

        objects_mux = [f"OD_MUX_{obj.index:X}_SUB_{sub} 0x{(obj.index << 8) | sub:X}"
                       for obj in self.objects_values
                       for sub in range(len(obj.entries))]

        defines = [
            f"OD_NODE_ID {self.id}",
            f"OD_OBJECTS_COUNT {len(self.objects_values)}",
            f"OD_ENTRY_SIZE_MAX {ENTRY_MAX_SIZE}",
            f"OD_TPDO_COUNT {len(tpdo)}",
            f"OD_RPDO_COUNT {len(rpdo)}"]

        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR),
                                 trim_blocks=True,
                                 lstrip_blocks=True,
                                 keep_trailing_newline=True)

        template = env.get_template(TEMPLATE_FILENAME)
        return template.render(
            defines=defines,
            defines_index=objects_index,
            defines_mux=objects_mux,
            namespace="CANopen",
            objects=self.objects_values,
            objectNames=[obj.varName for obj in self.objects_values],
            date=datetime.now())
