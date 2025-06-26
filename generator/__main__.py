import click


@click.command()
@click.argument("config_file", type=click.Path(exists=True))
@click.option(
    "--force", "-f", is_flag=True, help="Force the generation if target exists"
)
def cli(config_file):
    """Generate files from the given configuration file."""
    ...


if __name__ == "__main__":
    cli()
