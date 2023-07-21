# CANopen
CANopen library

## Tasks
- replace #pragma once by #ifndef .... for compatibility?
- check for correct types when needed
- SDO block transfer
- quick search algorithm for OD entries
- node guarding?
- constantes sauvages
- NMT state and communication objects (p.80)
- PDO: always 8 bytes long?
- PDO mapping sub0: SAM-MDPO and DAM-MPDO
- variable PDO mapping? 
- implement missing data types
- SDO error checking: may vary depending on entry type (i.e PDO mapping, see p.142)
- group bits together for access type, PDO mapping, and others ?
- OD object value range and all other properties (p.91)
- namespaces

## Tasks (OD generator)
- perform strict parsing for mandatory or standard entries (like PDO)
    1. 0x1400 to 0x15FF: RPDO communication parameter
    2. 0x1600 to 0x17FF: RPDO mapping parameter
    3. 0x1800 to 0x19FF: TPDO communication parameter
    3. 0x1A00 to 0x1BFF: TPDO mapping parameter
- skip unsupported entries and tell why

## Remarks
- PDO mapping: I'll do it for bytes, not bits (who maps bits in PDO anyways??)
