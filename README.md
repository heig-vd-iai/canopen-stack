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
- PDO
- variable PDO mapping? 
- replace getSize() of object by a size field (useful for string)
- implement missing data types
- SDO error checking: may vary depending on entry type (i.e PDO mapping, see p.142)
- group bits together for access type, PDO mapping, and others ?
- OD object value range

## Tasks (OD generator)
- perform strict parsing for mandatory or standard entries (like PDO)
- skip unsupported entries and tell why
