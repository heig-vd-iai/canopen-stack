# EMCY

`EMCY` (voir `src/emcy/emcy.hpp`) est le producteur d'urgence. Il n'y a pas de
consommateur EMCY.

Référence : CiA 301:2011 §7.2.7.

## Construction

```cpp
static CANopen::EMCY emcy(node.od(), transport, node.nmt(), node.nodeId);
node.attach(emcy);
CANopen::bindEmergency(emcy);
```

Le service a besoin de la machine d'états NMT pour appliquer le comportement en cas
d'erreur.

## Objets servis

| Index | Nom |
|---|---|
| `0x1001` | Registre d'erreurs |
| `0x1003` | Champ d'erreurs prédéfini |
| `0x1029` | Comportement en cas d'erreur |

## Registre d'erreurs

Chaque code d'erreur arme un bit du registre, selon sa classe.

| Plage de codes | Bit | Nom |
|---|---|---|
| `0x2000`–`0x2FFF` | 1 | `ErrorRegisterBit_Current` |
| `0x3000`–`0x3FFF` | 2 | `ErrorRegisterBit_Voltage` |
| `0x4000`–`0x4FFF` | 3 | `ErrorRegisterBit_Temperature` |
| `0x8100`–`0x81FF` | 4 | `ErrorRegisterBit_Communication` |
| `0xF000`–`0xFEFF` | 7 | `ErrorRegisterBit_Manufacturer` |
| `0xFF00`–`0xFFFF` | 5 | `ErrorRegisterBit_DeviceProfile` |
| autres | 0 | `ErrorRegisterBit_Generic` |

Le bit générique est armé avec chaque erreur. Effacer le dernier bit spécifique efface
le registre entier et envoie un message de remise à zéro.

## Historique

L'objet `0x1003` garde huit entrées, la plus récente en premier. Une entrée est un mot
de 32 bits : le code constructeur dans les 16 bits de poids fort, le code d'erreur dans
les 16 bits de poids faible.

Deux erreurs identiques consécutives ne créent qu'une entrée. Écrire `0` dans le
sous-index 0 efface l'historique.

## Comportement en cas d'erreur

L'objet `0x1029` fixe la réaction du nœud.

| Sous-index | Erreur concernée |
|---|---|
| 1 | erreur de communication, codes `0x8100`–`0x81FF` |
| 2 | erreur de l'appareil, tous les autres codes |

| Valeur | Nom | Effet |
|---|---|---|
| `0x00` | `ErrorBehaviorValue_PreOperational` | passe en `PreOperational` si le nœud est en `Operational` |
| `0x01` | `ErrorBehaviorValue_None` | aucun changement d'état |
| `0x02` | `ErrorBehaviorValue_Stop` | passe en `Stopped` |

Sans objet `0x1029` dans le dictionnaire d'objets, réglez le comportement depuis
l'application :

```cpp
canopen.emcy.setErrorBehavior(ErrorBehaviorValue_None, ErrorBehaviorValue_Stop);
```

## API applicative

```cpp
if (voltage < MIN_VOLTAGE) canopen.emcy.raiseError(EMCYErrorCode_Voltage);

canopen.emcy.clearErrorBit(ErrorRegisterBit_Voltage);
if (canopen.emcy.getErrorRegister() == 0) canopen.emcy.clearHistory();
```

| Méthode | Rôle |
|---|---|
| `raiseError(uint16_t code, uint16_t manufacturerCode = 0)` | arme le bit, empile l'erreur, envoie le message, applique le comportement |
| `clearErrorBit(ErrorRegisterBits bit)` | efface un bit du registre |
| `clearErrorBit(EMCYErrorCodes code)` | efface le bit que ce code arme |
| `getErrorRegister()` | valeur courante du registre |
| `history()` | historique, un `ErrorHistory` |
| `clearHistory()` | vide l'historique |
| `reset()` | remet le registre à zéro sans envoyer de message |

Les codes d'erreur sont déclarés dans `src/emcy/codes.hpp`.

## États NMT

Le service n'émet qu'en `PreOperational` et en `Operational`. `raiseError()` ne fait
rien dans les autres états.
