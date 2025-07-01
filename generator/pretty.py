from rich import box
from rich.console import Console
from rich.table import Table

console = Console()

for profile, erro in errors.items():

    # Créer un tableau Rich
    table = Table(
        title=f"Liste des erreurs pour le profil {profile}",
        show_lines=True,
        box=box.SQUARE,  # Bordure fine et élégante
        border_style="grey50",  # Gris clair
    )

    table.add_column("Location", style="cyan", overflow="fold", no_wrap=False)
    table.add_column("Type", style="magenta", overflow="fold", no_wrap=False)
    table.add_column("Description", style="white", overflow="fold", no_wrap=False)

    for err in erro:
        loc = list(err["loc"])
        try:
            idx = loc.index("objects")
            if isinstance(loc[idx + 1], int):
                loc[idx + 1] = f"0x{loc[idx+1]:X}"
        except (ValueError, IndexError):
            pass

        if loc and loc[0] == "profiles":
            loc = loc[2:]

        loc_path = "/".join(str(item) for item in loc)
        error_type = err.get("type", "")

        ctx_error = err.get("ctx", {}).get("error", "")
        if not ctx_error:
            ctx_error = err.get("msg", "")
        error_text = str(ctx_error)

        table.add_row(loc_path, error_type, error_text)

    console.print(table)
