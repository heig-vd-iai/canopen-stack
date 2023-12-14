import click
from . import ObjectGenerator


@click.command()
@click.argument('filename', type=click.Path(exists=True))
@click.option('--id', type=click.IntRange(1, 127), default=1, help='Node ID')
@click.option('--granularity', type=click.Choice(['1', '2', '4']), default='1', help='The smallest size that should be used for entries types')
def cli(filename=None, id=1, granularity='1'):
    """ This program converts a valid EDS file into an C++ header file."""
    og = ObjectGenerator(filename, id, int(granularity))
    click.echo(og.generate_od_header())


if __name__ == '__main__':
    cli()
