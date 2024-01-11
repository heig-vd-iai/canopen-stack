from . import ObjectDictionary
import click
import yaml


@click.command()
@click.argument('filename', type=click.Path(exists=True))
# @click.argument('--cpp_filename', default='od')
# @click.argument('--eds_filename', default='datasheet')
def cli(filename=None):
    """ This program converts a valid YAML file into an C++ header file and an EDS data sheet."""
    with open(filename) as f:
        od = ObjectDictionary(yaml.safe_load(f))
    with open('od.hpp', 'w') as f:
        f.write(od.to_cpp())
    with open('datasheet.eds', 'w') as f:
        f.write(od.to_eds())


if __name__ == '__main__':
    cli()
