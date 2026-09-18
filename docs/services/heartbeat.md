# Heartbeat

`HB` (voir `src/hb/hb.hpp`) est le producteur de contrôle d'erreur. Il envoie le message
de boot-up, le heartbeat périodique et répond au node guarding.

Référence : CiA 301:2011 §7.2.8.

Il n'y a pas de consommateur heartbeat.

## Construction

```cpp
static CANopen::HB hb(node.od(), transport, node.nodeId);
node.attach(hb);
CANopen::bindHeartbeat(hb);
```

## Objet servi

| Index | Nom | Type | Unité |
|---|---|---|---|
| `0x1017` | Temps du producteur heartbeat | `uint16` | ms |

La valeur par défaut de l'objet fixe la période au démarrage. Une période de `0` coupe
le heartbeat.

## Messages

| Message | Contenu de l'octet |
|---|---|
| Boot-up | `0x00` |
| Heartbeat | état NMT, bit 7 à `0` |
| Réponse au node guarding | état NMT, bit 7 alterné |

Le message de boot-up part à la première transition d'`Initialisation` vers
`PreOperational`.

Une trame RTR adressée au nœud déclenche une réponse immédiate avec le bit alterné. Le
bit repart à `0` à chaque entrée en `Initialisation`.

## API applicative

```cpp
uint16_t period = canopen.hb.producerTime_ms();
```

Pour changer la période, écrivez l'objet `0x1017`, depuis le bus ou depuis
l'application :

```cpp
Data value;
value.u16 = 500;
SDOAbortCodes abortCode;
node.od().writeData(value, 0x1017, 0, abortCode);
```

## États NMT

Le service émet dans tous les états, y compris `Stopped`. C'est le seul service qui ne
s'arrête jamais.
