import os

import click
import jinja2
import yaml

from . import ObjectDictionary

script_dir = os.path.dirname(__file__)
TEMPLATE_DIR = os.path.join(script_dir, "templates")


@click.command()
@click.argument("config_file", type=click.Path(exists=True))
def cli(config_file=None):
    with open(script_dir + "/profiles.yaml", "r") as p_file:
        with open(config_file) as c_file:
            file_name = os.path.splitext(os.path.basename(c_file.name))[0]
            od = ObjectDictionary(
                yaml.safe_load(p_file), yaml.safe_load(c_file), file_name
            )

    with open("doc.md", "w") as file:
        file.write(od.to_md())
    os.rename("doc.md", "../docs/objectDictionnary.md")

    for module in od.modules:
        with open(f"{module}.md", "w") as file:
            file.write(od.to_doc(module))
        os.rename(f"{module}.md", f"../docs/modules/{module}.md")

    with open("_sidebar.md", "w") as file:
        env = jinja2.Environment(
            loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
            trim_blocks=True,
            lstrip_blocks=True,
        )
        template = env.get_template("_sidebar.md.j2")
        file.write(template.render(modules=od.modules))
    os.rename("_sidebar.md", "../docs/_sidebar.md")

    with open("od.eds", "w") as file:
        file.write(od.to_eds())

    with open("od.hpp", "w") as file:
        file.write(od.to_hpp())
    os.rename("od.hpp", "../cm/od.hpp")  # TODO: user input for path

    with open("od.cpp", "w") as file:
        file.write(od.to_cpp())
    os.rename("od.cpp", "../cm/od.cpp")

    with open("od_remote.hpp", "w") as file:
        file.write(od.to_remote())
    os.rename("od_remote.hpp", "../cpu1/od_remote.hpp")

    # with open("remote_config.hpp", "w") as file:
    #     file.write(od.to_config())
    # os.rename("remote_config.hpp", "../cpu1/remote_config.hpp")


if __name__ == "__main__":
    cli()
