# Interfaces matérielles

La pile ne connaît aucun périphérique. L'application implémente trois interfaces de
`src/hal/` et les passe au nœud.

| Interface | En-tête | Rôle |
|---|---|---|
| `CanTransport` | `hal/can-transport.hpp` | accès au bus CAN et horloge |
| `Persistence` | `hal/persistence.hpp` | stockage non volatile du dictionnaire d'objets |
| `RemoteObjects` | `hal/remote-objects.hpp` | objets servis par le cœur CPU1 |

## CanTransport

```cpp
class CanTransport {
   public:
    virtual ~CanTransport() = default;
    virtual void init() = 0;
    virtual void sendFrame(const Frame &frame) = 0;
    virtual bool receiveFrame(Frame &frame) = 0;
    virtual uint32_t getTime_us() = 0;
};
```

- `receiveFrame()` ne bloque jamais. Elle rend `true` quand une trame est écrite dans
  `frame`, `false` quand la file est vide.
- `getTime_us()` est une horloge relative en microsecondes. Elle doit être monotone et
  reboucler à `0xFFFFFFFF`.

!!! warning "Horloge"

    Toute la pile mesure le temps avec `getTime_us()`. Les temporisateurs PDO, les
    délais SDO et le heartbeat en dépendent.

## Persistence

```cpp
class Persistence {
   public:
    virtual ~Persistence() = default;
    virtual void init() = 0;
    virtual bool saveGroup(uint8_t parameterGroup) = 0;
    virtual bool loadGroup(uint8_t parameterGroup) = 0;
    virtual bool saveSignature(uint64_t signature) = 0;
    virtual bool loadSignature(uint64_t &signature) = 0;
};
```

- Un groupe de paramètres a une image. Voir
  [dictionnaire d'objets](dictionnaire.md).
- La signature identifie le dictionnaire d'objets. `loadData()` compare la signature
  lue à `od_signature` et rejette une image écrite par un autre firmware.
- L'image contient huit octets par objet du groupe, dans l'ordre du dictionnaire
  d'objets.

## RemoteObjects

```cpp
class RemoteObjects {
   public:
    virtual ~RemoteObjects() = default;
    virtual void init() = 0;
    virtual void updateError() = 0;
    virtual void resetRemote() = 0;
    virtual int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode) = 0;
    virtual int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode) = 0;
    virtual void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void enablePDO() = 0;
    virtual void disablePDO() = 0;
    virtual void getRemoteTPDO(uint16_t pdoIndex, Data data[]) = 0;
    virtual void setRemoteRPDO(uint16_t pdoIndex, Data data[]) = 0;
};
```

| Méthode | Appelée par |
|---|---|
| `updateError()` | `Node::update()`, à chaque cycle |
| `resetRemote()` | NMT, sur un reset node |
| `getRemoteData()`, `setRemoteData()` | dictionnaire d'objets, pour un objet distant |
| `configRemoteRPDO()`, `configRemoteTPDO()` | PDO, après chaque remapping |
| `enablePDO()`, `disablePDO()` | PDO, à l'entrée et à la sortie d'`Operational` |
| `getRemoteTPDO()`, `setRemoteRPDO()` | PDO, à chaque émission et réception |

`getRemoteData()` et `setRemoteData()` rendent `0` quand l'accès est terminé, `1` tant
qu'il est en attente et `-1` en cas d'échec.

## NullRemote

`NullRemote` est l'implémentation pour une cible à un seul cœur. Tout accès distant
échoue avec `SDOAbortCode_AccessFailedHardwareError`. Les crochets PDO ne font rien.

```cpp
static CANopen::NullRemote remote;
```

## Implémentations fournies

| Dossier | Transport | Persistance |
|---|---|---|
| `platform/linux/` | `SocketCanTransport` | `FilePersistence` |
| `platform/c2000/` | `C2000CanTransport` | `C2000Persistence` |

Aucune implémentation de `RemoteObjects` n'est fournie pour la cible bi-cœur. Elle vit
dans le projet applicatif, car elle dépend des structures IPC partagées avec le cœur
CPU1.

Voir [Linux](../plateformes/linux.md), [C2000](../plateformes/c2000.md) et
[porter la pile](../plateformes/porter.md).
