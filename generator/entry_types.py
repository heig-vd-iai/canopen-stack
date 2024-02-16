import re


class Entry:
    parameter_name: str
    default_value: str
    pdo_mapping: bool
    access_type: str
    type_code: str
    type_value: int
    type_name: str
    ctype_name: str
    size: int
    subindex: int
    getter: str
    setter: str
    meta_data: int
    low_limit: str
    high_limit: str
    remote_getter: str
    remote_setter: str

    @classmethod
    def _get_subclasses(cls) -> "list[type]":
        classes = []
        for c in cls.__subclasses__():
            classes.append(c)
            classes.extend(c._get_subclasses())
        return classes

    
    @classmethod
    def get_instance(cls, type_name: str, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none"):
        return next(c(data, subindex, getter, setter) for c in cls._get_subclasses() if c.type_name in type_name)

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        self.parameter_name: str = str(data.get("ParameterName", ""))
        self.object_type = str(data.get("ObjectType", ""))
        self.default_value: str = str(data.get("DefaultValue", 0x00))
        self.pdo_mapping: bool = data.get("PDOMapping", False)
        self.access_type: str = str(data.get("AccessType", ""))
        self.subindex: int = subindex
        if setter == "none" and getter == "none":
            self.getter: str = str(data.get("Getter", "getLocalData"))
            self.setter: str = str(data.get("Setter", "setLocalData"))
        else:
            self.getter: str = getter
            self.setter: str = setter
        self.low_limit: str = str(data.get("LowLimit", "none"))
        self.high_limit: str = str(data.get("HighLimit", "none"))
        self.remote_getter: str = str(data.get("RemoteGetter", "none"))
        self.remote_setter: str = str(data.get("RemoteSetter", "none"))

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
    def meta_data(self) -> str:
        meta : str = "{.bits = {"
        if self.access_type == "ro" or self.access_type == "const":
            meta += ".readable = true, .writeable = false, "
        if self.access_type == "wo":
            meta += ".readable = false, .writeable = true, "
        if self.access_type == "rw":
            meta += ".readable = true, .writeable = true, "
        if self.pdo_mapping:
            meta += ".mappable = true, "
        else:
            meta += ".mappable = false, "
        if self.low_limit == "none" or self.high_limit == "none":
            meta += ".limited = false, "
        else:
            meta += ".limited = true, "
        if self.remote_getter == "none" or self.remote_setter == "none":
            meta += ".remote = false"
        else:
            meta += ".remote = true"
        meta += "}}"
        return meta

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
    
    def __init__(self, data: dict, subindex: int = 0, precision: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, getter, setter)
        self._isexpr: bool = False
        self._precision: int = precision
        # if len(self.default_value):
        #     pattern = "\$NODEID\+(0x[0-9a-fA-F]+|\d+)"
        #     result = re.search(pattern, self.default_value)
        #     if result == "none":
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
    type_code: str = "b"
    type_name: str = "BOOLEAN"
    ctype_name: str = "bool"
    size: int = 8
    low_limit = "none"
    high_limit = "none"

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, getter, setter)
        if len(self.default_value):
            self.default_value = str(int(self.default_value) > 0).lower()


class Integer8Entry(IntegerEntry):
    type_value: int = 0x02
    type_code: str = "i8"
    type_name: str = "INTEGER8"
    ctype_name: str = "int8_t"
    size: int = 8

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 2, getter, setter)

    

class Integer16Entry(IntegerEntry):
    type_value: int = 0x03
    type_code: str = "i16"
    type_name: str = "INTEGER16"
    ctype_name: str = "int16_t"
    size: int = 16

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 4, getter, setter)


class Integer32Entry(IntegerEntry):
    type_value: int = 0x04
    type_code: str = "i32"
    type_name: str = "INTEGER32"
    ctype_name: str = "int32_t"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 8, getter, setter)


class Integer64Entry(IntegerEntry):
    type_value: int = 0x15
    type_code: str = "i64"
    type_name: str = "INTEGER64"
    ctype_name: str = "int64_t"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 16, getter, setter)



class Unsigned8Entry(IntegerEntry):
    type_value: int = 0x05
    type_code: str = "u8"
    type_name: str = "UNSIGNED8"
    ctype_name: str = "uint8_t"
    size: int = 8

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 2, getter, setter)


class Unsigned16Entry(IntegerEntry):
    type_value: int = 0x06
    type_code: str = "u16"
    type_name: str = "UNSIGNED16"
    ctype_name: str = "uint16_t"
    size: int = 16

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 4, getter, setter)


class Unsigned32Entry(IntegerEntry):
    type_value: int = 0x07
    type_code: str = "u32"
    type_name: str = "UNSIGNED32"
    ctype_name: str = "uint32_t"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 8, getter, setter)


class Unsigned64Entry(IntegerEntry):
    type_value: int = 0x1B
    type_code: str = "u64"
    type_name: str = "UNSIGNED64"
    ctype_name: str = "uint64_t"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, 16, getter, setter)


class Float32Entry(Entry):
    type_value: int = 0x08
    type_code: str = "f32"
    type_name: str = "REAL32"
    ctype_name: str = "float"
    size: int = 32

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, getter, setter)
        if len(self.default_value):
            self.default_value = f"{float(self.default_value)}f"
    


class Float64Entry(Entry):
    type_value: int = 0x11
    type_code: str = "f64"
    type_name: str = "REAL64"
    ctype_name: str = "double"
    size: int = 64

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, getter, setter)
        if len(self.default_value):
            self.default_value = str(float(self.default_value))


class StringEntry(Entry):
    type_value: int = 0x09
    type_code: str = "s"
    type_name: str = "VISIBLE_STRING"
    ctype_name: str = "char"
    low_limit = "none"
    high_limit = "none"

    @property
    def value(self) -> str:
        return f"\"{self.default_value}\""

    @property #TODO: size from type_name
    def size(self) -> int:
        return (len(self.default_value.encode()) + 1) * 8

    def __init__(self, data: dict, subindex: int = 0, getter: str = "none", setter: str = "none") -> None:
        super().__init__(data, subindex, getter, setter)

    def eval_value(self, node_id: int) -> str:
        return f"\"{self.default_value}\""