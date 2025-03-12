# velocity-loop

[filename](../Position_Velocity.drawio ':include :type=code')

## Objects


### Velocity Control

**0x2200**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| Kp | float32 | 0 |
| Ki | float32 | 0 |
| Max sum | float32 | 0 |


### velocity-loop prescaler

**0x2201**


| Name | Type | default |
| --- | --- | --- |
| velocity-loop prescaler | uint16 | 0 |


### Velocity encoder inverted

**0x2202**


| Name | Type | default |
| --- | --- | --- |
| Velocity encoder inverted | bool | 0 |


### Following velocity window

**0x2203**


| Name | Type | default |
| --- | --- | --- |
| Following velocity window | uint32 | 0 |


### Following velocity time out

**0x2204**


| Name | Type | default |
| --- | --- | --- |
| Following velocity time out | uint16 | 0 |


### Velocity error

**0x2205**


| Name | Type | default |
| --- | --- | --- |
| Velocity error | int32 | 0 |


### Velocity demand value

**0x606B**


| Name | Type | default |
| --- | --- | --- |
| Velocity demand value | int32 | 0 |


### Velocity actual value

**0x606C**


| Name | Type | default |
| --- | --- | --- |
| Velocity actual value | int32 | 0 |


### Velocity window

**0x606D**


| Name | Type | default |
| --- | --- | --- |
| Velocity window | uint16 | 0 |


### Velocity window time

**0x606E**


| Name | Type | default |
| --- | --- | --- |
| Velocity window time | uint16 | 0 |


### Velocity threshold

**0x606F**


| Name | Type | default |
| --- | --- | --- |
| Velocity threshold | uint16 | 0 |


### Velocity threshold time

**0x6070**


| Name | Type | default |
| --- | --- | --- |
| Velocity threshold time | uint16 | 0 |


### Velocity sensor resolution

**0x6090**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 2 |
| Sensor increments | uint32 | 1 |
| motor revolutions | uint32 | 1 |


### Max slippage

**0x60F8**


| Name | Type | default |
| --- | --- | --- |
| Max slippage | int32 | 0 |


### target velocity

**0x60FF**


| Name | Type | default |
| --- | --- | --- |
| target velocity | int32 | 0 |

