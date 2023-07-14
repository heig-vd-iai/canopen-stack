## CANopen objects:
- NMT
- SYNC
- EMCY
- TIME
- SDO
- PDO
- HEARTBEAT

## NMT states:
- Initialisation
    - Inititlising
    - ResetApp
    - ResetComm
- PreOperational
- Operational
- Stopped

## NMT state transition:
- reception of an NMT service used for node control services
- hardware reset
- locally initiated by application events

## OD Object types (with code):
- VAR (0x07)
- ARRAY (0x08)
- RECORD (0x09)

## OD data types:
- BOOLEAN
- INT8
- INT16
- INT32
- INT64
- UINT8
- UINT16
- UINT32
- UINT64
- REAL32
- REAL64
- VISIBLE_STRING
- OCTET_STRING
- UNICODE_STRING
- TIME_OF_DAY
- TIME_DIFFERENCE

## PDO
### Transmission
This is when the node sends a PDO on the network.
It can be triggered on timer event, application event, RTR or SYNC.
Transmission type: 
- 0: after SYNC, if event occured
- 1: after SYNC
- n: on every n-th SYNC

### Reception
This is when the node receives a PDO.
It should emit an application event (callback)

## SDO
### Upload
This is when the client wants to read data from the server's OD.

### Download
This is when the client wants to write data to the server's OD.

### Transfer types:
- expedited (data within single frame)
- normal (segmented)
- block

# Cahier des charges
- Developper stack CanOpen compatible CiA402 supportant
  - NMT
  - SYNC
  - HEARTBEAT
  - SDO
  - PDO
- Avoir une architecture objet en C++
  - Driver avec D-CAN (mailbox? interuupt?)
  - Cia 301? 
- Gérer le OD: 
  - Nom 
  - ID
  - Type
  - Min
  - Max
  - Default
  - RW
  - ...
- Interactiona vec le CPU1


# A faire après
- Nice to have !! Travail supplémentaire
  - Stack TCP/IP
  - Socket ouvert entre un terminal et le CM avec une interface REPL
    > power on
    Drive is powered
    > move 10
    Moving
    Movement done
    > axis.currentcontrol.kp
    12.5

- PAge web 
