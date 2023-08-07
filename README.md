# CANopen
CANopen slave library

## Remarks
### Unsupported or aliased data types
- DOMAIN is not supported
- VISIBLE_STRING is UTF-8 encoded, OCTET_STRING and UNICODE_STRING are aliases to VISIBLE_STRING
- TIME_OF_DAY and TIME_DIFFERENCE are alisases to UNSIGNED64
### Tweaked or ignored objects
- Custom or rw cob-ids are not supported. Some objects are ignored/affected:
    - 1005 is ignored (COB-ID SYNC)
    - 1012 is ignored (COB-ID time stamp)
    - 1014 is ignored (COB-ID EMCY)
- Object 1006 is ignored (sync producer feature)
- Object 100D is ignored (node guarding unsupported)
- Object 1016 is ignored (heartbeat consumer feature)
- Object 1020 is ignored
- Object 1021 is ignored (DOMAIN unsupported)
- Objects 1023 to 1026 are ignored (OS commands unsupported)

- PDO mapping: I'll do it for bytes, not bits (who maps bits in PDO anyways??)
- Multiple SDOs not supported
- Multiple PDOS above 4 not supported
- Node guarding is not supported, heartbeat only
- OS commands are not supported
