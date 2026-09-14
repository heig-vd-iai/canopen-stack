# CANopen

- [CANopen](#canopen)
  - [Introduction](#introduction)
  - [Getting Started](#getting-started)
    - [Generate the object dictionary](#generate-the-object-dictionary)
    - [Build and test on the host](#build-and-test-on-the-host)
  - [Documentation](#documentation)
  - [Object Dictionary](#object-dictionary)
  - [Hardware Interface](#hardware-interface)
  - [Usage Guide](#usage-guide)
  - [Adding Objects](#adding-objects)
  - [Limitations](#limitations)

## Introduction

This is a C++14 CANopen slave library. It is based on the official [CiA 301 CANopen application layer and communication profile](https://www.can-cia.org/groups/specifications/) document.

This implementation features the following:

- Object dictionary generated as static C++ tables from a YAML description of the device, with the matching EDS file and Markdown documentation.
- Non volatile storage of the object dictionary, one parameter group at a time.
- NMT slave that can be controlled by a master or by the application.
- SDO server that supports expedited, segmented and block transfers with CRC. DOMAIN objects are streamed through a `DomainHandler` (see `src/od/domainHandler.hpp`), so a firmware image never sits in RAM.
- PDO for both transmission and reception with dynamic mapping, RTR, inhibit time and event timer.
- Heartbeat producer and node guarding.
- SYNC consumer with counter that triggers synchronous TPDO transmission.
- Emergency producer with managed error register, pre-defined error field and error behaviour.
- Services are attached to the node one by one, so a bootloader can link NMT, SDO and heartbeat only.

## Getting Started

Before compiling anything, you need a YAML configuration file that describes the device's object dictionary. Why not a regular EDS? Because EDS doesn't have the full story for each object: where the data lives, how it is read and written, its unit and its enumeration.

The Python package `generator` renders the object dictionary from the configuration file (schema v2, see `examples/minimal.v2.yaml` and `examples/bootloader.v2.yaml`). Standard objects come from the profiles in `generator/profiles/`, so the configuration only lists the objects the device implements.

### Generate the object dictionary

```bash
uv sync
uv run python -m generator generate config.yaml --all dist
```

`--all DIR` renders every target under `DIR`. `--local DIR`, `--remote DIR`, `--eds DIR` and `--doc DIR` render one of them:

| Files | Target | Content |
|---|---|---|
| `cm/od.hpp`, `cm/od.cpp`, `cm/od_lookup.{hpp,cpp}` | node core | dictionary tables, `OD_OBJECT_<index>_SUB<n>` ids, perfect hash lookup |
| `cpu1/od_remote.hpp`, `cpu1/od_enum.hpp`, `cpu1/od_modes.hpp` | remote core | getters and setters of the `remote` objects, enumerations, modes of operation |
| `od.eds` | master tools | EDS file |
| `docs/*.md` | documentation | one Markdown page per module |

A configuration written for the previous schema is converted with:

```bash
uv run python -m generator migrate config old.yaml new.yaml
```

### Build and test on the host

The library targets a TMS320F28388D (Cortex-M4 core for the stack, C28x core for the real-time application), but it builds on any C++14 compiler. The host build uses the dictionary of `tests/golden/minimal/`:

```bash
make            # canopen-slave.so
make test       # build/tests (doctest)
./build/tests
make example    # build/example, a SocketCAN node on vcan0
make lib-minimal test-minimal   # NMT + SDO + heartbeat only, bootloader dictionary
```

`make generate` renders `examples/minimal.v2.yaml` into `dist/`. Set `OD_DIR` to build against another dictionary.

## Documentation

To generate the Doxygen documentation:

```bash
sudo apt install doxygen graphviz
doxygen Doxyfile
```

The generated dictionary also comes with a Markdown page per module (`docs/` target of the generator).

## Object Dictionary

The object dictionary is rendered from the YAML configuration into static tables, so it is allocated at compile time and its size is known. Every sub-index gets an id, exposed as `OD_OBJECT_<index>_SUB<n>`, that indexes the tables of `ObjectDictionnary`:

| Table | Content |
|---|---|
| `CANopenOD::objectIndexTable` | `(index, subindex)` of each id, sorted |
| `objectMetadataTable` | `Metadata` of each id, in read-only memory |
| `objectGetterTable`, `objectSetterTable` | function called to read or write each id |
| `boolTable`, `u8Table`, ..., `f64Table`, `stringTable` | the values, one table per C type |
| `dataIndexTable` | position of each id in its typed table |

`Metadata` (see `src/od/metadata.hpp`) is a plain struct built at compile time: access rights, data type, size, and pointers to the default value and to the limits. The default values are used by `restoreData()` and by the services at `init()`.

Getters and setters follow the same signature, `int8_t f(Data &data, int32_t id, SDOAbortCodes &abortCode)`, and return:

- `0`: the value was read or written.
- `1`: the access is pending, the caller has to poll again (objects served by the other core).
- `-1`: the access failed, `abortCode` tells why.

`ObjectDictionnary::readData()` and `writeData()` route an access to these tables, by id or by index and subindex. `readDataWait()` and `writeDataWait()` (see `src/od_common.hpp`) wait up to 10 ms for a pending remote access.

The supported data types and their C type:

| Code | Name           | C Type     |
| ---- | -------------- | ---------- |
| 0x01 | BOOLEAN        | `bool`     |
| 0x02 | INTEGER8       | `int8_t`   |
| 0x03 | INTEGER16      | `int16_t`  |
| 0x04 | INTEGER32      | `int32_t`  |
| 0x05 | UNSIGNED8      | `uint8_t`  |
| 0x06 | UNSIGNED16     | `uint16_t` |
| 0x07 | UNSIGNED32     | `uint32_t` |
| 0x08 | REAL32         | `float`    |
| 0x09 | VISIBLE_STRING | `char[]`   |
| 0x0F | DOMAIN         | streamed   |
| 0x11 | REAL64         | `double`   |
| 0x15 | INTEGER64      | `int64_t`  |
| 0x1B | UNSIGNED64     | `uint64_t` |

A DOMAIN object has no storage in the dictionary. The SDO server hands its data to the `DomainHandler` registered on the node, chunk by chunk, in both directions.

The standard objects the stack serves itself:

| Index | Name | Service |
| --- | --- | --- |
| 0x1000 | Device type | dictionary |
| 0x1001 | Error register | EMCY |
| 0x1003 | Pre-defined error field | EMCY |
| 0x1005 | COB-ID SYNC | SYNC |
| 0x1007 | Synchronous window length | PDO |
| 0x1010 | Store parameters | dictionary |
| 0x1011 | Restore default parameters | dictionary |
| 0x1017 | Producer heartbeat time | HB |
| 0x1018 | Identity object | dictionary |
| 0x1019 | Synchronous counter overflow value | SYNC |
| 0x1029 | Error behaviour | EMCY |
| 0x1400 to 0x15FF | RPDO communication parameter | PDO |
| 0x1600 to 0x17FF | RPDO mapping parameter | PDO |
| 0x1800 to 0x19FF | TPDO communication parameter | PDO |
| 0x1A00 to 0x1BFF | TPDO mapping parameter | PDO |

Any other object, standard or manufacturer specific, is a plain dictionary entry. The profiles in `generator/profiles/301.yaml` and `402.yaml` describe the standard ones, so listing `0x1017: {}` in the configuration is enough.

## Hardware Interface

The library is device-agnostic. The application implements three small interfaces from `src/hal/` and hands them to the node:

| Interface | Header | Role |
|---|---|---|
| `CanTransport` | `hal/can-transport.hpp` | `init`, `sendFrame`, non-blocking `receiveFrame`, microsecond clock `getTime_us` (wraps at 0xFFFFFFFF) |
| `Persistence` | `hal/persistence.hpp` | `saveGroup`/`loadGroup` per parameter group, `saveSignature`/`loadSignature` |
| `RemoteObjects` | `hal/remote-objects.hpp` | Objects served by another core: `getRemoteData`/`setRemoteData` (return 1 while pending), PDO hooks, `resetRemote`, `updateError`. `NullRemote` for single-core targets |

The node is built on top of them and `update()` drains the transport, polls the remote side and runs the services:

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

Ready-made implementations:

- `platform/linux/`: `SocketCanTransport` (SocketCAN, `MSG_DONTWAIT`, `CLOCK_MONOTONIC`) and `FilePersistence` (one `od-group<N>.dat` file per parameter group). Built by `make example` into `build/example`, which runs against `tests/golden/minimal/cm/` and the `example/example.py` master on `vcan0`.
- `platform/c2000/`: `C2000CanTransport` (MCAN0, CPUTimer0) and `C2000Persistence` (F021 Flash API, sectors 10, 11 and 9 for the parameter groups, sector 12 for the signature, sector 13 left to the bootloader). Only driverlib is needed. `platform/c2000/example/` is a TI application skeleton with its Makefile (`TOOLCHAINPATH`, `C2000WAREPATH`, `OD_DIR`). The dual-core `RemoteObjects` implementation lives in the application project because it depends on the IPC structures shared with CPU1.

Persistence images hold eight bytes per object of the parameter group, in dictionary order, so the file and flash layouts are identical.

## Usage Guide

A `Node` owns the object dictionary and the NMT state machine. The protocol services (`HB`, `SDO`, `PDO`, `SYNC`, `EMCY`) are separate objects attached to the node with `attach()`. `FullNode` (see `src/full-node.hpp`) builds and attaches all of them. An application that needs less builds the services it wants:

```cpp
#include "hb/hb.hpp"
#include "node.hpp"
#include "sdo/sdoServer.hpp"

static CANopen::Node node(transport, persistence, remote);
static CANopen::HB hb(node.od(), transport, node.nodeId);
static CANopen::SDO sdo(node.odAccessor(), transport, node.nodeId);

int main() {
    node.attach(hb);
    node.attach(sdo);
    CANopen::bindHeartbeat(hb);
    node.init();
    while (true) node.update();
}
```

`bindHeartbeat()`, `bindPdo()`, `bindSync()` and `bindEmergency()` route the objects a service owns (0x1017, 0x1400..., 0x1005, 0x1001...) to that instance. An object whose service is not bound answers `SDOAbortCode_ObjectNonExistent`. `make lib-minimal` builds this configuration against `examples/bootloader.v2.yaml` without linking PDO, SYNC or EMCY.

> [NOTE]
> Only one node per program: the dictionary tables are generated as static data and the node id comes from `od.hpp`.

There is no built-in concurrency protection. `update()` polls the transport, so a single loop is the easiest approach. If frames are received from an interrupt or a thread, queue them and feed them to `receiveFrame()` from the loop, or guard `update()` with a mutex.

Load the saved parameters before `init()`, save them from the application or through object 0x1010:

```cpp
node.od().loadData(ParameterGroup_All);
node.init();
...
node.od().saveData(ParameterGroup_Application);
```

Dictionary access goes through the `Data` union and the generated ids:

```cpp
Data value;
value.u64 = 0;
SDOAbortCodes abortCode;
if (node.od().readData(value, OD_OBJECT_6064_SUB0, abortCode) == 0)
    printf("Position %ld\n", static_cast<long>(value.i32));

value.f32 = 49.3f;
node.od().writeData(value, 0x6048, 1, abortCode);
```

The NMT state is read and changed from the application:

```cpp
if (node.nmt().getState() == NMTState_PreOperational)
    node.nmt().setTransition(NMTServiceCommand_Start);
```

The PDO service reports RPDO reception and timeout, and sends event-driven TPDOs (transmission type 0xFE or 0xFF) on request:

```cpp
canopen.pdo.onReceive([](unsigned number) { printf("RPDO %u\n", number); });
canopen.pdo.onTimeout([](unsigned number) { printf("RPDO %u timed out\n", number); });

// Write the mapped entries first, then transmit.
canopen.pdo.transmitTPDO(0);
```

The EMCY service emits EMCY messages, sets the matching bit of the error register and pushes the error to the history:

```cpp
if (voltage < MIN_VOLTAGE) canopen.emcy.raiseError(EMCYErrorCode_Voltage);

canopen.emcy.clearErrorBit(ErrorRegisterBit_Voltage);
if (canopen.emcy.getErrorRegister() == 0) canopen.emcy.clearHistory();
```

The generic bit is set with every error. Clearing the last error bit clears the register and sends an error reset message.

DOMAIN objects need a `DomainHandler` registered on the accessor before `init()`. `example/linux/main.cpp` keeps object 0x2002 in a buffer; a bootloader writes the chunks to flash.

```cpp
canopen.node.odAccessor().setDomainHandler(&domain);
```

## Adding Objects

Objects are described in the `objects` block of the YAML configuration, keyed by index. An object listed in a profile inherits its description, so `{}` or a few overrides are enough. A new object gives its name, type and access:

```yaml
profiles:
- 301
- 402
objects:
  0x1017:
    default: 1000
  0x2003:
    name: A limited value
    module: fixture
    datatype: uint16
    access: rw
    default: 50
    limits:
      min: 10
      max: 100
  0x2004:
    name: An array
    module: fixture
    sub0:
      name: Number of entries
    record:
    - name: First element
      datatype: uint32
      access: rw
      default: 1
    - name: Second element
      datatype: uint32
      access: rw
      default: 2
```

The fields of a variable:

| Field | Meaning |
|---|---|
| `datatype` | `bool`, `int8`...`int64`, `uint8`...`uint64`, `float32`, `float64`, `string`, `domain` |
| `access` | `r`, `w`, `rw` |
| `default` | initial value, restored by object 0x1011 |
| `limits` | `min` and `max`, a write outside aborts with `DownloadValueTooLow` or `TooHigh` |
| `size` | capacity of a `string` |
| `pdo` | `true` when the entry can be mapped in a PDO |
| `unit`, `scale` | physical unit and scale of the value |
| `enum` | `typedef` and `values`, rendered in `od_enum.hpp` |
| `module` | Markdown page the object is documented on |

A record or array adds `sub0` and a `record` list, one item per sub-index.

An object whose value lives on the other core is declared `remote`. `get` and `set` are C++ expressions evaluated on that core, rendered in `od_remote.hpp`; `@` stands for the written value. A `scale` converts the CANopen value to the internal one:

```yaml
  0x6064:
    remote: cpu1
    get: remote.position
  0x2005:
    name: A remote value
    remote: cpu1
    get: remote.getValue()
    set: remote.setValue(@)
    datatype: uint32
    access: rw
```

On the node core, a remote object is read and written through `RemoteObjects::getRemoteData()` and `setRemoteData()`, which return `1` until the other core answers. The SDO server and the PDO service poll on their own; the application uses `readDataWait()` and `writeDataWait()`.

A local object can also name its own handler with `get` and `set`: the generated getter and setter then call these functions, declared by the application with the common signature. This is how the services serve their objects (`hbGetData`, `pdoGetTpdoComm`...), see `src/od_common.hpp`.

After changing the configuration, render it again and rebuild. `tests/test_golden.py` compares the rendering of the example configurations with `tests/golden/`; run `uv run pytest --update-golden` after a deliberate change of the generator.

## Limitations

- The TIME object is not supported.
- PDO mapping is limited to 8 objects, as granularity is set to 8 (byte level mapping).
- COB-IDs of PDOs and SYNC are read from the dictionary; the other services use the pre-defined connection set (CiA301:2011§7.3.3).
- MPDOs are not supported.
- One SDO server, no SDO client.
- Heartbeat consumer is not supported.
- OS commands are not supported.
- LSS is not supported.
- Object flags (ObjFlags) are not supported.
- The SYNC counter follows the received value when the message carries one, otherwise it increments up to the overflow of object 0x1019 (240 when the object is absent).
- SDO block transfer PST (protocol switch threshold) is accepted but ignored: the server always answers a block request with a block transfer.
- SDO timeouts default to 1 s per transfer, 100 ms between block sub-blocks and 100 ms for a remote object. Override with `-DCANOPEN_SDO_TIMEOUT_US`, `-DCANOPEN_SDO_BLOCK_TIMEOUT_US`, `-DCANOPEN_SDO_REMOTE_TIMEOUT_US` (see `src/sdo/config.hpp`).
- VISIBLE_STRING objects have a default text and a capacity, but the SDO server does not transfer them yet.

Ignored and non-implemented objects:

- Object 1006: SYNC producer
- Object 1012: standard TIME COB-ID is used
- Object 1013: not consumed and not published
- Object 1014: standard EMCY COB-ID is used
- Object 1015: no inhibit time for EMCY
- Object 1016: heartbeat consumer
- Object 1020: manufacturer specific
- Object 1021: EDS storage
- Object 1023: OS commands
- Object 1027: modular devices
- Object 1028: emergency consumer
- Objects 1200 to 12FF: additional SDO servers and clients
- Objects 1FA0 to 1FFF: MPDO
