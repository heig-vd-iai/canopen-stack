# Migrer sw-motion sur la pile refactorisée

Ce document décrit le portage du firmware CM de sw-motion depuis la pile `main` (2e489c5) vers `ace_refactor` (a80e1c7). Il suit l'ordre dans lequel les étapes se débloquent.

## Ce qui change

| Avant (`main`) | Après (`ace_refactor`) |
|---|---|
| `extern Node node`, `node.init(&hardware)` | `Node` construit par l'application, ou `FullNode` |
| `HardwareInterface` unique | `CanTransport`, `Persistence`, `RemoteObjects` (`src/hal/`) |
| Services internes au nœud | `HB`, `SDO`, `PDO`, `SYNC`, `EMCY` attachés avec `attach()` |
| `usleep()` fourni par l'application | plus nécessaire, attente bornée sur `getTime_us()` |
| `canopen_generator` v1, Poetry, YAML v1 | `generator` v2, uv, YAML v2 |
| `Metadata` virtuelle en RAM | `Metadata` constexpr en flash |
| Flash : groupes en secteurs 10, 11, 12, 13, signature en 0x27FFF0 | groupes en secteurs 10, 11, 9, signature en secteur 12, secteur 13 au bootloader |
| Objets 0x1005, 0x1019 ignorés | COB-ID SYNC et overflow lus dans l'OD |
| Node guarding non supporté | supporté |

## Prérequis

1. Pousser `ace_refactor` sur `origin`.
2. Dans sw-motion, pointer le sous-module sur le nouveau commit :

```bash
git -C canopen-stack fetch
git -C canopen-stack checkout a80e1c7
git add canopen-stack
```

3. Installer uv. Poetry n'est plus utilisé par la pile.

```bash
cd canopen-stack && uv sync
```

## Étape 1 : migrer `sw-motion.yaml` vers le schéma v2

Le migrateur lit le YAML v1 et les profils v1 de la pile :

```bash
cd canopen-stack
uv run python -m generator migrate config ../sw-motion.yaml ../sw-motion.v2.yaml \
    --v1-profiles tests/fixtures/profiles-v1.yaml
```

Deux blocages connus dans `sw-motion.yaml` (177 objets, 149 `remote`) :

1. Quatre objets portent `logicalDevices: [0]` (0x6040, 0x6041 et deux autres). Supprimer ces lignes. Le seul device logique est déclaré en tête du fichier.
2. Cinq arrays 402 ont `get: none` / `set: none` sur le sous-index 0 : 0x608F, 0x6090, 0x6091, 0x6092, 0x6099. Le migrateur refuse tout accesseur sur le sous-index 0. Deux options :
   - supprimer ces deux lignes dans le YAML v1, le sous-index 0 n'a jamais de handler en v2 ;
   - corriger `_sub0()` dans `generator/migrate.py` pour ignorer les clés dont la valeur est `none`.

Vérifier le résultat avant de générer :

- même nombre d'objets dans `objects` ;
- chaque objet `remote: true` devient `remote: cpu1` ;
- les `alias` disparaissent, les noms d'instances viennent de l'index (`object6040sub0`) ;
- les `enum` gardent leur `typedef`, donc `od_enum.hpp` garde les mêmes noms.

Ensuite `sw-motion.yaml` (v1) est remplacé par `sw-motion.v2.yaml`.

## Étape 2 : générer l'OD

```bash
cd canopen-stack
uv run python -m generator generate ../sw-motion.v2.yaml -f \
    --local ../cm --remote ../cpu1 --eds ..
```

Fichiers produits :

| Côté | Fichiers |
|---|---|
| CM | `cm/od.hpp`, `cm/od.cpp`, `cm/od_lookup.hpp`, `cm/od_lookup.cpp` |
| CPU1 | `cpu1/od_remote.hpp`, `cpu1/od_enum.hpp`, `cpu1/od_modes.hpp` |
| Racine | `od.eds` |

`od_lookup.{hpp,cpp}` sont nouveaux : ils portent la table de hachage parfaite de `findObject()`. Les ajouter au projet CCS.

Mettre à jour la cible `meta` du Makefile de sw-motion :

```makefile
meta:
	cd canopen-stack && uv run python -m generator generate ../sw-motion.v2.yaml -f \
		--local ../cm --remote ../cpu1 --eds ..
```

## Étape 3 : découper `C2000Hardware`

`C2000Hardware` implémente l'ancienne `HardwareInterface`. Elle se découpe en trois objets. Deux existent déjà dans la pile.

| Ancienne méthode | Nouvelle interface | Implémentation |
|---|---|---|
| `init()` (MCAN, timer) | `CanTransport::init()` | `platform/c2000/C2000CanTransport` |
| `update()` (lecture FIFO, `node.receiveFrame`) | `CanTransport::receiveFrame()` | `C2000CanTransport` |
| `sendFrame()` | `CanTransport::sendFrame()` | `C2000CanTransport` |
| `getTime_us()` | `CanTransport::getTime_us()` | `C2000CanTransport` |
| `initFlash()`, `saveDataToFlash()`, `loadDataFromFlash()` | `Persistence::init()`, `saveGroup()`, `loadGroup()` | `platform/c2000/C2000Persistence` |
| `saveSignatureToFlash()`, `loadSignatureFromFlash()` | `Persistence::saveSignature()`, `loadSignature()` | `C2000Persistence` |
| `init()` (IPC handshake, `ipc.setReady`) | `RemoteObjects::init()` | **à écrire** : `cm/C2000RemoteObjects` |
| `update()` (`ipc.update()`), `updateError()` | `RemoteObjects::updateError()` | à écrire |
| `resetRemote()` | `RemoteObjects::resetRemote()` | à écrire |
| `getRemoteData()`, `setRemoteData()`, `*RemoteDomain()` | `RemoteObjects::getRemoteData()`, `setRemoteData()` | à écrire |
| `configRemoteRPDO()`, `configRemoteTPDO()` | idem | à écrire |
| `enablePDO()`, `disablePDO()` | idem | à écrire |
| `getRemoteTPDO()`, `setRemoteRPDO()` | idem | à écrire |

`C2000RemoteObjects` reste dans sw-motion : il dépend de `ipcCommunication.hpp` et `sharedtypes.h` de `shared/`, que la pile ne doit pas connaître. Son contenu est celui de `cm/C2000Hardware.cpp` actuel, lignes 105 à 350, avec ces adaptations :

- `node.od().getMetadata(id)->dataType == DataType::DOMAIN` : la pile ne passe plus de DOMAIN par `getRemoteData`. Les DOMAIN passent par un `DomainHandler` (voir plus bas). Supprimer `getRemoteDomain` et `setRemoteDomain` si aucun objet DOMAIN n'est `remote`.
- `node.hardware().getTime_us()` : garder une référence au `C2000CanTransport` si la mesure de cycle est conservée.
- `node.emcy().raiseError(...)` dans `updateError()` : `emcy` n'est plus accessible par le nœud. Passer une référence à l'`EMCY` de `FullNode` dans le constructeur de `C2000RemoteObjects`, ou stocker les erreurs et les lever depuis `main_cm.cpp`.
- `Monitoring` n'est plus appelé par la pile. Le garder dans `C2000RemoteObjects` ou le supprimer.

Squelette :

```cpp
#pragma once
#include "emcy/emcy.hpp"
#include "hal/remote-objects.hpp"
#include "ipcCommunication.hpp"

namespace CANopen {

class C2000RemoteObjects : public RemoteObjects {
   public:
    void bindEmergency(EMCY &service) { emcy = &service; }
    void init() override;
    void updateError() override;
    void resetRemote() override;
    int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode) override;
    int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode) override;
    void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) override;
    void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDs[]) override;
    void enablePDO() override;
    void disablePDO() override;
    void getRemoteTPDO(uint16_t pdoIndex, Data data[]) override;
    void setRemoteRPDO(uint16_t pdoIndex, Data data[]) override;

   private:
    EMCY *emcy = nullptr;
    IPC ipc;
};
}  // namespace CANopen
```

`updateError()` teste `emcy` avant de lever une erreur : la pile l'appelle depuis `Node::update()`, après `init()`.

`getRemoteData` et `setRemoteData` gardent leur contrat : `0` terminé, `1` en attente, `-1` erreur. Le serveur SDO et le PDO relancent l'appel jusqu'à la fin ou au timeout (`CANOPEN_SDO_REMOTE_TIMEOUT_US`, 100 ms).

`main_cm.cpp` devient :

```cpp
#include "C2000CanTransport.hpp"
#include "C2000Persistence.hpp"
#include "C2000RemoteObjects.hpp"
#include "full-node.hpp"
extern "C" {
#include "cm.h"
}

namespace {
CANopen::C2000CanTransport transport(OD_NODE_ID);
CANopen::C2000Persistence persistence;
CANopen::C2000RemoteObjects remote;
}  // namespace

CANopen::FullNode canopen(transport, persistence, remote);

bool rebootApplication();

int main() {
    CM_init();
    remote.bindEmergency(canopen.emcy);
    canopen.node.nmt().onReset = []() { rebootApplication(); };
    canopen.init();
    while (true) canopen.update();
}
```

Points d'attention :

- `usleep()` disparaît. La pile borne ses attentes sur `getTime_us()`.
- `onReset` est un `void (*)()`. `rebootApplication()` renvoie un `bool` ignoré.
- `init()` passe par `NMT::reset(ParameterGroup_All)` : chargement de la flash, valeurs par défaut si la signature ne correspond pas, `resetRemote()` puis `onReset()`. Comme avant, `rebootApplication()` est donc appelé une fois au démarrage. Poser `onReset` avant `init()` reproduit l'ancien comportement.

## Étape 4 : carte flash et linker

`C2000Persistence` écrit aux adresses suivantes :

| Usage | Secteur | Adresse |
|---|---|---|
| Communication (0x1010 sub2) | 10 | 0x270000 |
| Application (sub3) | 11 | 0x274000 |
| ManufacturerA (sub4) | 9 | 0x260000, 64 Kio |
| Signature | 12 | 0x278000 |
| Bootloader | 13 | réservé |

`cm/2838x_FLASH_lnk_cm.cmd` de sw-motion utilise le secteur 9 pour le code, le secteur 13 pour ManufacturerB et 0x27FFF0 pour la signature. À aligner :

- retirer `CMBANK0_SECTOR9`, `CMBANK0_SECTOR13` et `CMBANK0_SECTOR14` des sections de code ;
- placer `.text`, `.const`, `.cinit` dans les secteurs 0 à 8 ;
- ajouter `.TI.ramfunc` avec `* (.TI.ramfunc)` et `* (.text:Fapi_*)`, la pile marque ses fonctions flash ainsi.

`platform/c2000/example/flash.cmd` est le modèle à reprendre.

Un `save` sur 0x1010 sub4 efface le secteur 9. Si du code y reste, le firmware est détruit au premier `save`.

## Étape 5 : projet CCS

Dans `cm/.cproject` et `cm/.project` :

- la ressource liée `canopen` pointe déjà sur `canopen-stack/src`. Elle inclut maintenant `emcy/`, `hb/`, `nmt/`, `od/`, `pdo/`, `sdo/`, `sync/`, `utils/` ;
- ajouter une ressource liée sur `canopen-stack/platform/c2000` et exclure `platform/c2000/example` ;
- ajouter `od_lookup.cpp` aux sources du projet, à côté de `od.cpp` ;
- retirer `C2000Hardware.{hpp,cpp}` ;
- chemins d'inclusion : `canopen-stack/src` et `canopen-stack/platform/c2000` ;
- flag `--c++14` sur armcl. La pile refuse toute construction plus récente.

## Étape 6 : CPU1

`cpu1/system/communication.cpp` utilise `remoteGetter[]` et `remoteSetter[]` de `od_remote.hpp`. Le v2 génère les mêmes tables, indexées par l'id d'objet, avec les mêmes noms. `_OD_DEFINES_ONLY` n'a jamais eu d'effet, il peut rester.

Les ids changent avec l'ordre des objets. Le CM et le CPU1 doivent être regénérés ensemble, depuis le même YAML, et flashés ensemble. La signature `od_signature` protège la flash du CM, pas l'IPC.

`od_enum.hpp` et `od_modes.hpp` gardent les `typedef` du YAML. `config.hpp` et `axis.hpp` compilent sans changement si les noms sont conservés à l'étape 1.

## Étape 7 : compiler et vérifier

Sur l'hôte, avant la cible :

```bash
cd canopen-stack
make -C platform/c2000/example OD_DIR=$PWD/../cm
```

Cela compile la pile, `platform/c2000` et l'OD de sw-motion sous armcl, sans le code IPC. Les erreurs de génération apparaissent ici.

Sur la cible :

1. Flasher CPU1 puis CM.
2. Boot-up 0x701, heartbeat toutes les 1000 ms.
3. SDO sur un objet local (0x1018) puis sur un objet `remote` (0x6064).
4. NMT start, un RPDO, un TPDO sur événement.
5. `save` sur 0x1010 sub1, reset, relecture.
6. NMT reset node : `rebootApplication()` doit repartir sur CPU1.

## Hors périmètre

- Le bootloader f28388d se porte séparément : pile minimale (`make lib-minimal`), `DomainHandler` flash sur 0x1F50, Makefile sans `src/objects`.
- Les objets DOMAIN de sw-motion, s'il y en a, passent par un `DomainHandler` enregistré avec `canopen.node.odAccessor().setDomainHandler()`, plus par l'IPC.
