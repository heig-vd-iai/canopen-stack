# Schéma YAML

Référence du schéma v2, vérifiée contre les modèles de
`generator/validation/models/`.

Un fichier de configuration a quatre blocs de premier niveau.

| Bloc | Obligatoire | Contenu |
|---|---|---|
| `device` | oui | identité de l'appareil |
| `info` | non | information sur le fichier |
| `profiles` | non | numéros des profils CiA utilisés |
| `objects` | oui | objets du dictionnaire d'objets, par index |

Tout champ inconnu est refusé.

## Bloc `device`

| Champ | Type | Défaut | Note |
|---|---|---|---|
| `vendor.name` | texte | `Unknown` | 1 à 244 caractères |
| `vendor.number` | entier | `0` | 32 bits |
| `product.name` | texte | `Unknown` | 1 à 244 caractères |
| `product.number` | entier | `0` | 32 bits |
| `revision` | texte, liste ou entier | `1.0.0` | `major`, `minor`, `patch`, chacun sur 8 bits |
| `baudrate` | liste ou table | tous les débits CiA | valeurs en kbit/s : 10, 20, 50, 125, 250, 500, 800, 1000 |
| `node_id` | entier | `1` | entre 1 et 127 |
| `order_code` | texte | `""` | 245 caractères au plus |
| `lss_supported` | booléen | `false` | |
| `simple_bootup_slave` | booléen | `false` | |
| `simple_bootup_master` | booléen | `false` | |
| `granularity` | entier | `8` | entre 1 et 255 |
| `dynamic_channels_supported` | booléen | `false` | |
| `compact_pdo` | booléen | `false` | |
| `group_messaging` | booléen | `false` | |

`revision` accepte plusieurs écritures : `1`, `"1.2.3"`, `[1, 2]` ou
`{major: 1, minor: 2}`.

## Bloc `info`

| Champ | Type | Défaut |
|---|---|---|
| `description` | texte | `""` |
| `file_version` | entier | `1` |
| `file_revision` | entier | `0` |
| `created_by` | texte | `""` |
| `created_at` | date et heure | absent |
| `modified_by` | texte | `""` |

## Bloc `profiles`

Liste de numéros entre 300 et 500.

```yaml
profiles:
- 301
- 402
```

Le premier numéro supérieur ou égal à 400 devient le profil annoncé par l'objet
`0x1000`.

## Bloc `objects`

Chaque clé est un index. Le type de l'objet est déduit de ses champs :

| Champ présent | Type déduit |
|---|---|
| `array` ou `length` | tableau |
| `record` | enregistrement |
| aucun des deux | variable |

### Champs communs

| Champ | Type | Défaut | Rôle |
|---|---|---|---|
| `name` | texte | — | nom de l'objet |
| `remote` | texte | `local` | cœur qui sert l'objet |
| `path` | texte | `""` | chemin pointé `a.b.c`, réservé |
| `description` | Markdown | `""` | documentation de l'objet |
| `inherit` | index | absent | objet dont celui-ci hérite |
| `module` | texte | absent | page Markdown de l'objet |
| `category` | texte | `optional` | `mandatory`, `optional` ou `conditional` |

### Champs d'une variable

| Champ | Type | Défaut | Rôle |
|---|---|---|---|
| `datatype` | texte | — | type de la donnée |
| `access` | texte | déduit | `r`, `w` ou `rw` |
| `default` | entier, réel ou texte | `0` | valeur initiale, restaurée par l'objet `0x1011` |
| `limits.min`, `limits.max` | nombre | absents | bornes, toujours par paire |
| `size` | entier | absent | capacité d'une chaîne, en octets |
| `pdo` | booléen | `false` | l'entrée est mappable dans un PDO |
| `unit` | texte | `""` | unité physique, validée par `pint` |
| `scale` | réel | absent | facteur entre la valeur CANopen et la valeur interne |
| `enum` | table | absent | `typedef` et `values` |
| `bitfield` | table | absent | découpage en champs de bits |
| `get` | texte | absent | expression ou fonction de lecture |
| `set` | texte | absent | expression ou fonction d'écriture |

### Types de données

`bool`, `int8`, `int16`, `int32`, `int64`, `uint8`, `uint16`, `uint32`, `uint64`,
`float32`, `float64`, `string`, `domain`.

### Règles de validation

- Le type de `default` doit suivre `datatype`. Un entier pour `intN` et `uintN`, un
  nombre pour `floatN`, un texte pour `string`.
- Une chaîne d'une configuration d'appareil exige `size`. Un profil en est dispensé.
- `limits` exige `min` et `max` ensemble.
- Sans `access` explicite, les droits sont déduits : lecture si `get` est présent,
  écriture si `set` est présent.
- Un `default` de la forme `$NODEID+0x180` est accepté pour un type entier.

## Variable

```yaml
  0x2003:
    name: A limited value
    module: fixture
    datatype: uint16
    access: rw
    default: 50
    limits:
      min: 10
      max: 100
```

Une écriture hors des bornes est refusée avec `SDOAbortCode_DownloadValueTooLow` ou
`SDOAbortCode_DownloadValueTooHigh`.

## Enregistrement

Un enregistrement porte des entrées de types différents, aux sous-index 1 à *n*.

```yaml
  0x2000:
    name: Every scalar type
    module: fixture
    sub0:
      name: Number of entries
    record:
    - name: A boolean
      datatype: bool
      access: rw
      default: 1
    - name: An int8
      datatype: int8
      access: rw
      default: -8
```

Le bloc `sub0` décrit le sous-index 0 :

| Champ | Type | Défaut |
|---|---|---|
| `name` | texte | `Highest sub-index supported` |
| `access` | texte | `r` |
| `default` | entier | nombre d'entrées |

Un `get` ou un `set` posé au niveau de l'objet s'applique à toute entrée qui n'en a pas.

Un enregistrement porte 255 entrées au plus.

## Tableau

Un tableau porte des entrées d'un seul type. Il déclare `datatype` une fois.

```yaml
  0x2100:
    name: Channel value
    module: fixture
    datatype: uint32
    access: r
    length: 4
    array:
    - name: Channel #
      get: remote.channel[#]
```

| Champ | Rôle |
|---|---|
| `length` | nombre de sous-index, déduit de `array` s'il manque |
| `array` | entrées déclarées |
| `sub0` | sous-index 0, comme pour un enregistrement |

Règles :

- Les sous-index au-delà des entrées déclarées répètent la dernière.
- Le caractère `#` dans un nom, un `get` ou un `set` est remplacé par le sous-index.
- Une entrée sans `name` prend `<nom du tableau>_<position>`.

## Énumération

```yaml
  0x2007:
    name: An enumerated value
    module: fixture
    datatype: uint8
    access: rw
    default: 0
    enum:
      typedef: FixtureLevel
      values:
        FIXTURE_LEVEL__LOW: 0
        FIXTURE_LEVEL__HIGH: 1
```

Le générateur rend l'énumération dans `cpu1/od_enum.hpp`. Les clés doivent être des
identifiants C valides. Deux valeurs identiques sont refusées.

Un objet nommé `Modes of operation` avec une énumération produit aussi
`cpu1/od_modes.hpp`.

## Champ de bits

```yaml
  0x6041:
    bitfield:
      0: Ready to switch on
      1: Switched on
      "6..4":
        name: State
        values:
          0: Idle
          1: Running
```

Une clé est un bit ou une plage `haut..bas`. Les plages ne doivent pas se recouvrir.
Une valeur doit tenir dans la largeur de sa plage.

## Héritage

```yaml
  0x2010:
    name: Base object
    datatype: uint32
    access: rw
  0x2011:
    inherit: 0x2010
    name: Derived object
```

L'objet fusionne la description de son parent avec la sienne. Un cycle d'héritage est
refusé.

## Objets distants

Voir [objets distants](objets-distants.md).
