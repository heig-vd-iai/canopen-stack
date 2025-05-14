"""Generate EDS file.

EDS file is a text file conform to the INI format.
"""

import configparser
import datetime
from .types import object_types



def write_eds(eds, filename):
    config = configparser.ConfigParser()
    config.optionxform = str  # Preserve case of keys
    config.read_dict(eds)
    with open(filename, 'w') as configfile:
        config.write(configfile)
        configfile.write("\n")
