# Installation

## Prérequis

| Outil | Rôle | Version |
|---|---|---|
| `uv` | environnement Python et générateur | Python 3.12 ou plus |
| `g++` | compilation de la pile sur PC | C++14 |
| `make` | cibles de compilation | GNU Make |
| `clang-format` | vérification du format C++ | facultatif |
| `doxygen`, `graphviz` | documentation de l'API C++ | facultatif |

Le compilateur TI fixe le plafond du langage à C++14. N'utilisez aucune construction
plus récente.

## Récupérer le dépôt

```bash
git clone https://github.com/heig-vd-iai/canopen-stack.git
cd canopen-stack
```

## Installer les dépendances Python

```bash
uv sync
```

La commande crée `.venv` et installe le paquet `generator` avec ses dépendances :
`click`, `pydantic`, `jinja2`, `pyyaml`, `pint`, `semver`.

## Vérifier l'installation

```bash
uv run python -m generator generate examples/minimal.v2.yaml -f --all dist
make lib
```

La première commande écrit le dictionnaire d'objets dans `dist/`. La seconde produit
`canopen-slave.so`.

## Compiler cette documentation

```bash
make docs-serve
```

Le site est servi en local. Pour produire les fichiers statiques dans `site/` :

```bash
make docs
```

## Interface CAN virtuelle

L'exemple Linux a besoin de l'interface `vcan0`.

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

!!! warning "WSL2"

    Le noyau WSL2 par défaut n'inclut pas le module `vcan`. Recompilez le noyau.
    La procédure complète figure dans `example/README.md`.
