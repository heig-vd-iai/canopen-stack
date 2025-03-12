"""
Parse a CANopen configuration file and generate all the metadata files.
"""

from pathlib import Path

import click
import jinja2
import yaml

from . import ObjectDictionary

SCRIPT_DIR = Path(__file__).parent.absolute()
TEMPLATE_DIR = SCRIPT_DIR / "templates"

PROFILE_FILE = SCRIPT_DIR / "profiles.yaml"


@click.command()
@click.argument("config", type=click.Path(exists=True))
@click.option(
    "--outdir", "-o", type=click.Path(), default="dist", help="Output directory"
)
@click.option("--profile", "-p", type=str, help="Path to the profile file if needed")
@click.option("--all", "-a", is_flag=True, help="Generate all the files")
@click.option("--eds", "-e", is_flag=True, help="Generate the EDS file")
@click.option("--firmware", "-w", is_flag=True, help="Generate the C++ files")
@click.option("--doc", "-d", is_flag=True, help="Generate the documentation")
@click.option("--force", "-f", is_flag=True, help="Force the generation")
def cli(config, outdir, profile, force, **kwargs):
    if not profile:
        profile = PROFILE_FILE

    # Load profile and configuration
    with open(profile, "r") as p:
        with open(config) as config:
            file_name = Path(config.name).stem
            od = ObjectDictionary(yaml.safe_load(p), yaml.safe_load(config), file_name)

    # Create output directory
    outdir = Path(outdir)
    if not outdir.exists():
        outdir.mkdir()
    if not outdir.is_dir():
        raise click.BadParameter("Output directory must be a directory")
    if not outdir.iterdir():
        if not force:
            raise click.BadParameter("Output directory must be empty")

    # Generate files
    if kwargs["all"] or kwargs["eds"]:
        (outdir / "docs").mkdir(exist_ok=force)
        (outdir / "docs/modules").mkdir(exist_ok=force)
        with open(outdir / "docs/object-dictionnary.md", "w") as file:
            file.write(od.to_md())

        for module in od.modules:
            with open(outdir / f"docs/modules/{module}.md", "w") as file:
                file.write(od.to_doc(module))

        with open(outdir / "docs/_sidebar.md", "w") as file:
            env = jinja2.Environment(
                loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
                trim_blocks=True,
                lstrip_blocks=True,
            )
            template = env.get_template("_sidebar.md.j2")
            file.write(template.render(modules=od.modules))

    if kwargs["all"] or kwargs["eds"]:
        with open(outdir / "od.eds", "w") as file:
            file.write(od.to_eds())

    if kwargs["all"] or kwargs["firmware"]:
        (outdir / "cm").mkdir(exist_ok=force)
        (outdir / "cpu1").mkdir(exist_ok=force)

        with open(outdir / "cm/od.hpp", "w") as file:
            file.write(od.to_hpp())

        with open(outdir / "cm/od.cpp", "w") as file:
            file.write(od.to_cpp())

        with open(outdir / "cpu1/od_remote.hpp", "w") as file:
            file.write(od.to_remote())

        with open(outdir / "cpu1/remote_config.hpp", "w") as file:
            file.write(od.to_config())


if __name__ == "__main__":
    cli()
