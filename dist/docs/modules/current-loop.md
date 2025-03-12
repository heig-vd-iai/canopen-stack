# current-loop



## Objects


### Current Control

**0x2100**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| Kp | float32 | 0 |
| Ki | float32 | 0 |
| Max sum | float32 | 0 |


### dc-link voltage

**0x2101**


| Name | Type | default |
| --- | --- | --- |
| dc-link voltage | uint32 | 0 |


### dc-link min-max voltage

**0x2102**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 2 |
| Min voltage | uint32 | 0 |
| Max voltage | uint32 | 0 |


### theta invert

**0x2103**


| Name | Type | default |
| --- | --- | --- |
| theta invert | bool | 0 |


### Theta source

**0x2104**
 - 0 : Theta Estimator (need pahsing)
 - 1 : Hall sensor


| Name | Type | default |
| --- | --- | --- |
| Theta source | uint16 | 0 |


### Current demand

**0x2105**


| Name | Type | default |
| --- | --- | --- |
| Current demand | int32 | 0 |


### Torque demand

**0x6074**


| Name | Type | default |
| --- | --- | --- |
| Torque demand | int16 | 0 |


### DC link voltage

**0x6079**


| Name | Type | default |
| --- | --- | --- |
| DC link voltage | uint32 | 0 |

