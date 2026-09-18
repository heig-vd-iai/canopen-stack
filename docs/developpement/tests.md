# Tests

Le dépôt porte deux suites : des tests C++ avec doctest et des tests Python avec pytest.

!!! warning "Exécution"

    Ces commandes compilent et exécutent du code. Ne les lancez que si vous le voulez.

## Tests C++

Le cadre est doctest. L'en-tête `tests/cpp/doctest.h` est livré avec le dépôt.

```bash
make test
./build/tests
```

| Fichier | Portée |
|---|---|
| `tests/cpp/test_nmt.cpp` | machine d'états NMT |
| `tests/cpp/test_sdo.cpp` | serveur SDO |
| `tests/cpp/test_pdo.cpp` | service PDO |
| `tests/cpp/test_sync.cpp` | service SYNC |
| `tests/cpp/test_emcy.cpp` | service EMCY |
| `tests/cpp/test_hb.cpp` | producteur heartbeat |
| `tests/cpp/test_od_contract.cpp` | contrat du dictionnaire d'objets |
| `tests/cpp/test_smoke.cpp` | démarrage du nœud |

Deux fixtures portent le matériel simulé :

| Fichier | Contenu |
|---|---|
| `tests/cpp/fake-hardware.hpp` | transport, persistance et objets distants simulés |
| `tests/cpp/full-harness.hpp` | nœud complet monté sur ces implémentations |

La suite compile contre le dictionnaire d'objets de `tests/golden/minimal/cm`.

## Pile minimale

```bash
make test-minimal
./build/tests-minimal
```

Cette cible compile NMT, SDO et heartbeat contre le dictionnaire d'objets de
`tests/golden/bootloader/cm`. Le test est `tests/cpp/minimal/test_minimal.cpp`.

## Tests Python

```bash
uv run pytest
```

La configuration de `pyproject.toml` active la couverture du paquet `generator` et
limite la collecte au dossier `tests/`.

| Fichier | Portée |
|---|---|
| `tests/test_golden.py` | rendu comparé aux fichiers de référence |
| `tests/test_schema_config.py`, `tests/test_schema_profile.py` | schémas |
| `tests/test_model_*.py` | modèles pydantic |
| `tests/test_migrate.py` | migration du schéma v1 |
| `tests/test_phf.py` | hachage parfait |
| `tests/test_tree.py`, `tests/test_identifier.py` | utilitaires |

## Fichiers de référence

`tests/golden/` contient le rendu attendu de deux configurations.

| Dossier | Source |
|---|---|
| `tests/golden/minimal/` | `examples/minimal.v2.yaml` |
| `tests/golden/bootloader/` | `examples/bootloader.v2.yaml` |

`tests/test_golden.py` rend chaque configuration dans un dossier temporaire, puis
compare fichier par fichier. Trois vérifications tournent :

1. La liste des fichiers produits correspond à celle des références.
2. Chaque fichier correspond à sa référence.
3. Aucun fichier C++ produit ne nomme le singleton `node.`.

Deux motifs de ligne sont neutralisés avant la comparaison, parce qu'ils changent à
chaque exécution : `od_signature` et les dates de modification de l'EDS.

Après une modification volontaire du générateur, mettez les références à jour :

```bash
uv run pytest --update-golden
```

Relisez le diff avant de valider.

!!! warning "Dictionnaire d'objets des tests C++"

    Les tests C++ compilent contre `tests/golden/`. Une mise à jour des références
    change aussi ce que la suite C++ compile.

## Format du code

```bash
make format
make format-check
```

`clang-format` traite les sources de `src/`, `platform/`, `example/linux/` et
`tests/cpp/`, sauf `tests/cpp/doctest.h`. La configuration est dans `.clang-format`.

Du côté Python, `ruff` et `black` sont dans le groupe `dev`. Un fichier
`.pre-commit-config.yaml` est fourni.

## Intégration continue

`.github/workflows/ci.yml` définit trois tâches, déclenchées à chaque `push` et à
chaque `pull_request`.

| Tâche | Étapes |
|---|---|
| `python` | `uv sync --locked`, `ruff check`, `black --check`, `pytest` |
| `cpp` | `make lib`, `make test`, `./build/tests`, `make example` |
| `format` | `make format-check` |

La tâche `cpp` ne compile pas la pile minimale et ne cible pas le C2000. Vérifiez ces
deux points à la main.
