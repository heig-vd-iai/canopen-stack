"""Types definitions according to the CiA 301 specification."""

objects_ranges = {
    (0x0001, 0x001F): "Static data types",
    (0x0020, 0x003F): "Complex data types",
    (0x0040, 0x005F): "Manufacturer-specific complex data types",
    (0x0060, 0x025F): "Device profile specific data types",
    # Reserved range
    (0x1000, 0x1FFF): "Communication profile area",
    (0x2000, 0x5FFF): "Manufacturer-specific profile area",
    (0x6000, 0x67FF): "Standardized profile 1st logical device",
    (0x6800, 0x6FFF): "Standardized profile 2nd logical device",
    (0x7000, 0x77FF): "Standardized profile 3rd logical device",
    (0x7800, 0x7FFF): "Standardized profile 4th logical device",
    (0x8000, 0x87FF): "Standardized profile 5th logical device",
    (0x8800, 0x8FFF): "Standardized profile 6th logical device",
    (0x9000, 0x97FF): "Standardized profile 7th logical device",
    (0x9800, 0x9FFF): "Standardized profile 8th logical device",
    (0xA000, 0xAFFF): "Standardized network variable area",
    (0xB000, 0xBFFF): "Standardized system variable area",
    # Reserved range
}
