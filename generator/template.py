from jinja2 import Environment, FileSystemLoader

def code_filter(value):
    return f"0x{value:04x}"  # Format hexadécimal sur 4 chiffres

# Initialisation de l'environnement Jinja
env = Environment(loader=FileSystemLoader("templates"))

# Ajout du filtre
env.filters['code'] = code_filter
