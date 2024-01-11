from .object_types import Object
from datetime import datetime
import jinja2
import os

script_dir = os.path.dirname(__file__)
TEMPLATES_DIR = script_dir + "/templates"
HEADER_TEMPLATE = "od.jinja"
EDS_TEMPLATE = "eds.jinja"


class ObjectDictionary:
    def __init__(self, data: dict) -> None:
        now = datetime.now()
        self.node_id: int = data.get("NodeId", 1)
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
            "ModificationTime": dico.get("ModificationTime", ""),
            "ModificationDate": dico.get("ModificationDate", ""),
            "ModifiedBy": dico.get("ModifiedBy", "")
        }
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
        self.mandatory_objects = sorted([Object.get_instance(obj_data.get("ObjectType"), obj_data) for obj_data in data.get("MandatoryObjects", [])], key=self._get_key)
        self.optional_objects = sorted([Object.get_instance(obj_data.get("ObjectType"), obj_data) for obj_data in data.get("OptionalObjects", [])], key=self._get_key)
        self.all_objects = sorted(self.mandatory_objects + self.optional_objects, key=self._get_key)

    def _get_key(self, object: Object) -> int:
        return object.index

    def to_eds(self) -> str:
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
        return env.get_template(EDS_TEMPLATE).render(
            file_info=self.file_info,
            device_info=self.device_info,
            mandatory_objects=self.mandatory_objects,
            optional_objects=self.optional_objects
        )

    def to_cpp(self) -> str:
        env = jinja2.Environment(loader=jinja2.FileSystemLoader(TEMPLATES_DIR), trim_blocks=True, lstrip_blocks=True)
        return env.get_template(HEADER_TEMPLATE).render(
            objects=self.all_objects,
            node_id=self.node_id,
            date=datetime.now(),
            rpdo_count=self.rpdo_count,
            tpdo_count=self.tpdo_count,
            existing_classes=[
                "Object1A00",
                "Object1001",
                "Object1003",
                "Object1010",
                "Object1011",
                "Object1019",
                "Object1400",
                "Object1600",
                "Object1800"
            ]
        )

    @property
    def rpdo_count(self) -> int:
        return len([obj for obj in self.all_objects if obj.cpp_class_name == "Object1400"])

    @property
    def tpdo_count(self) -> int:
        return len([obj for obj in self.all_objects if obj.cpp_class_name == "Object1800"])

    @property
    def getters(self) -> "list[tuple[int, int, str]]":
        """Returns the list of object entry getters, as a tuple: (index: int, subindex: int, getter: str | None)"""
        return [(object.index, entry.subindex, entry.getter) for object in self.all_objects for entry in object.entries]

    @property
    def setters(self) -> "list[tuple[int, int, str]]":
        """Returns the list of object entry setters, as a tuple: (index: int, subindex: int, getter: str | None)"""
        return [(object.index, entry.subindex, entry.setter) for object in self.all_objects for entry in object.entries]
