# Porter la pile

La pile ne dépend d'aucun périphérique. Pour une nouvelle cible, écrivez vos propres
implémentations des interfaces de `src/hal/`.

## 1. Écrire le transport

```cpp
#include "hal/can-transport.hpp"

class MyTransport : public CANopen::CanTransport {
   public:
    void init() override;
    void sendFrame(const CANopen::Frame &frame) override;
    bool receiveFrame(CANopen::Frame &frame) override;
    uint32_t getTime_us() override;
};
```

Règles :

- `receiveFrame()` ne bloque jamais. Elle rend `false` quand aucune trame n'attend.
- `getTime_us()` est monotone et reboucle à `0xFFFFFFFF`.
- `init()` configure le contrôleur CAN et l'horloge.

Filtrez la réception sur les COB-ID du nœud quand le contrôleur le permet. Cela réduit
le travail de `Node::update()`.

## 2. Écrire la persistance

```cpp
#include "hal/persistence.hpp"

class MyPersistence : public CANopen::Persistence {
   public:
    void init() override;
    bool saveGroup(uint8_t parameterGroup) override;
    bool loadGroup(uint8_t parameterGroup) override;
    bool saveSignature(uint64_t signature) override;
    bool loadSignature(uint64_t &signature) override;
};
```

Une image contient huit octets par objet du groupe, dans l'ordre du dictionnaire
d'objets. Parcourez `CANopenOD::objectIndexTable` et filtrez avec `inParameterGroup()`.

Sans mémoire non volatile, rendez `false` partout. Le nœud restaurera les valeurs par
défaut à chaque démarrage.

## 3. Choisir les objets distants

=== "Cible à un cœur"

    Utilisez `NullRemote`. Aucun objet de la configuration ne porte `remote`.

    ```cpp
    static CANopen::NullRemote remote;
    ```

=== "Cible bi-cœur"

    Implémentez `RemoteObjects` au-dessus de votre canal IPC.

    ```cpp
    class MyRemote : public CANopen::RemoteObjects { /* ... */ };
    ```

    `getRemoteData()` et `setRemoteData()` rendent `1` tant que la réponse n'arrive pas.
    Ne bloquez jamais dedans.

## 4. Assembler le nœud

```cpp
static MyTransport transport;
static MyPersistence persistence;
static CANopen::NullRemote remote;
static CANopen::FullNode canopen(transport, persistence, remote);

int main() {
    canopen.init();
    while (true) canopen.update();
}
```

Pour une pile plus petite, construisez et attachez vos services vous-même. Voir
[vue d'ensemble](../architecture/vue-ensemble.md).

## 5. Compiler

Les chemins d'inclusion minimaux :

| Chemin | Contenu |
|---|---|
| `src/` | la pile |
| dossier du dictionnaire d'objets | `od.hpp`, `od_lookup.hpp` |
| dossier de la plateforme | vos implémentations |

Les sources à compiler :

- `src/*.cpp` et `src/*/*.cpp` ;
- les `.cpp` du dictionnaire d'objets ;
- vos implémentations.

Le fichier `src/common.hpp` définit `LIKELY` et `UNLIKELY`. Le `Makefile` le force avec
`-include common.hpp`. Faites de même, ou incluez-le vous-même.

Options du `Makefile` à reprendre :

```
-std=c++14 -Isrc -I$(OD_DIR) -include common.hpp
```

!!! warning "C++14"

    Les compilateurs TI fixent le plafond du langage. N'introduisez aucune construction
    plus récente dans la pile ni dans vos implémentations.

## 6. Régler la pile

| Macro | Défaut | Rôle |
|---|---|---|
| `CANOPEN_MAX_SERVICES` | `8` | nombre de services attachables |
| `CANOPEN_SDO_TIMEOUT_US` | `1000000` | délai d'un transfert SDO |
| `CANOPEN_SDO_BLOCK_TIMEOUT_US` | `100000` | délai entre deux sous-blocs |
| `CANOPEN_SDO_REMOTE_TIMEOUT_US` | `100000` | délai d'un accès distant |
| `CANOPEN_SDO_BLOCK_SIZE` | `127` | segments par bloc |

Sur une cible à mémoire vive limitée, réduisez `CANOPEN_SDO_BLOCK_SIZE`. Le tampon de
bloc occupe `CANOPEN_SDO_BLOCK_SIZE * 7` octets.

## 7. Vérifier

1. Le nœud émet le message de boot-up après `init()`.
2. Le maître lit l'objet `0x1000` par SDO.
3. Le heartbeat arrive à la période de l'objet `0x1017`.
4. Un cycle sauvegarde puis redémarrage conserve les paramètres.

Prenez `platform/linux/` comme référence : c'est la plus courte des deux
implémentations fournies.
