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
### Upload (RSDO)
This is when the client wants to read data from the server's OD.

### Download (TSDO)
This is when the client wants to write data to the server's OD.

### Transfer types:
- expedited (data within single frame)
- normal (segmented)
- block
