# Standard Profile Definitions

## Profile 301 Application layer and communication profile
| Index | Name |
| --- | --- |
| **4096**| Device type |
|  Device type | uint32 |
| **4097**| Error register |
|  Error register | uint8 |
| **4098**| Manufacturer status register |
|  Manufacturer status register | uint32 |
| **4099**| Pre-defined error field |
|  Number of errors | uint8 |
|  Standard error field | uint32 |
|  Standard error field | uint32 |
| **5120**| RPDO Communication parameter |
|  RPDO Communication parameter | uint8 |
## Profile 402 Profile for drives and motion control
| Index | Name |
| --- | --- |
| **24640**| Controlword |
|  Controlword | uint16 |
| **24641**| Statusword |
|  Statusword | uint16 |

# Object Dictionary

## Object 1000 Device type | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Device type | uint32 | 0
- getter 
    - none
- setter 
    - none
## Object 1001 Error register | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Error register | uint8 | 0
- getter 
    - none
- setter 
    - none
## Object 1002 Manufacturer status register | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Manufacturer status register | uint32 | 0
- getter 
    - none
- setter 
    - none
## Object 1003 Pre-defined error field | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of errors | uint8 | 0
| Standard error field | uint32 | 0
| Standard error field | uint32 | 0
- getter 
    - none
- setter 
    - none
## Object 1400 RPDO Communication parameter | communication

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| RPDO Communication parameter | uint8 | 0
- getter 
    - none
- setter 
    - none
## Object 2000 break | break-system
break system for stop the motor when it's needed
[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| break | uint16 | 0
| break force | uint32 | 0
- getter 
    - foo
- setter 
    - toto
## Object 6040 Controlword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Controlword | uint16 | 10
- getter 
    - sys.axis[0].getControlWord()
- setter 
    - sys.axis[0].setControlWord()
## Object 6041 Statusword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Statusword | uint16 | 0
- getter 
    - sys.axis[0].getStatusWord()
- setter 
    - none
## Object 6840 Controlword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Controlword | uint16 | 10
- getter 
    - sys.axis[1].getControlWord()
- setter 
    - sys.axis[1].setControlWord()
## Object 6841 Statusword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Statusword | uint16 | 0
- getter 
    - sys.axis[1].getStatusWord()
- setter 
    - none
