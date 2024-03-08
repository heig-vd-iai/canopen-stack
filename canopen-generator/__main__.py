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
            file_name = os.path.splitext(os.path.basename(c_file.name))[0]
            od = ObjectDictionary(yaml.safe_load(p_file), yaml.safe_load(c_file), file_name)

    with open("doc.md", "w") as file:
        file.write(od.to_md())

    with open("od.eds", "w") as file:
        file.write(od.to_eds())

    with open("od.hpp", "w") as file:
        file.write(od.to_hpp())

    with open("od.cpp", "w") as file:
        file.write(od.to_cpp())

    with open("od_remote.hpp", "w") as file:
        file.write(od.to_remote())


if __name__ == "__main__":
    cli()