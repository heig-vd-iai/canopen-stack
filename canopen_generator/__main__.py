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


def generate_documentation(od: ObjectDictionary, outdir: Path, force: bool):
    index = outdir / "index.md"
    sidebar = outdir / "sidebar.md"
    if not force and index.exists():
        raise FileExistsError(f"File {index} already exists, won't overwrite it")

    if not force and sidebar.exists():
        raise FileExistsError(f"File {sidebar} already exists, won't overwrite it")

    outdir.mkdir(parents=True, exist_ok=True)
    for module in od.modules:
        if not force and (outdir / f"{module}.md").exists():
            raise FileExistsError(
                f"File {outdir / module}.md already exists, won't overwrite it"
            )

    with open(index, "w") as file:
        file.write(od.to_md())

    for module in od.modules:
        with open(outdir / f"{module}.md", "w") as file:
            file.write(od.to_doc(module))

    with open(outdir / "sidebar.md", "w") as file:
        env = jinja2.Environment(
            loader=jinja2.FileSystemLoader(TEMPLATE_DIR),
            trim_blocks=True,
            lstrip_blocks=True,
        )
        template = env.get_template("_sidebar.md.j2")
        file.write(template.render(modules=od.modules))


def generate_eds(od: ObjectDictionary, outdir: Path, force: bool):
    eds_file = outdir / "od.eds"
    if not force and eds_file.exists():
        raise FileExistsError(f"File {eds_file} already exists, won't overwrite it")

    outdir.mkdir(parents=True, exist_ok=True)
    with open(eds_file, "w") as file:
        file.write(od.to_eds())


def generate_local(od: ObjectDictionary, outdir: Path, force: bool):
    header = outdir / "od.hpp"
    source = outdir / "od.cpp"
    if not force and (header.exists() or source.exists()):
        raise FileExistsError(
            f"Files {header} or {source} already exists, won't overwrite them"
        )

    outdir.mkdir(parents=True, exist_ok=True)
    with open(header, "w") as file:
        file.write(od.to_hpp())

    with open(source, "w") as file:
        file.write(od.to_cpp())


def generate_remote(od: ObjectDictionary, outdir: Path, force: bool):
    header = outdir / "od_remote.hpp"
    if not force and header.exists():
        raise FileExistsError(
            f"File {outdir / 'od_remote.hpp'} already exists, won't overwrite it"
        )

    outdir.mkdir(parents=True, exist_ok=True)
    with open(header, "w") as file:
        file.write(od.to_remote())

def generate_enum(od: ObjectDictionary, outdir: Path, force: bool):
    enum_file = outdir / "od_enum.hpp"
    if not force and enum_file.exists():
        raise FileExistsError(f"File {enum_file} already exists, won't overwrite it")

    outdir.mkdir(parents=True, exist_ok=True)
    with open(enum_file, "w") as file:
        file.write(od.to_enum())

def generate_modes(od: ObjectDictionary, outdir: Path, force: bool):
    modes_file = outdir / "od_modes.hpp"
    if not force and modes_file.exists():
        raise FileExistsError(f"File {modes_file} already exists, won't overwrite it")

    outdir.mkdir(parents=True, exist_ok=True)
    with open(modes_file, "w") as file:
        file.write(od.to_modes())

@click.command()
@click.argument("config", type=click.Path(exists=True))
@click.option(
    "--profile",
    "-p",
    type=click.Path(exists=True),
    help="Path to the profile file if needed",
)
@click.option(
    "--force", "-f", is_flag=True, help="Force the generation if target exists"
)
@click.option(
    "--eds",
    "-e",
    required=False,
    help="Generate the EDS file",
    type=click.Path(file_okay=False, dir_okay=True, path_type=Path),
)
@click.option(
    "--local",
    required=False,
    help="Generate the C++ files for manager",
    type=click.Path(file_okay=False, dir_okay=True, path_type=Path),
)
@click.option(
    "--remote",
    required=False,
    help="Generate the C++ files for remote",
    type=click.Path(file_okay=False, dir_okay=True, path_type=Path),
)
@click.option(
    "--doc",
    "-d",
    required=False,
    help="Generate the documentation",
    type=click.Path(file_okay=False, dir_okay=True, path_type=Path),
)
def cli(config, profile, force, eds, local, remote, doc):
    if not profile:
        profile = PROFILE_FILE

    with open(profile, "r") as p:
        with open(config) as config:
            file_name = Path(config.name).stem
            od = ObjectDictionary(
                yaml.safe_load(p), yaml.safe_load(config), file_name
            )

    gen = 0

    if doc:
        generate_documentation(od, doc, force)
        gen += 1

    if eds:
        generate_eds(od, eds, force)
        gen += 1

    if local:
        generate_local(od, local, force)
        gen += 1

    if remote:
        generate_remote(od, remote, force)
        generate_enum(od, remote, force)
        generate_modes(od, remote, force)
        gen += 1

    if not gen:
        click.echo("No target selected, nothing to do")


if __name__ == "__main__":
    cli()
