# Premier nœud

Ce guide lance un nœud CANopen sur PC, au-dessus de SocketCAN.

## 1. Générer le dictionnaire d'objets

La pile a besoin d'un dictionnaire d'objets avant toute compilation. Le générateur le
rend depuis un fichier YAML de schéma v2.

```bash
uv run python -m generator generate examples/minimal.v2.yaml -f --all dist
```

Le dossier `dist/` contient alors :

| Chemin | Contenu |
|---|---|
| `dist/cm/` | `od.hpp`, `od.cpp`, `od_lookup.hpp`, `od_lookup.cpp` |
| `dist/cpu1/` | `od_remote.hpp`, `od_enum.hpp`, `od_modes.hpp` |
| `dist/od.eds` | fichier EDS pour les outils maître |
| `dist/docs/` | une page Markdown par module |

La même commande est disponible avec `make generate`.

## 2. Compiler l'exemple

```bash
make example
```

La cible produit `build/example`. Elle compile la pile contre le dictionnaire d'objets
de `tests/golden/minimal/cm`. Changez de dictionnaire d'objets avec la variable
`OD_DIR` :

```bash
make example OD_DIR=dist/cm
```

## 3. Préparer l'interface CAN

=== "Interface virtuelle"

    ```bash
    ./example/vcan0.sh
    ```

=== "Interface physique"

    ```bash
    ./example/can0.sh
    ```

## 4. Lancer le nœud

```bash
./build/example vcan0
```

Le programme affiche le numéro du nœud puis tourne jusqu'à `Ctrl-C`. Sans argument, il
ouvre `vcan0`.

## 5. Piloter le nœud depuis un maître

Le script `example/example.py` utilise la bibliothèque `canopen` et le fichier EDS de
référence.

```bash
cd example
uv run python example.py
```

Le script écrit l'objet `0x6060`, configure le premier TPDO, puis passe le nœud en
état `Operational`. Il affiche les TPDO reçus et les messages EMCY.

!!! note "Chemin de l'EDS"

    `example.py` charge `../tests/golden/minimal/od.eds`. Lancez-le depuis le dossier
    `example/`.

## Structure d'un programme

```cpp
#include "FilePersistence.hpp"
#include "SocketCanTransport.hpp"
#include "full-node.hpp"
#include "hal/remote-objects.hpp"

static CANopen::SocketCanTransport transport("vcan0");
static CANopen::FilePersistence persistence;
static CANopen::NullRemote remote;
static CANopen::FullNode canopen(transport, persistence, remote);

int main() {
    canopen.init();
    while (true) canopen.update();
}
```

`FullNode` construit et attache les cinq services. Pour une pile plus petite, lisez la
[vue d'ensemble](../architecture/vue-ensemble.md).
