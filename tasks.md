## Tasks (C++)
### PDO
- SAM-MDPO and DAM-MPDO
- RPDO
- multiple PDOs ?

### SDO
- SDO block transfer

### TIME
- implementation

### Other
- p.103: The default values shall be set valid after the CANopen device is reset (NMT service reset node for sub-index from 01h to 7Fh, NMT service reset communication for sub-index 02h) or power cycled
- p.112, 1019: A change of the value shall be responded with a SDO abort (abort code: 0800 0022h or 0800 0000h) in case the sync cycle period is unequal to 0
- node.cpp: make sure function code is supported (filters)
- node.cpp: don't mix sync and emcy messages
- implement all predefined objects
- OD object value range and all other properties (p.91): PDO mapping, range
- save, load and restore, objects 1010 and 1011, restore default from file ?
- remove const and only keep ro in AccessType union ?
- create predefined frames inheriting from base Frame ?
- group bits together for access type, PDO mapping, and others ?
- node guarding ?
- SYNC overflow ?
- 1013: High res timestamp supported ?
- how to perform reset ?

## Tasks (OD generator)
- CLI: EDS file name and node ID
- comment classes and functions
- PDO mapping: enforce correct size ?
- PDO: enforce PDOs 1 to 4 (standard) ?
