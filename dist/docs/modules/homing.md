# homing



## Objects


### Homing current threshold

**0x2500**


| Name | Type | default |
| --- | --- | --- |
| Homing current threshold | uint32 | 0 |


### Homing move away distance

**0x2501**


| Name | Type | default |
| --- | --- | --- |
| Homing move away distance | uint32 | 0 |


### Homing method

**0x6098**
 - -2 : Mechanical end stop negative
 - -1 : Mechanical end stop positive
 -  0 : No homing method
 - 37 : Homing on current position


| Name | Type | default |
| --- | --- | --- |
| Homing method | int16 | 0 |


### Homing speed

**0x6099**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 2 |
| Speed during search for switch | uint32 | 0 |
| Speed during search for zero | uint32 | 0 |


### Homing acceleration

**0x609A**


| Name | Type | default |
| --- | --- | --- |
| Homing acceleration | uint32 | 0 |


### Supported homing methods

**0x60E3**
 - -2 : Mechanical end stop negative
 - -1 : Mechanical end stop positive
 -  0 : No homing method
 - 37 : Homing on current position


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 2 |
| ?#st suported homing method | int16 | 37 |
| 1st suported homing method | int16 | -1 |
| 2st suported homing method | int16 | -2 |

