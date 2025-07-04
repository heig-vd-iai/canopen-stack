from typing import List

from .models.array import Array, ArrayProfile
from .models.config import SchemaConfig
from .models.enum import Enum, EnumEntry, EnumProfile
from .models.profile import SchemaProfile
from .models.record import Record, RecordProfile
from .models.var import Var, VarProfile


def check_compliance_with_profile(
    config: SchemaConfig, profile: SchemaProfile
) -> List[ValueError]:
    """Validate the configuration against the specified profiles."""
    if not config.profiles:
        return []

    errors = []

    for index, object_profile in profile.objects.items():
        if object_profile.category == "mandatory":
            if index not in config.objects:
                errors.append(
                    ValueError(f"Mandatory object {index:04x} not found in config")
                )
                continue

        if object_profile.category == "conditional":
            raise NotImplementedError(
                "Conditional objects are not yet supported in compliance checks"
            )

        assert object_profile.category in ["mandatory", "optional"]

        if index not in config.objects:
            continue

        object_config = config.objects[index]

        # Adds description if missing
        if not object_config.description:
            object_config.description = object_profile.description

        # Specific checks based on object type
        match object_profile.type:
            case "var":
                errors.extend(check_var_compliancy(object_config, object_profile))
            case "array":
                errors.extend(check_array_compliancy(object_config, object_profile))
            case "record":
                errors.extend(check_record_compliancy(object_config, object_profile))

    return errors


def check_var_compliancy(config_var: Var, profile_var: VarProfile):
    """Validate a single variable against its profile."""
    errors = []

    # Object must be the same type as the profile
    if config_var.type != profile_var.type:
        errors.append(f"Type mismatch: {config_var.type} vs {profile_var.type}")

    # Object must be the same datatype as the profile
    if config_var.datatype != profile_var.datatype:
        errors.append(
            f"Datatype mismatch: {config_var.datatype} vs {profile_var.datatype}"
        )

    # Object must have the same access as the profile
    if config_var.access != profile_var.access:
        errors.append(f"Access mismatch: {config_var.access} vs {profile_var.access}")

    return errors


def check_array_compliancy(config_array: Array, profile_array: ArrayProfile):
    """Validate a single array against its profile."""
    errors = []

    # Object must be the same type as the profile
    if config_array.type != profile_array.type:
        errors.append(f"Type mismatch: {config_array.type} vs {profile_array.type}")

    # Object must be the same datatype as the profile
    if config_array.datatype != profile_array.datatype:
        errors.append(
            f"Datatype mismatch: {config_array.datatype} vs {profile_array.datatype}"
        )

    # Object must have the same access as the profile
    if config_array.access != profile_array.access:
        errors.append(
            f"Access mismatch: {config_array.access} vs {profile_array.access}"
        )

    # Object must have the same length as the profile
    if config_array.length > profile_array.max_length:
        errors.append(
            f"Length mismatch: {config_array.length} vs {profile_array.length}"
        )

    # Validate each entry in the array
    for entry in config_array.array:
        if not isinstance(entry, Var):
            raise TypeError(f"Expected Var for array entry, got {type(entry)}")

        errors.extend(check_var_compliancy(entry, profile_array.entry_profile))

    return errors


def check_record_compliancy(config_record: Record, profile_record: RecordProfile):
    """Validate a record against its profile."""
    errors = []

    for subindex, entry in enumerate(profile_record.record):
        if not isinstance(entry, Var):
            raise TypeError(f"Expected Var for record entry, got {type(entry)}")

        if subindex >= len(config_record.record):
            errors.append(
                f"Subindex {subindex} not found in config record for index {config_record.index:04x}"
            )
            continue

        errors.extend(check_var_compliancy(config_record.record[subindex], entry))

    return errors

def check_compliance_with_enum(config_enum: Enum, profile_enum: EnumProfile):
    """Validate a single enum against its profile."""
    errors = []

    for entry_name, entry in profile_enum.values.items():
        # If the entry is not in the config: error
        if entry_name not in config_enum.values:
            errors.append(f"Enum entry {entry_name} not found in config")
            continue

        config_entry = config_enum.values.get(entry_name)
        if not isinstance(config_entry, EnumEntry):
            raise TypeError(
                f"Expected EnumEntry for {entry_name}, got {type(config_entry)}"
            )

        # Adds description if missing
        if config_entry.description is None:
            config_entry.description = entry.description

        # Validate value
        if config_entry.value != entry.value:
            errors.append(
                f"Enum value mismatch for {entry_name}: {config_entry.value} vs {entry.value}"
            )

    # Test if additional entries are in the allowed override range
    additional_entries = set(config_enum.values) - set(profile_enum.values)
    for range_start, range_end in profile_enum.allow_override:
        for entry_name in additional_entries:
            entry_value = config_enum.values[entry_name].value
            if not (range_start <= entry_value <= range_end):
                errors.append(
                    f"Additional enum entry {entry_name} with value {entry_value} not in allowed by profile"
                )
