"""Helpers shared by the configuration and profile schemas."""


def resolve_inheritance(objects: dict) -> dict:
    """Merge every object with the object it inherits from."""

    def resolve(idx: int, visited: set[int]) -> dict:
        obj = objects[idx]
        inherit_id = obj.get("inherit")
        if not inherit_id:
            return obj
        if inherit_id not in objects:
            raise ValueError(
                f"Object {idx:04X}h inherits from unknown object {inherit_id:04X}h"
            )
        if inherit_id in visited:
            raise ValueError(f"Circular inheritance involving {idx:04X}h")

        base = resolve(inherit_id, visited | {idx})
        merged = {**base, **obj}
        return merged

    return {idx: resolve(idx, set()) for idx in objects}


def infer_object_type(obj: dict) -> str:
    """Infer the type of an object based on its properties."""
    if "length" in obj or "array" in obj:
        return "array"
    if "record" in obj:
        return "record"

    return "var"
