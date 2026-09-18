# Linux

`platform/linux/` fournit un transport SocketCAN et une persistance sur fichiers. Ces
implémentations servent au développement, aux tests et aux démonstrations.

## SocketCanTransport

```cpp
class SocketCanTransport : public CanTransport {
   public:
    explicit SocketCanTransport(const char *interface = "vcan0");
    void setInterface(const char *interface);
    bool isOpen() const;
    void init() override;
    void sendFrame(const Frame &frame) override;
    bool receiveFrame(Frame &frame) override;
    uint32_t getTime_us() override;
};
```

- `receiveFrame()` interroge la socket avec `MSG_DONTWAIT`.
- `getTime_us()` lit `CLOCK_MONOTONIC`.
- `setInterface()` change l'interface avant `init()`.
- `isOpen()` dit si la socket est ouverte. Vérifiez-la après `init()`.

L'implémentation est mono-fil.

## FilePersistence

```cpp
class FilePersistence : public Persistence {
   public:
    explicit FilePersistence(const char *directory = ".");
};
```

| Fichier | Contenu |
|---|---|
| `od-group<N>.dat` | image d'un groupe de paramètres |
| `od-signature.dat` | signature du dictionnaire d'objets |

L'image contient huit octets par objet du groupe, dans l'ordre du dictionnaire
d'objets. C'est la même disposition que celle écrite en mémoire flash par
`C2000Persistence`.

## Compiler

```bash
make example
```

La cible produit `build/example` depuis `example/linux/main.cpp` et
`platform/linux/*.cpp`. Le dictionnaire d'objets par défaut est
`tests/golden/minimal/cm`.

| Variable | Défaut | Rôle |
|---|---|---|
| `OD_DIR` | `tests/golden/minimal/cm` | dossier du dictionnaire d'objets |
| `CXX` | `g++` | compilateur |

## Lancer

```bash
./build/example vcan0
```

Le programme enregistre un `DomainHandler` pour l'objet `0x2002`, initialise le nœud,
puis boucle avec une pause de 200 µs. `Ctrl-C` l'arrête.

## Interface CAN

=== "Interface virtuelle"

    ```bash
    ./example/vcan0.sh
    ```

=== "Interface physique"

    ```bash
    ./example/can0.sh
    ```

## Maître de test

`example/example.py` pilote le nœud avec la bibliothèque `canopen`.

```bash
cd example
uv run python example.py
```

Le script écrit l'objet `0x6060`, configure le premier TPDO puis passe le nœud en
`Operational`. Des sections commentées montrent le block download, le block upload, la
sauvegarde des paramètres et les requêtes RTR.

!!! note "Dépendance"

    `canopen` fait partie du groupe `dev` de `pyproject.toml`. `uv sync` l'installe.
