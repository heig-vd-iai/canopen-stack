"""Generate EDS file.

EDS file is a text file conform to the INI format.
"""

import configparser
import datetime
from .types import object_types

def od_to_eds(config, flat_od, filename, git_info):
    revision_number = 0x12345678

    baudrates = {10, 20, 50, 125, 250, 500, 800, 1000}
    supported_baudrates = {
        f"BaudRate_{baudrate}": baudrate in config['baudrate'] for baudrate in baudrates
    }

    eds = {
        'FileInfo': {
            'CreatedBy': git_info['committer'],
            'ModifiedBy': git_info['committer'],
            'Description': "Elmo Gold drive EDS file",
            'CreationTime': git_info['first_commit_date'].strftime("%I:%M%p"),
            'CreationDate': git_info['first_commit_date'].strftime("%d-%m-%Y"),
            'ModificationTime': datetime.datetime.now().strftime("%I:%M%p"),
            'ModificationDate': datetime.datetime.now().strftime("%d-%m-%Y"),
            'FileName': filename,
            'FileVersion': "002",
            'FileRevision': "01",
            'EDSVersion': "4.0",
        },

        'DeviceInfo': {
            'VendorName': config['device']['vendor']['name'],
            'VendorNumber': config['device']['vendor']['number'],
            'ProductName': config['device']['product']['name'],
            'ProductNumber': config['device']['product']['number'],
            'RevisionNumber': revision_number,
            'OrderCode': 1,
            'SimpleBootUpMaster': 0,
            'SimpleBootUpSlave': 0,
            'Granularity': 8,
            'DynamicChannelsSupported': 0,
            'CompactPDO': 23,
            'GroupMessaging': 0,
            'NrOfRXPDO': 4,
            'NrOfTXPDO': 4,
            'LSS_Supported': 0,
            **supported_baudrates
        },

        'MandatoryObjects': {
        },

        'OptionalObjects': {
        }
    }

    for code, item in flat_od.items():
        access = 'ro'
        if item['access'] == 'rw':
            access = 'rw'
        elif item['access'] == 'r':
            access = 'ro'
        elif item['access'] == 'w':
            access = 'wo'
        else:
            raise ValueError(f"Unknown access type: {item['access']}")

        highlimit = {'HighLimit': item['limits']['high']} if item['limits']['high'] is not None else {}
        lowlimit = {'LowLimit': item['limits']['low']} if item['limits']['low'] is not None else {}

        key = hex(code.index)[2:]
        keyfull = f"{key}sub{code.subindex:02x}"

        eds[key] = {
            'ParameterName': item['name'],
            'ObjectType': object_types[item['type']],
            'DataType': item['datatype'].code,
            'SubNumber': object['length'],
        }

        eds[keyfull] = {
            'ParameterName': item['name'],
            'ObjectType': object_types[item['type']],
            'DataType': item['datatype'].code,
            'AccessType': access,
            'DefaultValue': item['default'],
            'PDOMapping': int(bool(item['pdo'])),
            **lowlimit,
            **highlimit,
        }

    return eds

def write_eds(eds, filename):
    config = configparser.ConfigParser()
    config.optionxform = str  # Preserve case of keys
    config.read_dict(eds)
    with open(filename, 'w') as configfile:
        config.write(configfile)
        configfile.write("\n")
