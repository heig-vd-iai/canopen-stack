# Rapport du refactor de la pile CANopen

Branche `ace_refactor` comparée à `main`, état au 2026-09-18 (commit `c38c789`).

## 1. Résumé

Le refactor touche 228 fichiers en 46 commits : 31 549 lignes ajoutées, 9 210 supprimées.
Rien n'est poussé sur le dépôt distant.

Ce qui change pour vous :

- **Plus de singleton `node`.** L'application construit un `Node` et lui attache des services.
- **`HardwareInterface` est scindée en trois interfaces** : `CanTransport`, `Persistence`, `RemoteObjects`.
- **Le serveur SDO est réécrit.** Il gère le block download avec CRC et les objets DOMAIN en flux.
- **Chaque service a son dossier** : `src/sdo`, `src/pdo`, `src/nmt`, `src/emcy`, `src/hb`, `src/sync`.
- **Le générateur est remplacé.** Le paquet `generator/` utilise un schéma YAML v2 et migre les fichiers v1.
- **Les métadonnées de l'OD passent en mémoire morte.** La hiérarchie virtuelle `Metadata_*` disparaît.
- **Une pile minimale existe pour le bootloader** : NMT, SDO et heartbeat, sans PDO, SYNC ni EMCY.
- **La pile se compile et se teste sur PC** : 90 cas de test C++, 187 tests Python, une CI.
- **Le plafond de langage est C++14**, imposé par les compilateurs TI.

Bilan par dossier :

| Dossier | Fichiers | Lignes |
|---|---|---|
| `generator/` | 37 | +4363 |
| `src/sdo/` | 9 | +1211 |
| `src/pdo/` | 5 | +893 |
| `src/emcy/` | 5 | +487 |
| `src/od/` | 6 | +356 |
| `src/sync/` | 4 | +228 |
| `src/nmt/` | 4 | +208 |
| `src/hb/` | 4 | +203 |
| `src/hal/` | 3 | +110 |
| `src/utils/` | 3 | +98 |
| `platform/c2000/` | 8 | +891 |
| `platform/linux/` | 4 | +224 |
| `tests/cpp/` | 13 | +9273, dont `doctest.h` 7106 |
| `tests/golden/` | 26 | +7711 |
| `tests/` (pytest) | 25 | +2135 |
| `canopen_generator/` | 13 supprimés | −1891 |

## 2. Fichiers supprimés, déplacés, ajoutés

Git compte 33 fichiers supprimés, 155 ajoutés, 10 renommés et 17 modifiés.

Supprimés dans `src/` :

- `emergency.*`, `heartbeat.*`, `nmt.*`, `pdo.*`, `sdo.*`, `sync.*` : remplacés par les dossiers de service.
- `hardware-interface.hpp`, `hardware-delay.hpp` : remplacés par `src/hal/`.
- `iobject-dictionnary.hpp` (404 lignes) : remplacé par `src/od/metadata.hpp`.
- `units.cpp`, `units.hpp` : ils lisaient l'OD par le singleton.

Supprimés ailleurs :

- `canopen_generator/` en entier.
- `example/example.cpp`, `example/Makefile`, `example/od.hpp`.
- `poetry.lock`.

Déplacés :

- `canopen_generator/phf.py` → `generator/phf.py`.
- Les gabarits Jinja → `generator/templates/`.
- `canopen_generator/profiles.yaml` → `tests/fixtures/profiles-v1.yaml`. Ce fichier sert de source aux tests de migration.
- Le code C2000 de `example/c2000/` → `platform/c2000/`.

## 3. Architecture : du singleton aux services

Commits : `4b0d837`, `ac12950`, `4436a98`.

### Avant

- `Node()` sans argument, puis `init(HardwareInterface *)`.
- `extern Node node;` visible partout, y compris dans le code généré.
- `Node` déclarait `friend` chacun des six services.
- `Node::update()` lisait l'heure par `node.hardware()`, donc par le singleton depuis sa propre méthode.

### Après

`Node` possède l'OD, l'`ODAccessor` et la machine NMT.
Il distribue les trames, le temps et les états NMT aux services attachés.

```cpp
class Node : public ServiceBus {
    Node(CanTransport &transport, Persistence &persistence, RemoteObjects &remote);
    bool attach(Service &service);
    void init();
    ObjectDictionnary &od();
    NMT &nmt();
    ODAccessor &odAccessor();
};
```

Un service implémente `Service` (`src/service.hpp`) :

```cpp
virtual void init() {}
virtual bool consumes(FunctionCodes) const = 0;
virtual void onFrame(Frame &, uint32_t now_us) = 0;
virtual void update(uint32_t now_us) = 0;
virtual void onNmtState(NMTStates) = 0;
```

- La table de services est bornée par `CANOPEN_MAX_SERVICES`, 8 par défaut.
- Il n'y a plus aucun `friend` dans `src/`.
- `FullNode` (`src/full-node.hpp`) compose `Node`, `HB`, `SDO`, `PDO`, `SYNC` et `EMCY`.
- L'application définit elle-même son instance. La bibliothèque ne contient plus de `Node node;`.

### Liaison du code généré

Le code généré n'appelle plus `node.`.
Il appelle des fonctions libres : `hbGetData`, `pdoGetTpdoMap`, `emcySetErrorField`, `odGetSave`.
Ces fonctions sont routées vers l'instance liée dans `src/od_common.hpp` :

```cpp
bindHardware(CanTransport &, Persistence &, RemoteObjects &);
bindEmergency(EMCY &);
bindHeartbeat(HB &);
bindPdo(PDO &);
bindSync(SYNC &);
```

- Un service non lié répond `SDOAbortCode_ObjectNonExistent`.
- Une seule fonction libre existe par rôle PDO. Elle retrouve le canal depuis l'identifiant d'objet.
- Un test Python interdit la chaîne `node.` dans le code généré.

### Pile minimale

- `make lib-minimal` produit `canopen-slave-minimal.so` avec NMT, SDO et heartbeat.
- Le binaire ne contient aucun symbole PDO, SYNC ou EMCY.
- `examples/bootloader.v2.yaml` décrit son dictionnaire. L'objet 0x1001 y est local pour ne pas lier EMCY.
- `tests/cpp/minimal/test_minimal.cpp` joue un block download du firmware sur 0x1F50.

## 4. Couche matérielle

Commits : `3252091`, `602afc5`, `89195ce`, `9febf78`.

`HardwareInterface` portait 18 méthodes virtuelles. Elle est scindée en trois interfaces dans `src/hal/`.

| Interface | Méthodes | Changement |
|---|---|---|
| `CanTransport` | `init`, `sendFrame(const Frame &)`, `bool receiveFrame(Frame &)`, `uint32_t getTime_us()` | La pile tire les trames. L'ancien `update()` disparaît. |
| `Persistence` | `saveGroup`, `loadGroup`, `saveSignature`, `loadSignature` | Renommées depuis `saveDataToFlash` et voisines. |
| `RemoteObjects` | `updateError`, `resetRemote`, `getRemoteData`, `setRemoteData`, `configRemoteRPDO`, `configRemoteTPDO`, `enablePDO`, `disablePDO`, accès PDO distants | Le reset passe ici, car il traverse l'IPC. |

- `NullRemote` sert les cibles à un seul cœur. Il répond `SDOAbortCode_AccessFailedHardwareError`.
- `readDataWait` et `writeDataWait` attendent 10 ms au plus, sur l'horloge du transport. Ils remplacent `usleep` et les compteurs.

### `platform/linux`

- `SocketCanTransport` : SocketCAN non bloquant, interface `vcan0` par défaut.
- `FilePersistence` : fichiers `od-group<N>.dat` et `od-signature.dat`, même image que la flash.
- `example/linux/main.cpp` : `FullNode`, `NullRemote` et un DOMAIN de 4096 octets sur 0x2002. Cible `make example`.

### `platform/c2000`

- `C2000CanTransport` : MCAN0 en CAN classique, FIFO de réception de 16 trames, CPUTimer0 en horloge µs.
- `C2000Persistence` : API flash F021, fonctions placées en `.TI.ramfunc`.
- `platform/c2000/example/` : `main_cm.cpp`, `startup_cm.c`, `flash.cmd`, `Makefile` pour armcl.
- **`C2000RemoteObjects` n'existe pas dans ce dépôt.** Il dépend de `ipcCommunication.hpp`. Il s'écrit dans sw-motion.

Carte flash du CM (`platform/c2000/C2000Persistence.cpp`) :

| Usage | Secteur | Origine | Taille |
|---|---|---|---|
| Groupe ManufacturerA | 9 | 0x00260000 | 0x10000 |
| Groupe Communication | 10 | 0x00270000 | 0x4000 |
| Groupe Application | 11 | 0x00274000 | 0x4000 |
| Signature de l'OD | 12 | 0x00278000 | 0x4000 |
| Réservé au bootloader | 13 | 0x0027C000 | — |

## 5. Serveur SDO et accès à l'OD

Commits : `1c834f4`, `fc8a011`, `3662fac`.

- `src/sdo/` remplace `src/sdo.cpp` par cinq fichiers : `sdoServer`, `upload`, `download`, `blockUpload`, `blockDownload`.
- Une table état × commande route chaque trame.
- `sdo/frames.hpp` donne des vues typées sur les trames. Le code ne manipule plus d'octets bruts.
- Le block download vérifie le CRC-16 et reprend au bon numéro de séquence.
- `src/utils/{bits,endian,crc}.hpp` regroupent les fonctions d'octets.

`ODAccessor` (`src/od/odAccessor.hpp`) rend l'accès à l'OD asynchrone :

- États : `Idle`, `Pending`, `Done`, `Failed`.
- Méthodes : `lookup`, `startRead`, `startWrite`, `poll(now_us)`.
- Un objet distant ne bloque plus la boucle. Le serveur interroge `poll` à chaque tour.

`DomainHandler` (`src/od/domainHandler.hpp`) traite les objets DOMAIN en flux, sans copie en RAM :

```cpp
beginDownload(int32_t id, uint32_t size);
downloadChunk(id, offset, const uint8_t *, length);
endDownload(id);
abortDownload(id);
uploadSize(id);
uploadChunk(id, offset, uint8_t *, length);
```

Délais, dans `src/sdo/config.hpp`, modifiables par `-DCANOPEN_SDO_*` :

| Délai | Avant | Après |
|---|---|---|
| Transfert | 300 s | 1 s |
| Entre sous-blocs | 10 ms | 100 ms |
| Objet distant | aucun | 100 ms |

Le délai entre sous-blocs vaut 100 ms car python-canopen ne gère pas un acquittement anticipé.

## 6. Services CiA 301

Commits : `1500e69`, `6a0da3e`, `79b7faf`, `465a20e`, `fe8bd1f`, `11d2c6c`.

### PDO

- `PdoChannel` représente un PDO dans une direction. Il remplace `CommParameter` et `MapParameter`.
- `src/pdo/config.hpp` porte les constantes en `constexpr`.
- Le routage suit le COB-ID configuré, et non plus le numéro de canal.
- Le COB-ID est validé à l'écriture.
- Le choix local ou distant d'un objet mappé se lit dans les métadonnées.
- `PDO` expose `transmitTPDO(n)`, `reload()`, `onReceive`, `onTimeout`, `tpdo(n)`, `rpdo(n)`.

### NMT

- Une table de transitions remplace les branchements.
- `nmt/frames.hpp` et `nmt/states.hpp` isolent les trames et les états.

### EMCY

- Une table associe chaque code d'erreur à son bit du registre 0x1001.
- `ErrorHistory` gère l'objet 0x1003.
- Les codes vivent dans `emcy/codes.hpp`, sous l'espace de noms `CANopen`.

### Heartbeat

- Le node guarding est géré : réponse avec bit de bascule.
- Le temps producteur redémarre à l'écriture de 0x1017.

### SYNC

- Le COB-ID se lit dans 0x1005, le débordement du compteur dans 0x1019.
- `SyncListener` remplace l'appel direct de SYNC vers PDO.

### Divers

- `enums.hpp` devient un en-tête parapluie. Chaque module porte ses énumérations.
- `frame.hpp` n'a plus de macros, `frame.cpp` plus de casts.

## 7. Bugs corrigés

| Bug | Effet sur `main` | Correction |
|---|---|---|
| Inhibit time désactivé | `inhibitTime = 0;` écrit en dur dans `src/pdo.cpp` | Lu au sous-index 3, appliqué par `PDO::inhibited()` |
| Débordement des délais PDO | `getEventTimer_us()` rendait un `uint16_t` : 1000 ms devenait 17 ms | `uint32_t eventTimer_us()` et `inhibitTime_us()` dans `src/pdo/pdoChannel.hpp` |
| Tampon TPDO non initialisé | De la pile partait sur le bus sans objet distant | `Data values[pdo::MAPPING_MAX] = {}` dans `PDO::pack` |
| Crash à l'init PDO (`6ac41ab`) | `remapTPDO` lisait 8 entrées, seules les premières étaient remplies | Les 8 entrées sont initialisées, `getMetadata` et `getSize` bornent l'identifiant |
| Table de hachage variable (`f1fb03b`) | Le départage dépendait du temps de calcul de la machine | Départage sur `(B, A0, A1)`, sortie reproductible |
| Bit `writeable` faux (`778bd86`) | Il ne suivait pas le droit d'accès | Il le suit |
| Délai SDO de 300 s | Un transfert abandonné bloquait le serveur 5 minutes | 1 s |
| Identifiant MCAN mal décalé | Le code de 2024 décalait de 1 bit | Décalage de 18 bits, en émission et en réception |

Trois corrections viennent du premier essai sur cible (`e47605a`, `c38c789`) :

- **Bit timing MCAN.** L'ancien réglage donnait un point d'échantillonnage à 60 % et un SJW de 1 tq. Le commentaire annonçait 125 kbit/s. Le nouveau réglage donne 500 kbit/s, 10 tq, échantillonnage à 80 %, SJW de 2 tq. Il est aligné sur le bootloader.
- **Copie des octets de trame.** `MCAN_TxBufElement::data` est un `uint16_t data[64]` : un octet CAN par mot. Le `memcpy` entrelaçait les octets dans les deux sens. L'index SDO arrivait à 0. Des boucles avec masque `& 0xFF` le remplacent.
- **Sémaphore de pompe flash.** `init()` le prenait sans le rendre. Le CPU1 et le bootloader restaient bloqués. Une structure RAII le prend seulement pendant `saveGroup` et `saveSignature`.

`e47605a` décale aussi l'image CM dans `flash.cmd` : le vecteur de reset passe à 0x00204000 et `.text` commence au secteur 2. Les 16 premiers Kio restent au bootloader.

## 8. Dictionnaire d'objets

Commit : `70422a7`.

### Avant

- Une classe virtuelle par type : `Metadata_bool`, `Metadata_uint8_t_limited`, et ainsi de suite.
- La table contenait des pointeurs : `const Metadata *table[]`.
- Les objets de métadonnées vivaient en RAM.

### Après

`Metadata` est une structure simple, construite à la compilation :

```cpp
struct Metadata {
    Access access;
    DataType type;
    uint16_t size;
    const void *defaultValue;
    const void *low;
    const void *high;
};
```

- Fabriques : `makeMetadata`, `makeStringMetadata`, `makeDomainMetadata`.
- La table `const Metadata objectMetadataTable[]` va dans la section `.const`.
- `getMetadata()` rend un `const Metadata *`.
- `getSize()` lit `metadata.size`. Une chaîne rend donc sa capacité.
- Le générateur émet `OD_BOOL_COUNT`, `OD_U8_COUNT` et voisins. Les tables typées vides ne sont plus émises.
- `od_common.hpp` ne garde que deux macros publiques : `OD_PDO_MAPPING_MAX` et `DOMAIN_MAX_SIZE`.

## 9. Générateur

Commits : `1ca5d63` à `55e30b9`, puis `a0cb7ef`.

### Schéma

| | v1 | v2 |
|---|---|---|
| Profils | Un fichier `profiles.yaml` embarqué | `generator/profiles/301.yaml` et `402.yaml` |
| Configuration | Chaque objet répète `profile:` et `module:` | `profiles: [301, 402]`, puis seulement les objets modifiés |
| Identité | `info.device` | Bloc `device:` en snake_case |
| Validation | voluptuous | pydantic, 24 fichiers sous `generator/validation/` |

La fusion profil et configuration se fait dans `generator/loaders/config.py`.
Les erreurs de validation citent l'index en hexadécimal.

### Commandes

```bash
uv run python -m generator generate CONFIG --all DIR
uv run python -m generator migrate config SOURCE TARGET --v1-profiles FILE
uv run python -m generator migrate profiles SOURCE TARGET_DIR
```

`--all` répartit les sorties :

- `cm/` : `od.hpp`, `od.cpp`, `od_lookup.hpp`, `od_lookup.cpp`.
- `cpu1/` : `od_remote.hpp`, `od_enum.hpp`, et `od_modes.hpp` si « Modes of operation » existe.
- Racine : `od.eds`.
- `docs/` : `index.md`, une page par module, `sidebar.md`.

Les options `--local`, `--remote`, `--eds` et `--doc` choisissent un dossier par sortie.

### Code généré

- `od.cpp` n'inclut plus `node.hpp`.
- Les valeurs par défaut et les limites sortent en `constexpr`.
- Les tables des objets distants sont dimensionnées par `OD_LENGTH`.
- `od_lookup.*` est nouveau. Ajoutez-le au projet CCS.
- **La signature de l'OD change.** Au premier démarrage, `loadGroup` échoue et les valeurs par défaut s'appliquent.

### EDS

- `[DeviceInfo]` vient de `Device.to_eds_dict`.
- La clé `Alias` vide disparaît.
- Les listes d'objets obligatoires et optionnels sont numérotées à partir de 1.
- Le rendu se fait en Python. Le gabarit `od.eds.j2` disparaît.

### Outillage Python

- uv remplace Poetry : `uv sync`, `uv run pytest`.
- Le script `can-meta-generator` devient `canopen-generator`.
- Python 3.12 au minimum.

## 10. Tests et outillage

Tests C++ avec doctest, 90 cas :

| Fichier | Cas |
|---|---|
| `test_sdo.cpp` | 29 |
| `test_pdo.cpp` | 15 |
| `test_emcy.cpp` | 9 |
| `test_nmt.cpp` | 8 |
| `test_od_contract.cpp` | 8 |
| `test_hb.cpp` | 7 |
| `test_smoke.cpp` | 5 |
| `test_sync.cpp` | 5 |
| `minimal/test_minimal.cpp` | 4 |

- `tests/cpp/fake-hardware.hpp` implémente les trois interfaces matérielles. Les tests rejouent les échanges trame par trame.
- Tests Python : 187 fonctions dans 25 fichiers.
- `tests/test_golden.py` compare la sortie du générateur à `tests/golden/{minimal,bootloader}`. L'option `--update-golden` régénère la référence.

Cibles du `Makefile` :

- `lib`, `test`, `lib-minimal`, `test-minimal`, `example`, `generate`, `format`, `format-check`, `clean`.
- Compilation en `-std=c++14` avec 14 avertissements, dont `-Wconversion`, `-Wsign-conversion` et `-Wold-style-cast`.

Intégration continue, dans `.github/workflows/ci.yml` :

- `python` : ruff, black, pytest.
- `cpp` : `make lib test`, exécution des tests, `make example`.
- `format` : `make format-check`, bloquant.

`pre-commit` lance ruff, black et clang-format.

État des vérifications :

- Jusqu'au commit `11d2c6c` : tests hôte, tests minimaux, pytest, format et édition de liens armcl au vert.
- Échange réel SDO, NMT et PDO avec `example.py` sur `vcan0` au commit `ac12950`.
- Depuis `70422a7` : compilation, format et armcl seulement. **`./build/tests` et pytest n'ont pas été relancés.**
- Sur cible : premier essai le 2026-09-17, trois corrections commitées. La validation complète reste à faire.

## 11. Points ouverts

À faire hors de ce dépôt :

- Écrire `C2000RemoteObjects` dans sw-motion. Source : `example/c2000/C2000Hardware.cpp` au commit `3ca5245`.
- Migrer `sw-motion.yaml` vers le schéma v2. Le migrateur refuse deux cas : quatre objets avec `logicalDevices: [0]`, et cinq tableaux 402 avec `get/set: none` sur le sous-index 0.
- Aligner le fichier de liens CM de sw-motion : secteurs 9, 12 et 13 libres de code, section `.TI.ramfunc`.
- Porter le bootloader sur `make lib-minimal` avec un `DomainHandler` flash sur 0x1F50. Son `Makefile` référence encore `src/objects`.
- La marche à suivre est dans `docs/migration-sw-motion.md`.

À corriger dans ce dépôt :

- `src/monitoring.*` est du code mort. La pile ne l'appelle plus, mais la bibliothèque le compile encore.
- `src/od_common.cpp:249-261` : les accesseurs locaux de chaînes et de DOMAIN rendent `-1`. Le SDO ne transfère pas les `VISIBLE_STRING`.
- `README.md:56` : la commande `migrate config` omet `--v1-profiles`. Elle échoue telle quelle.
- La CI ne lance ni `lib-minimal` ni `test-minimal`, alors que le bootloader lie cette configuration.
- `pre-commit` ne formate ni `platform/` ni `example/linux/`. Le `Makefile` les formate.
- `src/canopen.hpp` n'inclut que `frame.hpp` et `node.hpp`. Son commentaire le présente comme seul en-tête à inclure. Il devrait inclure `full-node.hpp`.
- `examples/minimal.yaml` et `examples/bootloader.yaml` sont en v1. Ils ne servent qu'aux tests de migration, et rien ne le dit.
- Le profil 301 laisse 0x1017 à 0. Sans valeur sauvegardée, le nœud envoie son boot-up puis aucun heartbeat.
- L'event timer RPDO n'est pas testé : 0x1400 n'a que 3 entrées dans le dictionnaire minimal.

Actions sur le dépôt distant, à décider :

- Pousser `ace_refactor`.
- Poser les étiquettes `archive/refactoring` et `archive/refactoring-sdo`.
- Supprimer les branches locales `generator-v2`, `sdo-v2`, `hal-v2`, `services-v2`, `modules-v2`. Toutes sont ancêtres d'`ace_refactor`.
