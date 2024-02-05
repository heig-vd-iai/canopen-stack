import click
from . import ObjectGenerator, ObjectDictionary
import yaml


@click.command()
@click.argument('filename', type=click.Path(exists=True))
@click.argument('eds', type=click.Path(exists=False), default="od.eds")
@click.argument('hpp', type=click.Path(exists=False), default="od.hpp")
@click.argument('cpp', type=click.Path(exists=False), default="od.cpp")
@click.argument('remote', type=click.Path(exists=False), default="od_remote.hpp")
@click.option('--id', type=click.IntRange(1, 127), default=1, help='Node ID')
@click.option('--granularity', type=click.Choice(['1', '2', '4']), default='1', help='The smallest size that should be used for entries types')
def cli(filename=None, eds=None, hpp=None, cpp=None, remote=None, id=1, granularity='1'):
    """ This program converts a valid YAML file into an C++ EDS file and a C++ header file. """
    with open(filename) as f:
        od = ObjectDictionary(yaml.safe_load(f))

    with open(eds, "w") as f:
        f.write(od.to_eds())

    og = ObjectGenerator(eds, od.node_id, int(granularity))

    with open(hpp, "w") as f:
        f.write(od.to_hpp())

    with open(cpp, "w") as f:
        f.write(od.to_cpp())

    with open(remote, "w") as f:
        f.write(od.to_remote())


if __name__ == '__main__':
    cli()
