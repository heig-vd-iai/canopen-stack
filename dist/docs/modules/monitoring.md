# monitoring



## Objects


### Position encoder sin signal

**0x2800**


| Name | Type | default |
| --- | --- | --- |
| Position encoder sin signal | float32 | 0 |


### Position encoder cos signal

**0x2801**


| Name | Type | default |
| --- | --- | --- |
| Position encoder cos signal | float32 | 0 |


### Velocity encoder sin signal

**0x2802**


| Name | Type | default |
| --- | --- | --- |
| Velocity encoder sin signal | float32 | 0 |


### Velocity encoder cos signal

**0x2803**


| Name | Type | default |
| --- | --- | --- |
| Velocity encoder cos signal | float32 | 0 |


### Position encoder sin corrected signal

**0x2804**


| Name | Type | default |
| --- | --- | --- |
| Position encoder sin corrected signal | float32 | 0 |


### Position encoder cos corrected signal

**0x2805**


| Name | Type | default |
| --- | --- | --- |
| Position encoder cos corrected signal | float32 | 0 |


### Velocity encoder sin corrected signal

**0x2806**


| Name | Type | default |
| --- | --- | --- |
| Velocity encoder sin corrected signal | float32 | 0 |


### Velocity encoder cos corrected signal

**0x2807**


| Name | Type | default |
| --- | --- | --- |
| Velocity encoder cos corrected signal | float32 | 0 |


### Cycle time

**0x2808**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| cycle time | float32 | 0 |
| cycle time min | float32 | 0 |
| cycle time max | float32 | 0 |


### Cycle time inner loop

**0x2809**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| cycle time | float32 | 0 |
| cycle time min | float32 | 0 |
| cycle time max | float32 | 0 |


### Cycle time outer loop

**0x2810**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| cycle time | float32 | 0 |
| cycle time min | float32 | 0 |
| cycle time max | float32 | 0 |


### Cycle time pdo

**0x2811**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| cycle time | float32 | 0 |
| cycle time min | float32 | 0 |
| cycle time max | float32 | 0 |


### I2t current value

**0x2812**


| Name | Type | default |
| --- | --- | --- |
| I2t current value | float32 | 0 |


### Trace signal

**0x2850**
 - 0 : None
 - 1 : Position demand
 - 2 : Position actual
 - 3 : Velocity demand
 - 4 : Velocity actual
 - 5 : Torque demand
 - 6 : Torque actual
 - 7 : Current demand
 - 8 : Current actual
 - 9 : Force demand
 - 10 : Force actual
 

| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 2 |
| Trace 0 signal | uint8 | 0 |
| Trace 1 signal | uint8 | 0 |


### Trigger mode

**0x2851**
 - 0 : Immediate
 - 1 : Beginning of movement
 - 2 : End of movement
 - 3 : Trace Value


| Name | Type | default |
| --- | --- | --- |
| Trigger mode | uint8 | 0 |


### Manual trigger

**0x2852**


| Name | Type | default |
| --- | --- | --- |
| Manual trigger | bool | 0 |


### Trigger value

**0x2853**


| Name | Type | default |
| --- | --- | --- |
| highest sub-index supported | uint8 | 2 |
| Trace value | float32 | 0 |
| Trace index | uint8 | 0 |


### Trigger delay ms

**0x2854**


| Name | Type | default |
| --- | --- | --- |
| Trigger delay ms | uint32 | 0 |


### Trace remaining length

**0x2855**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 4 |
| trace 0 remaining length | int16 | 0 |
| trace 1 remaining length | int16 | 0 |
| trace 2 remaining length | int16 | 0 |
| trace 3 remaining length | int16 | 0 |


### Trace a

**0x5000**


| Name | Type | default |
| --- | --- | --- |
| Trace a | domain | 0 |


### Trace b

**0x5001**


| Name | Type | default |
| --- | --- | --- |
| Trace b | domain | 0 |

