from pathlib import Path

import inflection
from jinja2 import Environment, FileSystemLoader

SCRIPT_DIR = Path(__file__).parent.absolute()
TEMPLATE_DIR = SCRIPT_DIR / "templates"

env = Environment(loader=FileSystemLoader(TEMPLATE_DIR))

def render_template(template_name, **kwargs):
    """Render a template with the given context."""
    template = env.get_template(template_name)
    return template.render(**kwargs)

env.filters['snake_case'] = inflection.underscore
