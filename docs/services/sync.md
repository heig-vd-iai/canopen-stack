# SYNC

`SYNC` (voir `src/sync/sync.hpp`) est le consommateur SYNC. Il n'y a pas de producteur
SYNC.

Référence : CiA 301:2011 §7.2.5.

## Construction

```cpp
static CANopen::SYNC sync(node.od(), &pdo);
node.attach(sync);
CANopen::bindSync(sync);
```

Le second argument est un `SyncListener`. Le service PDO en est un. Passez `nullptr`
quand aucun service n'écoute.

```cpp
class SyncListener {
   public:
    virtual void onSync(uint8_t counter, uint32_t now_us) = 0;
};
```

## Objets

| Index | Nom | Rôle |
|---|---|---|
| `0x1005` | COB-ID SYNC | COB-ID écouté, entrée ordinaire du dictionnaire d'objets |
| `0x1019` | Débordement du compteur SYNC | servi par le service |

Le COB-ID est relu à chaque entrée en `PreOperational`. Sans objet `0x1005`, le service
écoute le COB-ID par défaut `0x80`.

!!! note "Valeurs valides du débordement"

    `0x1019` accepte `0`, ou une valeur entre `2` et `240`. `0` coupe le compteur. Une
    valeur invalide est ramenée à `0` au démarrage.

## Compteur

Le compteur suit deux règles :

- Le message SYNC porte un compteur : le service reprend la valeur reçue.
- Le message SYNC ne porte rien : le service incrémente son compteur, jusqu'au
  débordement de l'objet `0x1019`, ou jusqu'à 240 quand l'objet est absent.

## API applicative

```cpp
canopen.sync.onSync([](unsigned counter) { printf("SYNC %u\n", counter); });
```

| Méthode | Rôle |
|---|---|
| `onSync(callback)` | rappel exécuté à chaque SYNC, avant l'écouteur |
| `cobId()` | COB-ID écouté |
| `counterOverflow()` | valeur de débordement courante |

!!! warning "Chemin de réception"

    Le rappel s'exécute dans le chemin de réception des trames. Gardez-le court.

## États NMT

Le service est actif en `PreOperational` et en `Operational`. Il ignore les messages
SYNC dans les autres états.

## Effet sur les PDO

Chaque SYNC déclenche l'émission des TPDO synchrones et le déballage des RPDO
synchrones reçus depuis le SYNC précédent. Voir [PDO](pdo.md).
