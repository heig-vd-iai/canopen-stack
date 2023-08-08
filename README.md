# CANopen
CANopen slave library
This library uses the generic pre-defined connection set, as such, custom COB-IDs for objects are not supported (see p.80).

## Remarks
- PDO mapping is limited to 8 objects, minimum size of a mapped object is 1 byte and not 1 bit
- Mutable COB-IDs are unsupported
- Multiple SDOs not supported
- Multiple PDOS above 4 not supported
- Node guarding is not supported
- OS commands are not supported
### Unsupported or aliased data types
- DOMAIN is not supported
- VISIBLE_STRING is UTF-8 encoded, OCTET_STRING and UNICODE_STRING are aliases to VISIBLE_STRING
- TIME_OF_DAY and TIME_DIFFERENCE are alisases to UNSIGNED64
### Tweaked or ignored objects
- Custom or mutable cob-ids are not supported. Some objects are affected:
    - 1005 is ignored (COB-ID SYNC)
    - 1012 is ignored (COB-ID time stamp)
    - 1014 is ignored (COB-ID EMCY)
    - 1400 to 15FF: sub1 tweaked to be const (mutable COB-IDs unsupported)
    - 1800 to 19FF: sub1 tweaked to be const (mutable COB-IDs unsupported)
- Object 1006 is ignored (sync producer feature)
- Object 100D is ignored (node guarding unsupported)
- Object 1016 is ignored (heartbeat consumer feature)
- Object 1020 is ignored
- Object 1021 is ignored (DOMAIN unsupported)
- Objects 1023 to 1026 are ignored (OS commands unsupported)
- Objects 1027 is ignored (modular devices unsupported)
- Object 1028 is ignored (emergency consumer feature)
- Objects 1200 to 127F are ignored (multiple SDO unsupported)
- Objects 1280 to 12FF are ignored (multiple SDO unsupported)
