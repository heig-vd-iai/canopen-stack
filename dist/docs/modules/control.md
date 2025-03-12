# control



## Objects


### Regulation switch

**0x2700**
 ### Position Switch
 - -1 : None
 -  0 : profile-position
 -  1 : Function Generator


### Velocity Switch
 - -1 : None
 -  0 : profile-velocity
 -  1 : Position Controller
 -  2 : Function Generator


### Current Switch
 - -1 : None
 -  0 : Current Target
 -  1 : profile-torque -  2 : Velocity Controller
 -  3 : Function Generator



| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 3 |
| Position switch | int8 | 0 |
| Velocity switch | int8 | 0 |
| Current switch | int8 | 0 |


### Function generator parameter

**0x2701**


| Name | Type | default |
| --- | --- | --- |
| Highest sub-index supported | uint8 | 6 |
| Waveform | uint8 | 0 |
| Amplitude | float32 | 0 |
| Frequency | float32 | 0 |
| Offset | float32 | 0 |
| Chirp start frequency | float32 | 0 |
| Chirp end frequency | float32 | 0 |


### Digital input

**0x2702**


| Name | Type | default |
| --- | --- | --- |
| Digital input | uint32 | 0 |


### Digital output

**0x2703**


| Name | Type | default |
| --- | --- | --- |
| Digital output | uint32 | 0 |

