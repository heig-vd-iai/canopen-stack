# Utilisation du générateur

Le paquet Python `generator` rend le dictionnaire d'objets depuis un fichier YAML de
schéma v2.

Pourquoi pas un simple EDS ? Parce qu'un EDS ne dit pas où vit la donnée, comment elle
est lue et écrite, ni quelle est son unité ou son énumération.

## Commande

```bash
uv run python -m generator generate CONFIG [OPTIONS]
```

Le paquet expose aussi le script `canopen-generator`.

## Options

| Option | Rôle |
|---|---|
| `--all DIR` | rend toutes les cibles sous `DIR` |
| `--local DIR` | dictionnaire d'objets C++ du cœur CM |
| `--remote DIR` | accesseurs et énumérations du cœur CPU1 |
| `--eds DIR`, `-e DIR` | fichier EDS |
| `--doc DIR`, `-d DIR` | documentation Markdown |
| `--profiles DIR` | dossier des profils CiA, `<numéro>.yaml` |
| `--force`, `-f` | écrase les fichiers existants |

Sans cible, la commande ne fait rien et le dit.

`--all DIR` équivaut à `--local DIR/cm --remote DIR/cpu1 --eds DIR --doc DIR/docs`. Une
cible passée explicitement garde sa valeur.

!!! warning "Écrasement"

    Sans `--force`, la commande s'arrête dès qu'un fichier existe.

## Fichiers produits

| Chemin | Cible | Contenu |
|---|---|---|
| `cm/od.hpp`, `cm/od.cpp` | cœur CM | tables du dictionnaire d'objets, identifiants `OD_OBJECT_*` |
| `cm/od_lookup.hpp`, `cm/od_lookup.cpp` | cœur CM | hachage parfait de recherche |
| `cpu1/od_remote.hpp` | cœur CPU1 | accesseurs des objets distants |
| `cpu1/od_enum.hpp` | cœur CPU1 | énumérations déclarées dans la configuration |
| `cpu1/od_modes.hpp` | cœur CPU1 | modes de fonctionnement |
| `od.eds` | outils maître | fichier EDS |
| `docs/index.md`, `docs/<module>.md`, `docs/sidebar.md` | documentation | une page par module |

`od_modes.hpp` n'est écrit que si la configuration contient un objet nommé
`Modes of operation` avec une énumération.

## Exemples

```bash
uv run python -m generator generate examples/minimal.v2.yaml -f --all dist
uv run python -m generator generate config.yaml --local firmware/cm --eds .
```

La cible `make generate` rend `examples/minimal.v2.yaml` dans `dist/`. Changez la source
avec la variable `CONFIG` :

```bash
make generate CONFIG=examples/bootloader.v2.yaml
```

## Profils

Les objets standards viennent des profils de `generator/profiles/`.

| Fichier | Contenu |
|---|---|
| `301.yaml` | profil de communication CiA 301 |
| `402.yaml` | profil d'entraînement CiA 402 |

La configuration liste les profils qu'elle utilise, puis se contente de nommer les
objets qu'elle implémente :

```yaml
profiles:
- 301
- 402
objects:
  0x1017:
    default: 1000
```

Un objet présent dans un profil hérite de sa description. `{}` suffit. La configuration
peut redéfinir n'importe quel champ.

Le module par défaut d'un objet de profil est `cia<numéro>`, donc `cia301` ou `cia402`.

## Migration depuis le schéma v1

```bash
uv run python -m generator migrate config OLD.yaml NEW.yaml --v1-profiles PROFILES.yaml
uv run python -m generator migrate profiles PROFILES.yaml TARGET_DIR
```

- `migrate config` convertit une configuration d'appareil. L'option `--v1-profiles` est
  obligatoire : elle nomme le `profiles.yaml` v1 contre lequel la configuration a été
  écrite.
- `migrate profiles` convertit un `profiles.yaml` v1 en un fichier v2 par profil.

## Après une modification

1. Modifier le fichier YAML.
2. Rendre à nouveau le dictionnaire d'objets.
3. Recompiler.

!!! warning "Signature"

    `od_signature` porte l'horodatage de la génération. Chaque exécution du générateur
    invalide les images non volatiles écrites par le firmware précédent. Le nœud
    restaure alors les valeurs par défaut.
