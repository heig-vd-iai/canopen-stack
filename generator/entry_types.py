import re


class Entry:
    parameter_name: str
    default_value: str
    pdo_mapping: bool
    access_type: str
    type_value: int
    type_name: str
    ctype_name: str
    size: int
    subindex: int
    getter: str
    setter: str
    meta_data: int
    min_value: str
    max_value: str

    @classmethod
    def _get_subclasses(cls) -> "list[type]":
        classes = []
        for c in cls.__subclasses__():
            classes.append(c)
            classes.extend(c._get_subclasses())
        return classes

    
    @classmethod
    def get_instance(cls, type_name: str, data: dict, subindex: int = 0):
        return next(c(data, subindex) for c in cls._get_subclasses() if c.type_name in type_name)

    def __init__(self, data: dict, subindex: int = 0) -> None:
        self.parameter_name: str = str(data.get("ParameterName", ""))
        self.object_type = str(data.get("ObjectType", ""))
        self.default_value: str = str(data.get("DefaultValue", 0x00))
        self.pdo_mapping: bool = data.get("PDOMapping", False)
        self.access_type: str = str(data.get("AccessType", ""))
        self.subindex: int = subindex
        self.getter: str = str(data.get("Getter", "getLocalData"))
        self.setter: str = str(data.get("Setter", "setLocalData"))
        self.min_value: str = str(data.get("MinValue", None))
        self.max_value: str = str(data.get("MaxValue", None))

    def __str__(self) -> str:
        return '\n'.join([f"{k}={v}" for k, v in {
            "ParameterName": self.parameter_name,
            "ObjectType": "0x07",
            "DataType": self.type_hexstr,
            "AccessType": self.access_type,
            "DefaultValue": self.default_value,
            "PDOMapping": str(int(self.pdo_mapping))
        }.items() if v is not None])

    @property
    def meta_data(self) -> int:
        if not self.pdo_mapping:
            if self.access_type == "ro" or self.access_type == "const":
                return "Metadata_ReadOnlyNotMappable"
            if self.access_type == "wo":
                return "Metadata_WriteOnlyNotMappable"
            if self.access_type == "rw":
                return "Metadata_ReadWriteNotMappable"
        else:
            if self.access_type == "ro":
                return "Metadata_ReadOnlyMappable"
            if self.access_type == "wo":
                return "Metadata_WriteOnlyMappable"
            if self.access_type == "rw":
                return "Metadata_ReadWriteMappable"

    @property
    def value(self) -> str:
        ## test if start with $NODEID+ and remplace node id by the current node id
        if self.default_value.startswith("$NODEID+"):
            return int(self.default_value.split('+')[-1], 0) + 1
        return self.default_value

    @property
    def type_hexstr(self) -> str:
        return f"0x{self.type_value:04X}"

    @property
    def cpp_instance_name(self) -> str:
        return f"sub{self.subindex}"

    # TODO: find a better way for this method, maybe change name ? cpp_value(data: dict) ? In reality, it returns the C++ default value
    def eval_value(self, node_id: int) -> str:
        """Returns the evaluated expression $NODEID+{...} for integer entries, the quoted default value for string entry, for other types returns default_value"""
        return self.default_value


class IntegerEntry(Entry):
    type_name: str = "INTEGERENTRYBASE"
    
    def __init__(self, data: dict, subindex: int = 0, precision: int = 0) -> None:
        super().__init__(data, subindex)
        self._isexpr: bool = False
        self._precision: int = precision
        # if len(self.default_value):
        #     pattern = "\$NODEID\+(0x[0-9a-fA-F]+|\d+)"
        #     result = re.search(pattern, self.default_value)
        #     if result == "None":
        #         self.default_value = f"0x{int(self.default_value, 0):0{self._precision}X}"
        #     else:
        #         self.default_value = result.group()
        #         self._isexpr = True

    def eval_value(self, node_id: int) -> str:
        if self._isexpr:
            return f"0x{(int(self.default_value.split('+')[-1], 0) + node_id):0{self._precision}X}"
        return self.default_value


class BooleanEntry(Entry):
    type_value: int = 0x01
    type_name: str = "BOOLEAN"
    ctype_name: str = "bool"
    size: int = 8

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex)
        if len(self.default_value):
            self.default_value = str(int(self.default_value) > 0).lower()
    @property
    def value_min(self) -> str:
        return "0"
    @property
    def value_max(self) -> str:
        return "1"


class Integer8Entry(IntegerEntry):
    type_value: int = 0x02
    type_name: str = "INTEGER8"
    ctype_name: str = "int8_t"
    size: int = 8

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, precision=2)
    
    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-128"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "127"
        return self.max_value


class Integer16Entry(IntegerEntry):
    type_value: int = 0x03
    type_name: str = "INTEGER16"
    ctype_name: str = "int16_t"
    size: int = 16

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 4)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-32768"
        return self.min_value
    
    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "32767"
        return self.max_value


class Integer32Entry(IntegerEntry):
    type_value: int = 0x04
    type_name: str = "INTEGER32"
    ctype_name: str = "int32_t"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 8)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-2147483648"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "2147483647"
        return self.max_value


class Integer64Entry(IntegerEntry):
    type_value: int = 0x15
    type_name: str = "INTEGER64"
    ctype_name: str = "int64_t"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 16)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-9223372036854775808"
        return self.min_value
    
    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "9223372036854775807"
        return self.max_value



class Unsigned8Entry(IntegerEntry):
    type_value: int = 0x05
    type_name: str = "UNSIGNED8"
    ctype_name: str = "uint8_t"
    size: int = 8

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 2)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "0"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "255"
        return self.max_value


class Unsigned16Entry(IntegerEntry):
    type_value: int = 0x06
    type_name: str = "UNSIGNED16"
    ctype_name: str = "uint16_t"
    size: int = 16

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 4)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "0"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "65535"
        return self.max_value


class Unsigned32Entry(IntegerEntry):
    type_value: int = 0x07
    type_name: str = "UNSIGNED32"
    ctype_name: str = "uint32_t"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 8)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "0"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "4294967295"
        return self.max_value


class Unsigned64Entry(IntegerEntry):
    type_value: int = 0x1B
    type_name: str = "UNSIGNED64"
    ctype_name: str = "uint64_t"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex, 16)

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "0"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "18446744073709551615"
        return self.max_value


class Float32Entry(Entry):
    type_value: int = 0x08
    type_name: str = "REAL32"
    ctype_name: str = "float"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex)
        if len(self.default_value):
            self.default_value = f"{float(self.default_value)}f"

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-3.402823466e+38"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "3.402823466e+38"
        return self.max_value


class Float64Entry(Entry):
    type_value: int = 0x11
    type_name: str = "REAL64"
    ctype_name: str = "double"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex)
        if len(self.default_value):
            self.default_value = str(float(self.default_value))

    @property
    def value_min(self) -> str:
        if self.min_value == "None":
            return "-1.7976931348623158e+308"
        return self.min_value

    @property
    def value_max(self) -> str:
        if self.max_value == "None":
            return "1.7976931348623158e+308"
        return self.max_value


class StringEntry(Entry):
    type_value: int = 0x09
    type_name: str = "VISIBLE_STRING"
    ctype_name: str = "char"

    @property
    def value(self) -> str:
        return f"\"{self.default_value}\""

    @property #TODO: size from type_name
    def size(self) -> int:
        return (len(self.default_value.encode()) + 1) * 8

    def __init__(self, data: dict, subindex: int = 0) -> None:
        super().__init__(data, subindex)

    def eval_value(self, node_id: int) -> str:
        return f"\"{self.default_value}\""

    @property
    def value_min(self) -> str:
        return "'\\0'"

    @property
    def value_max(self) -> str:
        return "'\\0'"