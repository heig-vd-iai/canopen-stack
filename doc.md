# Standard Profile Definitions

## Profile 301 Application layer and communication profile
| Index | Name |
| --- | --- |
| **1000**| Device type |
|  Device type | uint32 |
| **1001**| Error register |
|  Error register | uint8 |
| **1002**| Manufacturer status register |
|  Manufacturer status register | uint32 |
| **1003**| Pre-defined error field |
|  Number of errors | uint8 |
|  Standard error field | uint32 |
|  Standard error field | uint32 |
| **1010**| Store parameters |
|  Number of entries | uint8 |
|  save all parameters | uint32 |
|  save communication parameters | uint32 |
|  save application parameters | uint32 |
|  save manufacturer specific parameters | uint32 |
| **1011**| Restore default parameters |
|  Number of entries | uint8 |
|  restore all parameters | uint32 |
|  restore communication parameters | uint32 |
|  restore application parameters | uint32 |
|  restore manufacturer specific parameters | uint32 |
| **1018**| Identity object |
|  Number of entries | uint8 |
|  Vendor-ID | uint32 |
|  Product code | uint32 |
|  Revision number | uint32 |
|  Serial number | uint32 |
| **1019**| Synchronous counter overflow value |
|  Synchronous counter overflow value | uint8 |
| **1400**| Receive PDO 1 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
| **1401**| Receive PDO 2 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
| **1402**| Receive PDO 3 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
## Profile 402 Profile for drives and motion control
| Index | Name |
| --- | --- |
| **6040**| Controlword |
|  Controlword | uint16 |
| **6041**| Statusword |
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
## Object 1400 Receive PDO 1 communication parameter | communication

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 2
| COB-ID | uint32 | 2147484161
| Transmission type | uint8 | 0
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
