"""Access to templates."""
from pathlib import Path
from typing import ClassVar

from jinja2 import Environment, FileSystemLoader


class Template:
    """A class representing a template."""
    TEMPLATE_DIR: ClassVar[Path] = Path(__file__).parent.absolute()

    def __init__(self):
        import inflection
        env = Environment(loader=FileSystemLoader(self.TEMPLATE_DIR))
        env.filters['snake_case'] = inflection.underscore
        env.filters['camel_case'] = inflection.camelize
        env.filters['kebab_case'] = inflection.dasherize
        self.env = env

    def get_template(self, name):
        """Get a template by name."""
        template = self.env.get_template(name)
        if not template:
            raise ValueError(f"Template {name} not found.")
        return template

    def render(self, name, **kwargs):
        """Render the template with the given context."""
        return self.get_template(name).render(**kwargs)

    def list_templates(self):
        """List all available templates."""
        return self.env.list_templates()
