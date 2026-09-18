# PDO

`PDO` (voir `src/pdo/pdo.hpp`) émet les TPDO et déballe les RPDO reçus. Le mapping est
dynamique.

Référence : CiA 301:2011 §7.2.2.

## Construction

```cpp
static CANopen::PDO pdo(node.od(), transport, remote, node.nodeId);
node.attach(pdo);
CANopen::bindPdo(pdo);
```

Le nombre de canaux vient du dictionnaire d'objets, par les macros `OD_TPDO_COUNT` et
`OD_RPDO_COUNT`.

## Objets servis

| Index | Contenu |
|---|---|
| `0x1400`–`0x15FF` | paramètres de communication RPDO |
| `0x1600`–`0x17FF` | paramètres de mapping RPDO |
| `0x1800`–`0x19FF` | paramètres de communication TPDO |
| `0x1A00`–`0x1BFF` | paramètres de mapping TPDO |

Le service lit aussi l'objet `0x1007`, la longueur de la fenêtre synchrone, exprimée en
microsecondes. Cet objet reste une entrée ordinaire du dictionnaire d'objets.

## Paramètres de communication

| Sous-index | Nom | Unité |
|---|---|---|
| 0 | Nombre d'entrées | — |
| 1 | COB-ID | — |
| 2 | Type de transmission | — |
| 3 | Temps d'inhibition | 100 µs |
| 4 | Réservé | — |
| 5 | Temporisateur d'événement | 1 ms |
| 6 | Valeur de départ SYNC | — |

Bits du COB-ID :

| Bit | Masque | Sens |
|---|---|---|
| 31 | `0x80000000` | PDO invalide |
| 30 | `0x40000000` | RTR interdit |
| 10..0 | `0x7FF` | identifiant CAN |

## Types de transmission

| Valeur | Comportement |
|---|---|
| `0x00` | synchrone acyclique : émis au prochain SYNC après un événement |
| `0x01`–`0xF0` | synchrone cyclique : émis tous les *n* SYNC |
| `0xFC` | RTR synchrone |
| `0xFD` | RTR événementiel |
| `0xFE` | événementiel, spécifique au constructeur |
| `0xFF` | événementiel, spécifique au profil |

Les valeurs `0xF1` à `0xFB` sont réservées.

## Mapping

Une entrée de mapping est un mot de 32 bits : index sur 16 bits, sous-index sur 8 bits,
longueur en bits sur 8 bits.

```
0x60640020  →  objet 0x6064, sous-index 0, 32 bits
```

Un canal accepte au plus `OD_PDO_MAPPING_MAX` entrées, soit 8. La granularité est
l'octet.

Le service refuse une entrée de mapping dans trois cas :

| Condition | Code d'abandon |
|---|---|
| l'objet n'existe pas | `SDOAbortCode_ObjectNonExistent` |
| l'objet n'est pas mappable, ou son accès ne convient pas au sens du PDO | `SDOAbortCode_CannotMapToPDO` |
| la somme des tailles dépasse 8 octets | `SDOAbortCode_MappedPDOLengthExceeded` |

Le temps d'inhibition et la valeur de départ SYNC ne s'écrivent que sur un canal
invalide. Les bits 0 à 30 du COB-ID sont figés tant que le canal est valide.

Le remapping est appliqué tout de suite quand le service est actif. Sinon il attend
l'entrée en `Operational`.

## API applicative

```cpp
canopen.pdo.onReceive([](unsigned number) { printf("RPDO %u\n", number); });
canopen.pdo.onTimeout([](unsigned number) { printf("RPDO %u expiré\n", number); });

canopen.pdo.transmitTPDO(0);
```

| Méthode | Rôle |
|---|---|
| `transmitTPDO(unsigned number)` | déclenche un événement sur un TPDO, numéroté depuis 0 |
| `reload()` | résout à nouveau tous les mappings et les pousse au cœur CPU1 |
| `onReceive(callback)` | appelée après la réception d'un RPDO |
| `onTimeout(callback)` | appelée quand le temporisateur d'un RPDO expire |
| `tpdo(number)`, `rpdo(number)` | accès au canal, un `PdoChannel` |

!!! warning "Deux numérotations"

    `transmitTPDO()` numérote les TPDO depuis 0. Les rappels `onReceive` et `onTimeout`
    reçoivent un numéro depuis 1.

Écrivez les objets mappés avant d'appeler `transmitTPDO()`. Un TPDO événementiel part
tout de suite si le temps d'inhibition est écoulé. Un TPDO synchrone acyclique part au
prochain SYNC.

## États NMT

Le service n'émet et ne reçoit qu'en `Operational`. À la première transition vers
`PreOperational`, il appelle `reload()` et recharge tous les paramètres depuis le
dictionnaire d'objets.

## Objets distants

Un objet mappé qui vit sur le cœur CPU1 ne passe pas par le dictionnaire d'objets. Le
service utilise `getRemoteTPDO()` et `setRemoteRPDO()`, et pousse le mapping résolu
avec `configRemoteTPDO()` et `configRemoteRPDO()`. Voir
[objets distants](../generateur/objets-distants.md).
