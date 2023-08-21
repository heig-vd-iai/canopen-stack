# CANopen
CANopen slave library
This library uses the generic pre-defined connection set, as such, custom COB-IDs for objects are not supported (see p.80).

## Remarks
- The TIME object is not supported
- PDO mapping is limited to 8 objects, as granularity is set to 8
- Mutable COB-IDs are unsupported
- Multiple SDOs not supported
- Multiple RPDOS above default 4 are not supported
- Node guarding is not supported
- OS commands are not supported
- If object 1029 (Error behaviour) is present, only subindex 1 (communication error) is accounted for, as other entries are device specific
- The SYNC object has an internal counter that is incremented on each SYNC message. If the data contains a counter value, it is copied to the internal counter, otherwise the internal counter is incremented up to its maximum value.
The maximum value depends on whether or not object 1019 is present: if it is, the max value will be in range 2 to 240 (0 will set it to 240) based on the value of object 1019. If not, standard 240.
- LSS is not supported
- Object flags (ObjFlags) is not supported
- SDO block transfer CRC and PST not supported
### Unsupported or aliased data types
- DOMAIN is not supported
- VISIBLE_STRING is UTF-8 encoded, OCTET_STRING and UNICODE_STRING are aliases to VISIBLE_STRING
- TIME_OF_DAY and TIME_DIFFERENCE are alisases to UNSIGNED64
### Tweaked or ignored objects
- Custom or mutable cob-ids are not supported. Some objects are affected:
    - 1005 is ignored (COB-ID SYNC)
    - 1012 is ignored (COB-ID time stamp)
    - 1014 is ignored (COB-ID EMCY)
- Object 1006 is ignored (sync producer feature)
- Object 100D is ignored (node guarding unsupported)
- Object 1013 is ignored (not consumed and not published)
- Object 1015 is ignored
- Object 1016 is ignored (heartbeat consumer feature)
- Object 1020 is ignored
- Object 1021 is ignored (DOMAIN unsupported)
- Objects 1023 to 1026 are ignored (OS commands unsupported)
- Objects 1027 is ignored (modular devices unsupported)
- Object 1028 is ignored (emergency consumer feature)
- Objects 1200 to 127F are ignored (multiple SDO unsupported)
- Objects 1280 to 12FF are ignored (multiple SDO unsupported)
