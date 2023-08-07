## Tasks
### PDO
- PDO mapping sub0: SAM-MDPO and DAM-MPDO
- RPDO
- mappable objects/entries
- multiple PDOs

### SDO
- SDO block transfer

### EMCY
- manufacturer error code
- link to 1003
- 1015
- 1029

### SYNC
- 1019

### TIME
- 

### Other
- save, load and restore, objects 1010 and 1011, restore default from file ?
- p.103: The default values shall be set valid after the CANopen device is reset (NMT service reset node for sub-index from 01h to 7Fh, NMT service reset communication for sub-index 02h) or power cycled
- p.112, 1019: A change of the value shall be responded with a SDO abort (abort code: 0800 0022h or 0800 0000h) in case the sync cycle period is unequal to 0
- 1001: error register
- remove #include <cstdio> <cstdlib>
- node.cpp: make sure function code is supported (filters)
- implement all predefined objects
- OD object value range and all other properties (p.91): PDO mapping, range
- remove const access type and only keep ro ?
- create predefined frames inheriting from base Frame ?
- group bits together for access type, PDO mapping, and others ?
- node guarding ?
- SYNC overflow ?
- 1013: High res timestamp supported ?

## Tasks (OD generator)
- CLI
- print messages uniformity, use function ?
- make sure objects are ordered (sort by index)
- perform strict parsing for mandatory or standard entries (like PDO)
    1. 0x1400 to 0x15FF: RPDO communication parameter
    2. 0x1600 to 0x17FF: RPDO mapping parameter
    3. 0x1800 to 0x19FF: TPDO communication parameter
    3. 0x1A00 to 0x1BFF: TPDO mapping parameter
    4. 0x1003: Pre-defined error fields, make sure correct access
- PDO mapping: always allocate 8 entries?
- PDO mapping: enforce correct size
- PDO: enforce PDOs 1 to 4 (standard) ?
- PDO: enforce at least 4 PDOs?
- PDO parameter: make sure cob-id is correct and set to ro (no dynamic changing)
- custom or rw cob-ids are not supported (pdo, emcy, sync, ...)
