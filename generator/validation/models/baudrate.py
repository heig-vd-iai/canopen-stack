"""Model for a set of supported baudrates."""

from pydantic_core import core_schema

CIA_BAUDRATES = {10, 20, 50, 125, 250, 500, 800, 1000}


class Baudrate(set):
    """Set of supported baudrates."""

    def __init__(self, iterable=()):
        items = list(iterable)  # <<< FORCING evaluation
        if not all(item in CIA_BAUDRATES for item in items):
            raise ValueError(
                f"Invalid Baudrate: {items}. Valid values are: {CIA_BAUDRATES}"
            )
        super().__init__(item for item in items if item in CIA_BAUDRATES)

    def to_dict(self) -> dict[int, bool]:
        """Convert the Baudrate set to a dictionary."""
        return {b: (b in self) for b in CIA_BAUDRATES}

    def to_dict_eds(self) -> dict[str, int]:
        """Convert the Baudrate set to a dictionary for EDS export."""
        return {f"BaudRate_{b}": int(b in self) for b in CIA_BAUDRATES}

    @classmethod
    def __get_pydantic_core_schema__(cls, _source_type, _handler):
        return core_schema.no_info_after_validator_function(
            cls._validate,
            core_schema.list_schema(items_schema=core_schema.int_schema()),
            serialization=core_schema.plain_serializer_function_ser_schema(list),
        )

    @classmethod
    def _validate(cls, v):
        if isinstance(v, dict):
            # e.g. {125: True, 250: None, 300: False}
            v = [k for k, val in v.items() if val is not False]
        elif not isinstance(v, (list, set, tuple)):
            raise TypeError(f"Invalid Baudrate input: {v}")
        return cls(v)

    def __repr__(self):
        return f"Baudrate({', '.join(map(str, sorted(self)))})"
