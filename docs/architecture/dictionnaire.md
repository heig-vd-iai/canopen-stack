# Dictionnaire d'objets

Le dictionnaire d'objets est rendu depuis le fichier YAML en tables C++ statiques. Sa
taille est connue à la compilation. Aucune allocation dynamique n'a lieu.

## Identifiants d'objet

Chaque sous-index reçoit un identifiant, exposé comme `OD_OBJECT_<index>_SUB<n>`. Cet
identifiant indexe toutes les tables.

```cpp
#define OD_OBJECT_1017_SUB0 14
#define OD_OBJECT_6064_SUB0 72
```

`od.hpp` déclare aussi les tailles du dictionnaire d'objets :

| Macro | Contenu |
|---|---|
| `OD_NODE_ID` | numéro du nœud |
| `OD_LENGTH` | nombre de sous-index |
| `OD_OBJECTS_COUNT` | nombre d'objets |
| `OD_TPDO_COUNT`, `OD_RPDO_COUNT` | nombre de PDO |
| `OD_U32_COUNT`, `OD_F32_COUNT`, … | taille de chaque table typée |

## Tables

| Table | Contenu |
|---|---|
| `CANopenOD::objectIndexTable` | couple `(index, subindex)` de chaque identifiant, trié |
| `ObjectDictionnary::objectMetadataTable` | `Metadata` de chaque identifiant, en mémoire morte |
| `objectGetterTable`, `objectSetterTable` | fonction appelée pour lire ou écrire |
| `boolTable`, `u8Table`, …, `f64Table`, `stringTable` | les valeurs, une table par type C |
| `dataIndexTable` | position de chaque identifiant dans sa table typée |

`od_lookup.hpp` et `od_lookup.cpp` contiennent un hachage parfait. `phf::find()` rend
l'identifiant d'une clé `(index << 8) | subindex`, ou `-1`.

`od_signature` est un entier 64 bits qui identifie le dictionnaire d'objets. Le
générateur y écrit l'horodatage de la génération, au format `AAAAMMJJhhmm`. `loadData()`
compare la signature lue en mémoire non volatile à cette valeur et rejette toute image
qui ne correspond pas.

!!! warning "Chaque génération invalide les images"

    La signature change à chaque exécution du générateur, même sans modification du
    fichier YAML. Le nœud restaure alors les valeurs par défaut au premier démarrage.

## Métadonnées

`Metadata` (voir `src/od/metadata.hpp`) est une structure simple construite à la
compilation et rangée en mémoire morte.

```cpp
struct Metadata {
    Access access;
    DataType dataType;
    uint16_t size;
    const void *defaultValue;
    const void *low;
    const void *high;
};
```

- `access` est un champ de bits : `readable`, `writeable`, `mappable`, `limited`,
  `remote`.
- `defaultValue`, `low` et `high` pointent vers des constantes du type C de l'objet.
- Un `VISIBLE_STRING` pointe vers son texte par défaut et `size` est sa capacité.
- Un `DOMAIN` n'a pas de valeur par défaut et `size` vaut 0.

`getDefaultValue()` rend la valeur par défaut d'un objet scalaire. `restoreData()` et
les services l'utilisent au démarrage.

## Accesseurs générés

Les accesseurs partagent deux signatures :

```cpp
int8_t getter(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setter(const Data &data, int32_t id, SDOAbortCodes &abortCode);
```

Le code de retour :

| Valeur | Sens |
|---|---|
| `0` | la valeur est lue ou écrite |
| `1` | l'accès est en attente, l'appelant doit réessayer |
| `-1` | l'accès échoue, `abortCode` dit pourquoi |

Le code `1` concerne les objets servis par le cœur CPU1. Voir
[objets distants](../generateur/objets-distants.md).

## Lire et écrire

`ObjectDictionnary` expose quatre surcharges par sens d'accès.

```cpp
int8_t readData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t readData(Data &data, uint16_t index, uint8_t subIndex, SDOAbortCodes &abortCode);
int8_t writeData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t writeData(const Data &data, uint16_t index, uint8_t subIndex, SDOAbortCodes &abortCode);
```

Exemple d'accès depuis l'application :

```cpp
Data value;
value.u64 = 0;
SDOAbortCodes abortCode;
if (node.od().readData(value, OD_OBJECT_6064_SUB0, abortCode) == 0)
    printf("Position %ld\n", static_cast<long>(value.i32));

value.f32 = 49.3f;
node.od().writeData(value, 0x6048, 1, abortCode);
```

`readDataWait()` et `writeDataWait()` (voir `src/od_common.hpp`) attendent la fin d'un
accès distant. L'attente est bornée à 10 ms sur l'horloge du transport. Elles rendent
`1` en cas d'expiration.

Autres méthodes utiles :

| Méthode | Rôle |
|---|---|
| `findObject(index, subIndex)` | identifiant de l'objet, `-1` s'il est absent |
| `isSubValid(index, subIndex)` | présence d'un sous-index |
| `getMetadata(id)` | pointeur vers les métadonnées |
| `getSize(id)` | taille de l'objet en octets |
| `loadData(group)`, `saveData(group)` | image non volatile d'un groupe |
| `restoreData(group)` | valeurs par défaut d'un groupe |

## Types de données

| Code | Nom | Type C |
|---|---|---|
| `0x01` | `BOOLEAN` | `bool` |
| `0x02` | `INTEGER8` | `int8_t` |
| `0x03` | `INTEGER16` | `int16_t` |
| `0x04` | `INTEGER32` | `int32_t` |
| `0x05` | `UNSIGNED8` | `uint8_t` |
| `0x06` | `UNSIGNED16` | `uint16_t` |
| `0x07` | `UNSIGNED32` | `uint32_t` |
| `0x08` | `REAL32` | `float` |
| `0x09` | `VISIBLE_STRING` | `char[]` |
| `0x0F` | `DOMAIN` | flux |
| `0x11` | `REAL64` | `double` |
| `0x15` | `INTEGER64` | `int64_t` |
| `0x1B` | `UNSIGNED64` | `uint64_t` |

L'union `Data` (voir `src/unions.hpp`) porte un champ par type : `b`, `i8`, `i16`,
`i32`, `i64`, `u8`, `u16`, `u32`, `u64`, `f32`, `f64`, `str`, `domain`.

## Objets DOMAIN

Un objet `DOMAIN` n'a pas de stockage dans le dictionnaire d'objets. Le serveur SDO
passe ses données au `DomainHandler` enregistré, morceau par morceau, dans les deux
sens.

```cpp
canopen.node.odAccessor().setDomainHandler(&domain);
```

Enregistrez le gestionnaire avant `init()`. L'interface est décrite dans
[SDO](../services/sdo.md).

## Groupes de paramètres

Les objets `0x1010` et `0x1011` agissent sur un groupe de paramètres. Chaque groupe
couvre une plage d'index (voir `src/od/parameterGroup.hpp`) :

| Groupe | Valeur | Plage d'index |
|---|---|---|
| `ParameterGroup_All` | 1 | `0x0000`–`0xFFFF` |
| `ParameterGroup_Communication` | 2 | `0x1000`–`0x1FFF` |
| `ParameterGroup_Application` | 3 | `0x6000`–`0x9FFF` |
| `ParameterGroup_ManufacturerA` | 4 | `0x2000`–`0x4FFF` |

L'image non volatile contient huit octets par objet du groupe, dans l'ordre du
dictionnaire d'objets. La disposition du fichier et celle de la mémoire flash sont
identiques.

## Objets servis par un service

La plupart des objets sont de simples entrées du dictionnaire d'objets. Certains sont
servis par un service, qui répond à leur place.

| Index | Nom | Service |
|---|---|---|
| `0x1001` | Registre d'erreurs | EMCY |
| `0x1003` | Champ d'erreurs prédéfini | EMCY |
| `0x1010` | Sauvegarde des paramètres | dictionnaire d'objets |
| `0x1011` | Restauration des paramètres | dictionnaire d'objets |
| `0x1017` | Temps du producteur heartbeat | HB |
| `0x1019` | Débordement du compteur SYNC | SYNC |
| `0x1029` | Comportement en cas d'erreur | EMCY |
| `0x1400`–`0x15FF` | Paramètres de communication RPDO | PDO |
| `0x1600`–`0x17FF` | Paramètres de mapping RPDO | PDO |
| `0x1800`–`0x19FF` | Paramètres de communication TPDO | PDO |
| `0x1A00`–`0x1BFF` | Paramètres de mapping TPDO | PDO |

D'autres objets standards restent de simples entrées, mais un service lit leur valeur :

| Index | Nom | Lecteur |
|---|---|---|
| `0x1000` | Type de l'appareil | — |
| `0x1005` | COB-ID SYNC | SYNC |
| `0x1007` | Longueur de la fenêtre synchrone | PDO |
| `0x1018` | Objet d'identité | — |

Les profils `generator/profiles/301.yaml` et `402.yaml` décrivent les objets standards.
Écrire `0x1017: {}` dans la configuration suffit.

!!! note "COB-ID SYNC"

    L'objet `0x1005` n'est pas décrit dans le profil 301. Déclarez-le vous-même pour
    changer le COB-ID SYNC. Sans lui, le service SYNC écoute le COB-ID `0x80`.
