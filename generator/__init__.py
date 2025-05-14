import os
import datetime
from enum import Enum
from collections import namedtuple, defaultdict

from .schema import SchemaConfig
from .tree import bst_to_array_zero_indexed, build_balanced_bst
from .types import ObjectCode, datatypes

def hex_code_to_str(index, subindex):
    return f"0x{index:04x}_{subindex:02d}"

def flatten_od(od):
    """
    Flattens the object dictionary. Hierarchical objects in the YAML file
    are only a convenience for the user. The actual object dictionary is flat.
    This function takes the hierarchical object dictionary and flattens it
    into a flat dictionary.

    It also adds additional information such as c/c++ type.

    """
    flat_od = {}
    for index, object in od.items():
        match object['type']:
            case 'var':
                """ VAR type are stored with the subindex 0 """
                flat_od[ObjectCode(index, 0)] = object
            case 'array':
                flat_od[ObjectCode(index, 0)] = {
                    'type': 'uint8',
                    'name': 'Number of array entries',
                    'access': 'r',
                    'default': len(object['length'])
                }
                for subindex in range(len(object['length'])):
                    flat_od[ObjectCode(index, subindex + 1)] = {
                        **object,
                        'type': datatypes[object['datatype']],
                        'name': f'{object["name"]} {subindex + 1}',
                        'access': object['access'],
                        'get': object['get'].replace('#', str(subindex + 1)),
                        'set': object['set'].replace('#', str(subindex + 1)),
                    }
            case 'record':
                flat_od[ObjectCode(index, 0)] = {
                    'type': 'uint8',
                    'name': 'Number of records',
                    'access': 'r',
                    'default': len(object['record'])
                }
                for subindex, subobject in enumerate(object['subindex']):
                    flat_od[ObjectCode(index, subindex + 1)] = subobject
            case _:
                raise ValueError(f"Unknown type: {object['type']}")

    # Add additional information to the flat object dictionary
    # used by the Jinja templates
    for code, object in flat_od.items():
        object['c_type'] = object['datatype'].ctype
        object['field'] = object['datatype'].field
        object['index'] = code.index
        object['subindex'] = code.subindex
        object['id'] = index << 0x100 | (subindex & 0xff)
        object['hex_code'] = hex_code_to_str(code)
        object['eds_name'] = f"{code.index:04x}sub{code.subindex}"
        if object['get']:
            object['access'] = ''.join(set(object['access']).add('w'))
        if object['set']:
            object['access'] = ''.join(set(object['access']).add('r'))

    # Set the default index value.
    # Default values are grouped by types.
    index_counter = defaultdict(int)
    for object in flat_od.values():
        if object['default'] is not None:
            index_counter[object['type']] += 1

def get_objects_per_type(od_flat):
    """
    Returns a dictionary with the number of objects per type.
    The keys are the datatype.
    """
    objects_per_type = {}
    for code, object in od_flat.items():
        if object.default is not None:
            if object['type'] not in objects_per_type:
                objects_per_type[object['type']] = None
            objects_per_type[object['type']] = object['default']
    return objects_per_type

def get_bst_search_array(od_flat):
    """
    Returns a dictionary with the number of objects per type.
    The keys are the datatype.
    """
    root = build_balanced_bst(od_flat.items())
    return bst_to_array_zero_indexed(root)

def get_git_info():
    return {
        'committer': os.popen('git config user.name').read().strip(),
        'branch': os.popen('git rev-parse --abbrev-ref HEAD').read().strip(),
        'commit': os.popen('git rev-parse HEAD').read().strip(),
        'date': os.popen('git show -s --format=%ci HEAD').read().strip(),
        'first_commit_date': os.popen('git show -s --format=%ci $(git rev-list --max-parents=0 HEAD)').read().strip(),
    }

class Config:
    def __init__(self, filename: str):
        with open(filename, 'r') as f:
            self.schema = SchemaConfig(f.read())
