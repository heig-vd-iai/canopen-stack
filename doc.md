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
|  1st Standard error field | uint32 |
|  2nt Standard error field | uint32 |
|  3th Standard error field | uint32 |
|  4th Standard error field | uint32 |
|  5th Standard error field | uint32 |
|  6th Standard error field | uint32 |
|  7th Standard error field | uint32 |
|  8th Standard error field | uint32 |
| **1007**| Synchronous window length |
|  Synchronous window length | uint32 |
| **1008**| Manufacturer device name |
|  Manufacturer device name | string |
| **1009**| Manufacturer hardware version |
|  Manufacturer hardware version | string |
| **100A**| Manufacturer software version |
|  Manufacturer software version | string |
| **1010**| Store parameters |
|  highest sub-index supported | uint8 |
|  save all parameters | uint32 |
|  save communication parameters | uint32 |
|  save application parameters | uint32 |
|  save manufacturer specific parameters A | uint32 |
|  save manufacturer specific parameters B | uint32 |
| **1011**| Restore default parameters |
|  highest sub-index supported | uint8 |
|  restore all parameters | uint32 |
|  restore communication parameters | uint32 |
|  restore application parameters | uint32 |
|  restore manufacturer specific parameters A | uint32 |
|  restore manufacturer specific parameters B | uint32 |
| **1017**| Producer hearbeat time |
|  Producer hearbeat time | uint16 |
| **1018**| Identity object |
|  Number of entries | uint8 |
|  Vendor-ID | uint32 |
|  Product code | uint32 |
|  Revision number | uint32 |
|  Serial number | uint32 |
| **1019**| Synchronous counter overflow value |
|  Synchronous counter overflow value | uint8 |
| **1020**| Verify configuration |
|  Highest sub-index supported | uint8 |
|  Configuration date | uint32 |
|  Configuration time | uint32 |
| **1029**| Error behavior |
|  Highest sub-index supported | uint32 |
|  Communication error | uint8 |
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
| **1403**| Receive PDO 4 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
| **1600**| Receive PDO 1 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1601**| Receive PDO 2 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1602**| Receive PDO 3 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1603**| Receive PDO 4 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1800**| Transmit PDO 1 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
|  Inhibit time | uint16 |
|  Compatibility entry | uint8 |
|  Event timer | uint16 |
|  Sync start value | uint8 |
| **1801**| Transmit PDO 2 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
|  Inhibit time | uint16 |
|  Compatibility entry | uint8 |
|  Event timer | uint16 |
|  Sync start value | uint8 |
| **1802**| Transmit PDO 3 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
|  Inhibit time | uint16 |
|  Compatibility entry | uint8 |
|  Event timer | uint16 |
|  Sync start value | uint8 |
| **1803**| Transmit PDO 4 communication parameter |
|  Number of of entries | uint8 |
|  COB-ID | uint32 |
|  Transmission type | uint8 |
|  Inhibit time | uint16 |
|  Compatibility entry | uint8 |
|  Event timer | uint16 |
|  Sync start value | uint8 |
| **1A00**| Transmit PDO 1 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1A01**| Transmit PDO 2 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1A02**| Transmit PDO 3 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1A03**| Transmit PDO 4 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
## Profile 402 Profile for drives and motion control
| Index | Name |
| --- | --- |
| **6007**| Abort connection option code |
|  Abort connection option code | int16 |
| **603F**| Error code |
|  Error code | uint16 |
| **6040**| Controlword |
|  Controlword | uint16 |
| **6041**| Statusword |
|  Statusword | uint16 |
| **6042**| vl target velocity |
|  vl target velocity | int16 |
| **6043**| vl velocity demand |
|  vl velocity demand | int16 |
| **6044**| vl velocity actual value |
|  vl velocity actual value | int16 |
| **6046**| vl velocity min max amount |
|  Highest sub-index supported | uint8 |
|  vl velocity min amount | uint32 |
|  vl velocity max amount | uint32 |
| **6048**| vl velocity acceleration |
|  Highest sub-index supported | uint8 |
|  Delta speed | uint32 |
|  Delta time | uint16 |
| **6049**| vl velocity deceleration |
|  Highest sub-index supported | uint8 |
|  Delta speed | uint32 |
|  Delta time | uint16 |
| **604A**| vl velocity quick stop |
|  Highest sub-index supported | uint8 |
|  Delta speed | uint32 |
|  Delta time | uint16 |
| **604B**| vl set-point factor |
|  Highest sub-index supported | uint8 |
|  vl set-point factor numerator | int16 |
|  vl set-point factor denominator | int16 |
| **604C**| vl dimension factor |
|  Highest sub-index supported | uint8 |
|  vl dimension factor numerator | int32 |
|  vl dimension factor denominator | int32 |
| **605A**| Quick stop option code |
|  Quick stop option code | int16 |
| **605B**| Shutdown option code |
|  Shutdown option code | int16 |
| **605C**| Disable operation option code |
|  Disable operation option code | int16 |
| **605D**| Halt option code |
|  Halt option code | int16 |
| **605E**| Fault reaction option code |
|  Fault reaction option code | int16 |
| **6060**| Modes of operation |
|  Modes of operation | int8 |
| **6061**| Modes of operation display |
|  Modes of operation display | int8 |
| **6062**| Position demand value |
|  Position demand value | int32 |
| **6063**| Position actual value |
|  Position actual value | int32 |
| **6064**| Position actual value |
|  Position actual value | int32 |
| **6065**| Following error window |
|  Following error window | uint32 |
| **6066**| Following error time out |
|  Following error time out | uint16 |
| **6067**| Position window |
|  Position window | uint32 |
| **6068**| Position window time |
|  Position window time | uint16 |
| **6069**| Velocity sensor actual value |
|  Velocity sensor actual value | int32 |
| **606A**| Sensor selection code |
|  Sensor selection code | int16 |
| **606B**| Velocity demand value |
|  Velocity demand value | int32 |
| **606C**| Velocity actual value |
|  Velocity actual value | int32 |
| **606D**| Velocity window |
|  Velocity window | uint16 |
| **606E**| Velocity window time |
|  Velocity window time | uint16 |
| **606F**| Velocity threshold |
|  Velocity threshold | uint16 |
| **6070**| Velocity threshold time |
|  Velocity threshold time | uint16 |
| **6071**| Target torque |
|  Target torque | int16 |
| **6072**| Max torque |
|  Max torque | uint16 |
| **6073**| Max current |
|  Max current | uint16 |
| **6074**| Torque demand |
|  Torque demand | int16 |
| **6075**| motor rated current |
|  motor rated current | uint32 |
| **6076**| motor rated torque |
|  motor rated torque | uint32 |
| **6077**| Torque actual value |
|  Torque actual value | int16 |
| **6078**| Current actual value |
|  Current actual value | int16 |
| **6079**| DC link voltage |
|  DC link voltage | uint32 |
| **6087**| Torque slope |
|  Torque slope | uint32 |
| **6088**| Torque profile type |
|  Torque profile type | int16 |
| **607A**| Target position |
|  Target position | int32 |
| **607B**| Position range limit |
|  Highes sub-index supported | uint8 |
|  Min position range limit | int32 |
|  Max position range limit | int32 |
| **607D**| Software position limit |
|  Highes sub-index supported | uint8 |
|  Min position limit | int32 |
|  Max position limit | int32 |
| **607E**| Polarity |
|  Polarity | int8 |
| **607F**| Max profile velocity |
|  Max profile velocity | uint32 |
| **6080**| Max motor speed |
|  Max motor speed | uint32 |
| **6081**| Profile velocity |
|  Profile velocity | uint32 |
| **6082**| End velocity |
|  End velocity | uint32 |
| **6083**| Profile acceleration |
|  Profile acceleration | uint32 |
| **6084**| Profile deceleration |
|  Profile deceleration | uint32 |
| **6085**| Quick stop deceleration |
|  Quick stop deceleration | uint32 |
| **6086**| Motion profile type |
|  Motion profile type | int16 |
| **608F**| Position encoder resolution |
|  Highes sub-index supported | uint8 |
|  Encoder increments | uint32 |
|  motor revolutions | uint32 |
| **6090**| Velocity sensor resolution |
|  Highes sub-index supported | uint8 |
|  Sensor increments | uint32 |
|  motor revolutions | uint32 |
| **6091**| Gear ratio |
|  Highes sub-index supported | uint8 |
|  Motor shaft revolutions | uint32 |
|  Driving shaft revolutions | uint32 |
| **6092**| Feed constant |
|  Highes sub-index supported | uint8 |
|  Feed | uint32 |
|  Shaft revolutions | uint32 |
| **6096**| Velocity factor |
|  Highes sub-index supported | uint8 |
|  Numerator | uint32 |
|  Denominator | uint32 |
| **6097**| Acceleration factor |
|  Highes sub-index supported | uint8 |
|  Numerator | uint32 |
|  Denominator | uint32 |
| **6098**| Homing method |
|  Homing method | int8 |
| **6099**| Homing speed |
|  Highes sub-index supported | uint8 |
|  Speed during search for switch | uint32 |
|  Speed during search for zero | uint32 |
| **609A**| Homing acceleration |
|  Homing acceleration | uint32 |
| **60A2**| Jerk factor |
|  Highes sub-index supported | uint8 |
|  Numerator | uint32 |
|  Denominator | uint32 |
| **60A3**| Profile jerk use |
|  Profile jerk use | uint8 |
| **60A4**| Profile jerk |
|  Highes sub-index supported | uint8 |
|  profile jerk 1 | uint32 |
|  profile jerk 2 | uint32 |
|  profile jerk 3 | uint32 |
|  profile jerk 4 | uint32 |
|  profile jerk 5 | uint32 |
|  profile jerk 6 | uint32 |
| **60A8**| SI unit Position |
|  SI unit Position | uint32 |
| **60A9**| SI unit Velocity |
|  SI unit Velocity | uint32 |
| **60AA**| SI unit Acceleration |
|  SI unit Acceleration | uint32 |
| **60AB**| SI unit Jerk |
|  SI unit Jerk | uint32 |
| **60C5**| Max acceleration |
|  Max acceleration | uint32 |
| **60C6**| Max deceleration |
|  Max deceleration | uint32 |
| **607C**| Home offset |
|  Home offset | int32 |
| **60B0**| Position offset |
|  Position offset | int32 |
| **60B1**| Velocity offset |
|  Velocity offset | int32 |
| **60B2**| Torque offset |
|  Torque offset | int16 |
| **60B8**| Touch probe function |
|  Touch probe function | uint16 |
| **60B9**| Touch probe status |
|  Touch probe status | uint16 |
| **60BA**| Touch probe 1 positive edge |
|  Touch probe 1 positive edge | int32 |
| **60BB**| Touch probe 1 negative edge |
|  Touch probe 1 negative edge | int32 |
| **60BC**| Touch probe 2 positive edge |
|  Touch probe 2 positive edge | int32 |
| **60BD**| Touch probe 2 negative edge |
|  Touch probe 2 negative edge | int32 |
| **60C0**| Interpolation sub mode select |
|  Interpolation sub mode select | int16 |
| **60C1**| Interpolation data record |
|  Highes sub-index supported | int8 |
|  Interpolation data record 1 | int32 |
| **60C2**| Interpolation time period |
|  Highes sub-index supported | uint8 |
|  Interpolation time period | uint8 |
|  Interpolation time index | int16 |
| **60C4**| Interpolation data configuration |
|  Highes sub-index supported | uint8 |
|  Maximum buffer size | uint32 |
|  Acutal buffe size | uint32 |
|  Buffer organisation | uint8 |
|  Buffer position | uint16 |
|  Size of data record | uint8 |
|  Buffer clear | uint8 |
| **60D0**| Touch probe source |
|  Highes sub-index supported | int8 |
|  Touch probe 1 | int16 |
| **60D1**| Touch probe time stamp 1 positive value |
|  Touch probe time stamp 1 positive value | uint32 |
| **60D2**| Touch probe time stamp 1 negative value |
|  Touch probe time stamp 1 negative value | uint32 |
| **60D3**| Touch probe time stamp 2 positive value |
|  Touch probe time stamp 2 positive value | uint32 |
| **60D4**| Touch probe time stamp 2 negative value |
|  Touch probe time stamp 2 negative value | uint32 |
| **60D5**| Touch probe 1 positive edge counter |
|  Touch probe 1 positive edge counter | uint16 |
| **60D6**| Touch probe 1 negative edge counter |
|  Touch probe 1 negative edge counter | uint16 |
| **60D7**| Touch probe 2 positive edge counter |
|  Touch probe 2 positive edge counter | uint16 |
| **60D8**| Touch probe 2 negative edge counter |
|  Touch probe 2 negative edge counter | uint16 |
| **60E0**| Positive torque limit value |
|  Positive torque limit value | uint16 |
| **60E1**| Negative torque limit value |
|  Negative torque limit value | uint16 |
| **60E3**| Supported homing methods |
|  Highes sub-index supported | uint8 |
|  Homing method 1 | int8 |
| **60E4**| Additional position actual value |
|  Highes sub-index supported | uint8 |
|  1st additional position actual value | int32 |
| **60E5**| Additional velocity actual value |
|  Highes sub-index supported | uint8 |
|  1st additional velocity actual value | int32 |
| **60E6**| Additional position encoder resolution - encoder increments |
|  Highes sub-index supported | uint8 |
|  1st additional position encoder increments | uint32 |
| **60E7**| Additional velocity encoder resolution - encoder increments per second |
|  Highes sub-index supported | uint8 |
|  1st additional velocity encoder increments per second | uint32 |
| **60E8**| Additional gear ratio - motor shaft revolutions |
|  Highes sub-index supported | uint8 |
|  1st additional gear ratio motor shaft revolutions | uint32 |
| **60E9**| Additional feed constant - feed |
|  Highes sub-index supported | uint8 |
|  1st additional feed constant feed | uint32 |
| **60EA**| Commutation angle |
|  Commutation angle | uint16 |
| **60EB**| Additional position encoder resolution - motor revolutions |
|  Highes sub-index supported | uint8 |
|  1st additional position motor revolutions | uint32 |
| **60EC**| Additional velocity encoder resolution - motor revolutions per second |
|  Highes sub-index supported | uint8 |
|  1st additional velocity motor revolutions per second | uint32 |
| **60ED**| Additional gear ratio - driving shaft revolutions |
|  Highes sub-index supported | uint8 |
|  1st additional gear ratio driving shaft revolutions | uint32 |
| **60EE**| Additional feed constant - driving shaft revolutions |
|  Highes sub-index supported | uint8 |
|  1st additional feed constant shaft revolutions | uint32 |
| **60F2**| Positioning option code |
|  Positioning option code | uint16 |
| **60F4**| Following error actual value |
|  Following error actual value | int32 |
| **60F8**| Max slippage |
|  Max slippage | int32 |
| **60FA**| Control effort |
|  Control effort | int32 |
| **60FC**| Position demand internal value |
|  Position demand internal value | int32 |
| **60FD**| Digital inputs |
|  Digital inputs | uint32 |
| **60FE**| Digital outputs |
|  Highes sub-index supported | uint8 |
|  Pyhsical outputs | uint32 |
|  Bit mask mask | uint32 |
| **60FF**| target velocity |
|  target velocity | int32 |
| **6402**| Motor type |
|  Motor type | uint16 |
| **6403**| Motor catalogue number |
|  Motor catalogue number | string |
| **6404**| Motor manufacturer |
|  Motor manufacturer | string |
| **6405**| http motor catalogue address |
|  http motor catalogue address | string |
| **6406**| Motor calibration date |
|  Motor calibration date | uint64 |
| **6407**| Motor service periode |
|  Motor service periode | uint32 |
| **6502**| Supported drive modes |
|  Supported drive modes | uint32 |
| **6503**| Drive calatogue number |
|  Drive calatogue number | string |
| **6505**| http drive catalogue address |
|  http drive catalogue address | string |
| **67FE**| Version number |
|  Version number | uint32 |

# Object Dictionary

## Object 1000 Device type | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Device type | uint32 | 0x00000192 | none | none |
## Object 1001 Error register | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Error register | uint8 | 0 | none | none |
## Object 1002 Manufacturer status register | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Manufacturer status register | uint32 | 0 | none | none |
## Object 1003 Pre-defined error field | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of errors | uint8 | 0 | none | none |
| 1st Standard error field | uint32 | 0 | none | none |
| 2nt Standard error field | uint32 | 0 | none | none |
| 3th Standard error field | uint32 | 0 | none | none |
| 4th Standard error field | uint32 | 0 | none | none |
| 5th Standard error field | uint32 | 0 | none | none |
| 6th Standard error field | uint32 | 0 | none | none |
| 7th Standard error field | uint32 | 0 | none | none |
| 8th Standard error field | uint32 | 0 | none | none |
## Object 1010 Store parameters | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| highest sub-index supported | uint8 | 5 | node.od().getSave | node.od().saveData |
| save all parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save communication parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save application parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save manufacturer specific parameters A | uint32 | 0 | node.od().getSave | node.od().saveData |
| save manufacturer specific parameters B | uint32 | 0 | node.od().getSave | node.od().saveData |
## Object 1011 Restore default parameters | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| highest sub-index supported | uint8 | 5 | node.od().getRestore | node.od().restoreData |
| restore all parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore communication parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore application parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore manufacturer specific parameters A | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore manufacturer specific parameters B | uint32 | 0 | node.od().getRestore | node.od().restoreData |
## Object 1018 Identity object | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of entries | uint8 | 4 | none | none |
| Vendor-ID | uint32 | 0 | none | none |
| Product code | uint32 | 0 | none | none |
| Revision number | uint32 | 0 | none | none |
| Serial number | uint32 | 0 | none | none |
## Object 1019 Synchronous counter overflow value | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Synchronous counter overflow value | uint8 | 0 | node.sync().getData | node.sync().setData |
## Object 1400 Receive PDO 1 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 2 | node.pdo().rpdos[0].commParameter.getData | node.pdo().rpdos[0].commParameter.setData |
| COB-ID | uint32 | 2147484161 | node.pdo().rpdos[0].commParameter.getData | node.pdo().rpdos[0].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().rpdos[0].commParameter.getData | node.pdo().rpdos[0].commParameter.setData |
## Object 1401 Receive PDO 2 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 2 | node.pdo().rpdos[1].commParameter.getData | node.pdo().rpdos[1].commParameter.setData |
| COB-ID | uint32 | 2147484417 | node.pdo().rpdos[1].commParameter.getData | node.pdo().rpdos[1].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().rpdos[1].commParameter.getData | node.pdo().rpdos[1].commParameter.setData |
## Object 1402 Receive PDO 3 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 2 | node.pdo().rpdos[2].commParameter.getData | node.pdo().rpdos[2].commParameter.setData |
| COB-ID | uint32 | 2147484673 | node.pdo().rpdos[2].commParameter.getData | node.pdo().rpdos[2].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().rpdos[2].commParameter.getData | node.pdo().rpdos[2].commParameter.setData |
## Object 1403 Receive PDO 4 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 2 | node.pdo().rpdos[3].commParameter.getData | node.pdo().rpdos[3].commParameter.setData |
| COB-ID | uint32 | 2147484929 | node.pdo().rpdos[3].commParameter.getData | node.pdo().rpdos[3].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().rpdos[3].commParameter.getData | node.pdo().rpdos[3].commParameter.setData |
## Object 1600 Receive PDO 1 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().rpdos[0].mapParameter.getData | node.pdo().rpdos[0].mapParameter.setData |
## Object 1601 Receive PDO 2 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().rpdos[1].mapParameter.getData | node.pdo().rpdos[1].mapParameter.setData |
## Object 1602 Receive PDO 3 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().rpdos[2].mapParameter.getData | node.pdo().rpdos[2].mapParameter.setData |
## Object 1603 Receive PDO 4 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().rpdos[3].mapParameter.getData | node.pdo().rpdos[3].mapParameter.setData |
## Object 1800 Transmit PDO 1 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 6 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| COB-ID | uint32 | 2147484033 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| Inhibit time | uint16 | 0 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| Compatibility entry | uint8 | 0 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| Event timer | uint16 | 0 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
| Sync start value | uint8 | 0 | node.pdo().tpdos[0].commParameter.getData | node.pdo().tpdos[0].commParameter.setData |
## Object 1801 Transmit PDO 2 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 6 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| COB-ID | uint32 | 2147484289 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| Inhibit time | uint16 | 0 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| Compatibility entry | uint8 | 0 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| Event timer | uint16 | 0 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
| Sync start value | uint8 | 0 | node.pdo().tpdos[1].commParameter.getData | node.pdo().tpdos[1].commParameter.setData |
## Object 1802 Transmit PDO 3 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 6 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| COB-ID | uint32 | 2147484545 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| Inhibit time | uint16 | 0 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| Compatibility entry | uint8 | 0 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| Event timer | uint16 | 0 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
| Sync start value | uint8 | 0 | node.pdo().tpdos[2].commParameter.getData | node.pdo().tpdos[2].commParameter.setData |
## Object 1803 Transmit PDO 4 communication parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 6 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| COB-ID | uint32 | 2147484801 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| Transmission type | uint8 | 0 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| Inhibit time | uint16 | 0 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| Compatibility entry | uint8 | 0 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| Event timer | uint16 | 0 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
| Sync start value | uint8 | 0 | node.pdo().tpdos[3].commParameter.getData | node.pdo().tpdos[3].commParameter.setData |
## Object 1A00 Transmit PDO 1 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().tpdos[0].mapParameter.getData | node.pdo().tpdos[0].mapParameter.setData |
## Object 1A01 Transmit PDO 2 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().tpdos[1].mapParameter.getData | node.pdo().tpdos[1].mapParameter.setData |
## Object 1A02 Transmit PDO 3 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().tpdos[2].mapParameter.getData | node.pdo().tpdos[2].mapParameter.setData |
## Object 1A03 Transmit PDO 4 mapping parameter | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of of entries | uint8 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 1st object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 2nd object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 3rd object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 4th object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 5th object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 6th object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 7th object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
| 8th object to be mapped | uint32 | 0 | node.pdo().tpdos[3].mapParameter.getData | node.pdo().tpdos[3].mapParameter.setData |
## Object 6040 Controlword | state-machine

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Controlword | uint16 | 0 | sys.axis[0].getControlword | sys.axis[0].setControlword |
## Object 6041 Statusword | state-machine

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Statusword | uint16 | 0 | sys.axis[0].getStatusword | none |
## Object 6060 Modes of operation | state-machine

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Modes of operation | int8 | 0 | sys.axis[0].getOperationMode | sys.axis[0].setOperationMode |
## Object 6061 Modes of operation display | state-machine

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Modes of operation display | int8 | 0 | sys.axis[0].getOperationModeDisplay | none |
## Object 6062 Position demand value | position-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Position demand value | int32 | 0 | sys.axis[0].getDemandPosition | none |
## Object 6064 Position actual value | position-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Position actual value | int32 | 0 | sys.axis[0].getActualPosition | none |
## Object 606B Velocity demand value | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Velocity demand value | int32 | 0 | sys.axis[0].getDemandVelocity | none |
## Object 606C Velocity actual value | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Velocity actual value | int32 | 0 | sys.axis[0].getActualVelocity | none |
## Object 6071 Target torque | torque-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Target torque | int16 | 0 | sys.axis[0].getTargetTorque | sys.axis[0].setTargetTorque |
## Object 6075 motor rated current | motor

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| motor rated current | uint32 | 0 | sys.config.getMotorRatedCurrent | sys.config.setMotorRatedCurrent |
## Object 6076 motor rated torque | motor

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| motor rated torque | uint32 | 0 | sys.config.getMotorRatedTorque | sys.config.setMotorRatedTorque |
## Object 6077 Torque actual value | torque-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Torque actual value | int16 | 0 | sys.axis[0].getActualTorque | none |
## Object 6078 Current actual value | torque-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Current actual value | int16 | 0 | sys.axis[0].getActualCurrent | none |
## Object 607A Target position | position-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Target position | int32 | 0 | sys.axis[0].getTargetPosition | sys.axis[0].setTargetPosition |
## Object 6080 Max motor speed | motor

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Max motor speed | uint32 | 0 | sys.config.getMaxMotorSpeed | sys.config.setMaxMotorSpeed |
## Object 6081 Profile velocity | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Profile velocity | uint32 | 0 | sys.axis[0].getProfileVelocity | sys.axis[0].setProfileVelocity |
## Object 6083 Profile acceleration | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Profile acceleration | uint32 | 0 | sys.axis[0].getProfileAcceleration | sys.axis[0].setProfileAcceleration |
## Object 6084 Profile deceleration | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Profile deceleration | uint32 | 0 | sys.axis[0].getProfileDeceleration | sys.axis[0].setProfileDeceleration |
## Object 608F Position encoder resolution | position-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Highes sub-index supported | uint8 | 2 | none | none |
| Encoder increments | uint32 | 1 | sys.config.getPositionEncoderIncrements | sys.config.setPositionEncoderIncrements |
| motor revolutions | uint32 | 1 | sys.config.getPositionEncoderMotorRevolutions | sys.config.setPositionEncoderMotorRevolutions |
## Object 60A8 SI unit Position | position-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Position | uint32 | 4244701184 | none | none |
## Object 60A9 SI unit Velocity | velocity-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Velocity | uint32 | 4244701952 | none | none |
## Object 60AA SI unit Acceleration | velocity-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Acceleration | uint32 | 4244723456 | none | none |
## Object 60AB SI unit Jerk | torque-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Jerk | uint32 | 4244742144 | none | none |
