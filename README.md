# CANopen
CANopen library

## Tasks
### PDO
- PDO: SYNC counter
- PDO: always 8 bytes long?
- PDO mapping sub0: SAM-MDPO and DAM-MPDO
- PDO: when remapping, check for objects actual size? (find object->entry->size)

### SDO
- SDO block transfer
- SDO error checking: may vary depending on entry type (i.e PDO mapping, see p.142)
- SDO: multiple servers?
- SDO: check toggle bit, send abort if problem

### Other
- node.cpp: make sure function code is supported
- quick search algorithm for OD entries
- node guarding?
- constantes sauvages
- NMT state and communication objects (p.80)
- implement missing data types
- group bits together for access type, PDO mapping, and others ?
- OD object value range and all other properties (p.91): PDO mapping, range
- namespaces

## Tasks (OD generator)
- perform strict parsing for mandatory or standard entries (like PDO)
    1. 0x1400 to 0x15FF: RPDO communication parameter
    2. 0x1600 to 0x17FF: RPDO mapping parameter
    3. 0x1800 to 0x19FF: TPDO communication parameter
    3. 0x1A00 to 0x1BFF: TPDO mapping parameter
- skip unsupported entries and tell why
- PDO mapping: always allocate 8 entries?
- PDO mapping: enforce correct size
- PDO: enforce PDOs 1 to 4 (standard) ?
- PDO: enforce at least 4 PDOs

## Remarks
- PDO mapping: I'll do it for bytes, not bits (who maps bits in PDO anyways??)
- Multiple SDOs not supported
- Multiple PDOS above 4 not supported

## Object & entry access
Object access:
- application
- SDO
Entry access:
- object
