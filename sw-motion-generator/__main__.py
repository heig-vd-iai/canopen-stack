from . import ObjectDictionary
import click
import yaml
import os

script_dir = os.path.dirname(__file__)


@click.command()
@click.argument("config_file", type=click.Path(exists=True))
def cli(config_file=None):
    with open(script_dir +"/profiles.yaml", "r") as p_file:
        with open(config_file) as c_file:
            od = ObjectDictionary(yaml.safe_load(p_file), yaml.safe_load(c_file))

    with open("doc.md", "w") as file:
        file.write(od.to_md())

    with open("od.eds", "w") as file:
        file.write(od.to_eds())


if __name__ == "__main__":
    cli()