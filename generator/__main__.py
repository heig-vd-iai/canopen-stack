"""Command line interface of the CANopen object dictionary generator."""

from pathlib import Path
from typing import Optional

import click
from pydantic import ValidationError

from .loaders import load_config, read_yaml_file
from .migrate import migrate_config, migrate_profiles, write_yaml
from .render import Renderer, build_context

LOOKUP_HEADER = "od_lookup.hpp"


def _write(path: Path, content: str, force: bool) -> None:
    if not force and path.exists():
        raise click.ClickException(f"{path} already exists, use --force to overwrite")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8", newline="\n")


def generate_local(renderer: Renderer, outdir: Path, force: bool) -> None:
    _write(outdir / "od.hpp", renderer.to_hpp(), force)
    _write(outdir / "od.cpp", renderer.to_cpp(), force)
    hpp, cpp = renderer.to_phf(LOOKUP_HEADER)
    _write(outdir / LOOKUP_HEADER, hpp + "\n", force)
    _write(outdir / "od_lookup.cpp", cpp + "\n", force)


def generate_remote(renderer: Renderer, outdir: Path, force: bool) -> None:
    _write(outdir / "od_remote.hpp", renderer.to_remote(), force)
    _write(outdir / "od_enum.hpp", renderer.to_enum(), force)
    modes = renderer.to_modes()
    if modes is not None:
        _write(outdir / "od_modes.hpp", modes, force)


def generate_eds(renderer: Renderer, outdir: Path, force: bool) -> None:
    _write(outdir / "od.eds", renderer.to_eds(), force)


def generate_documentation(renderer: Renderer, outdir: Path, force: bool) -> None:
    _write(outdir / "index.md", renderer.to_md(), force)
    for module in renderer.module_names():
        _write(outdir / f"{module}.md", renderer.to_doc(module), force)
    _write(outdir / "sidebar.md", renderer.to_sidebar(), force)


def format_validation_error(error: ValidationError) -> str:
    lines = [f"{error.error_count()} validation error(s):"]
    for item in error.errors():
        location = "/".join(
            f"0x{part:04X}" if isinstance(part, int) and part > 255 else str(part)
            for part in item["loc"]
        )
        lines.append(f"  {location}: {item['msg']}")
    return "\n".join(lines)


def load(config: Path, profiles: Optional[Path]):
    try:
        return load_config(config, profiles)
    except ValidationError as error:
        raise click.ClickException(format_validation_error(error)) from error
    except (ValueError, FileNotFoundError) as error:
        raise click.ClickException(str(error)) from error


@click.group()
def cli():
    """Generate CANopen object dictionaries from a YAML configuration."""


OUTDIR = click.Path(file_okay=False, dir_okay=True, path_type=Path)


@cli.command()
@click.argument("config", type=click.Path(exists=True, dir_okay=False, path_type=Path))
@click.option(
    "--profiles",
    type=click.Path(exists=True, file_okay=False, path_type=Path),
    help="Directory holding the CiA profile definitions (<number>.yaml)",
)
@click.option("--force", "-f", is_flag=True, help="Overwrite existing files")
@click.option(
    "--all", "everything", type=OUTDIR, help="Generate everything under this directory"
)
@click.option("--local", type=OUTDIR, help="C++ object dictionary for the CANopen core")
@click.option(
    "--remote", type=OUTDIR, help="C++ accessors and enums for the remote core"
)
@click.option("--eds", "-e", type=OUTDIR, help="EDS file")
@click.option("--doc", "-d", type=OUTDIR, help="Markdown documentation")
def generate(config, profiles, force, everything, local, remote, eds, doc):
    """Generate files from CONFIG (schema v2)."""
    if everything is not None:
        local = local or everything / "cm"
        remote = remote or everything / "cpu1"
        eds = eds or everything
        doc = doc or everything / "docs"

    if not any((local, remote, eds, doc)):
        click.echo("No target selected, nothing to do")
        return

    file_name = config.name.split(".")[0]
    try:
        renderer = Renderer(build_context(load(config, profiles), file_name))
        if local:
            generate_local(renderer, local, force)
        if remote:
            generate_remote(renderer, remote, force)
        if eds:
            generate_eds(renderer, eds, force)
        if doc:
            generate_documentation(renderer, doc, force)
    except ValueError as error:
        raise click.ClickException(str(error)) from error


@cli.group()
def migrate():
    """Convert schema v1 files to schema v2."""


@migrate.command("config")
@click.argument("source", type=click.Path(exists=True, dir_okay=False, path_type=Path))
@click.argument("target", type=click.Path(dir_okay=False, path_type=Path))
@click.option(
    "--v1-profiles",
    required=True,
    type=click.Path(exists=True, dir_okay=False, path_type=Path),
    help="The v1 profiles.yaml the configuration was written against",
)
def migrate_config_command(source: Path, target: Path, v1_profiles: Path):
    """Convert a v1 device configuration to v2."""
    try:
        data = migrate_config(read_yaml_file(source), read_yaml_file(v1_profiles))
    except ValueError as error:
        raise click.ClickException(str(error)) from error
    write_yaml(target, data)
    click.echo(f"Wrote {target}")


@migrate.command("profiles")
@click.argument("source", type=click.Path(exists=True, dir_okay=False, path_type=Path))
@click.argument("target_dir", type=click.Path(file_okay=False, path_type=Path))
def migrate_profiles_command(source: Path, target_dir: Path):
    """Convert a v1 profiles.yaml to one v2 file per profile."""
    try:
        profiles = migrate_profiles(read_yaml_file(source))
    except ValueError as error:
        raise click.ClickException(str(error)) from error
    for pid, data in profiles.items():
        write_yaml(target_dir / f"{pid}.yaml", data)
        click.echo(f"Wrote {target_dir / f'{pid}.yaml'}")


if __name__ == "__main__":
    cli()
