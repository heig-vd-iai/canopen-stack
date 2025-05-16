"""Generate EDS file.

EDS file is a text file conform to the INI format.
"""

import configparser


def write_eds(eds, filename):
    """Write EDS data to a file."""
    config = configparser.ConfigParser()
    config.optionxform = str  # Preserve case of keys
    config.read_dict(eds)
    with open(filename, "w", encoding="utf-8") as configfile:
        config.write(configfile)
        configfile.write("\n")
