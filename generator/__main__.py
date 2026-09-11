"""Command line interface of the CANopen object dictionary generator."""

from pathlib import Path

import click

from .loaders import read_yaml_file
from .migrate import migrate_config, migrate_profiles, write_yaml


@click.group()
def cli():
    """Generate CANopen object dictionaries from a YAML configuration."""


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
