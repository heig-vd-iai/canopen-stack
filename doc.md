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
| **1a00**| Transmit PDO 1 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1a01**| Transmit PDO 2 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1a02**| Transmit PDO 3 mapping parameter |
|  Number of of entries | uint8 |
|  1st object to be mapped | uint32 |
|  2nd object to be mapped | uint32 |
|  3rd object to be mapped | uint32 |
|  4th object to be mapped | uint32 |
|  5th object to be mapped | uint32 |
|  6th object to be mapped | uint32 |
|  7th object to be mapped | uint32 |
|  8th object to be mapped | uint32 |
| **1a03**| Transmit PDO 4 mapping parameter |
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
| **6040**| Controlword |
|  Controlword | uint16 |
| **6041**| Statusword |
|  Statusword | uint16 |
| **6060**| Modes of operation |
|  Modes of operation | int8 |
| **6061**| Modes of operation display |
|  Modes of operation display | int8 |
| **6062**| Position demand value |
|  Position demand value | int32 |
| **6064**| Position actual value |
|  Position actual value | int32 |
| **606b**| Velocity demand value |
|  Velocity demand value | int32 |
| **606c**| Velocity actual value |
|  Velocity actual value | int32 |
| **6071**| Target torque |
|  Target torque | int16 |
| **6075**| motor rated current |
|  motor rated current | uint32 |
| **6076**| motor rated torque |
|  motor rated torque | uint32 |
| **6077**| Torque actual value |
|  Torque actual value | int16 |
| **6078**| Current actual value |
|  Current actual value | int16 |
| **607a**| Target position |
|  Target position | int32 |
| **6080**| Max motor speed |
|  Max motor speed | uint32 |
| **6081**| Profile velocity |
|  Profile velocity | uint32 |
| **6083**| Profile acceleration |
|  Profile acceleration | uint32 |
| **6084**| Profile deceleration |
|  Profile deceleration | uint32 |
| **608f**| Position encoder resolution |
|  Highes sub-index supported | uint8 |
|  Encoder increments | uint32 |
|  motor revolutions | uint32 |
| **60a8**| SI unit Position |
|  SI unit Position | uint32 |
| **60a9**| SI unit Velocity |
|  SI unit Velocity | uint32 |
| **60aa**| SI unit Acceleration |
|  SI unit Acceleration | uint32 |
| **60ab**| SI unit Jerk |
|  SI unit Jerk | uint32 |

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
## Object 1010 Store parameters | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of entries | uint8 | 4
| save all parameters | uint32 | 0
| save communication parameters | uint32 | 0
| save application parameters | uint32 | 0
| save manufacturer specific parameters | uint32 | 0
- getter 
    - node.od().getSave
- setter 
    - node.od().saveData
## Object 1011 Restore default parameters | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of entries | uint8 | 4
| restore all parameters | uint32 | 0
| restore communication parameters | uint32 | 0
| restore application parameters | uint32 | 0
| restore manufacturer specific parameters | uint32 | 0
- getter 
    - node.od().getRestore
- setter 
    - node.od().restoreData
## Object 1018 Identity object | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of entries | uint8 | 4
| Vendor-ID | uint32 | 0
| Product code | uint32 | 0
| Revision number | uint32 | 0
| Serial number | uint32 | 0
- getter 
    - none
- setter 
    - none
## Object 1019 Synchronous counter overflow value | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Synchronous counter overflow value | uint8 | 0
- getter 
    - node.sync().getData
- setter 
    - node.sync().setData
## Object 1400 Receive PDO 1 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 2
| COB-ID | uint32 | 2147484161
| Transmission type | uint8 | 0
- getter 
    - node.pdo().rpdos[0].commParameter.getData
- setter 
    - node.pdo().rpdos[0].commParameter.setData
## Object 1401 Receive PDO 2 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 2
| COB-ID | uint32 | 2147484417
| Transmission type | uint8 | 0
- getter 
    - node.pdo().rpdos[1].commParameter.getData
- setter 
    - node.pdo().rpdos[1].commParameter.setData
## Object 1402 Receive PDO 3 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 2
| COB-ID | uint32 | 2147484673
| Transmission type | uint8 | 0
- getter 
    - node.pdo().rpdos[2].commParameter.getData
- setter 
    - node.pdo().rpdos[2].commParameter.setData
## Object 1403 Receive PDO 4 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 2
| COB-ID | uint32 | 2147484929
| Transmission type | uint8 | 0
- getter 
    - node.pdo().rpdos[3].commParameter.getData
- setter 
    - node.pdo().rpdos[3].commParameter.setData
## Object 1600 Receive PDO 1 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().rpdos[0].mapParameter.getData
- setter 
    - node.pdo().rpdos[0].mapParameter.setData
## Object 1601 Receive PDO 2 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().rpdos[1].mapParameter.getData
- setter 
    - node.pdo().rpdos[1].mapParameter.setData
## Object 1602 Receive PDO 3 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().rpdos[2].mapParameter.getData
- setter 
    - node.pdo().rpdos[2].mapParameter.setData
## Object 1603 Receive PDO 4 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().rpdos[3].mapParameter.getData
- setter 
    - node.pdo().rpdos[3].mapParameter.setData
## Object 1800 Transmit PDO 1 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 6
| COB-ID | uint32 | 2147484033
| Transmission type | uint8 | 0
| Inhibit time | uint16 | 0
| Compatibility entry | uint8 | 0
| Event timer | uint16 | 0
| Sync start value | uint8 | 0
- getter 
    - node.pdo().tpdos[0].commParameter.getData
- setter 
    - node.pdo().tpdos[0].commParameter.setData
## Object 1801 Transmit PDO 2 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 6
| COB-ID | uint32 | 2147484289
| Transmission type | uint8 | 0
| Inhibit time | uint16 | 0
| Compatibility entry | uint8 | 0
| Event timer | uint16 | 0
| Sync start value | uint8 | 0
- getter 
    - node.pdo().tpdos[1].commParameter.getData
- setter 
    - node.pdo().tpdos[1].commParameter.setData
## Object 1802 Transmit PDO 3 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 6
| COB-ID | uint32 | 2147484545
| Transmission type | uint8 | 0
| Inhibit time | uint16 | 0
| Compatibility entry | uint8 | 0
| Event timer | uint16 | 0
| Sync start value | uint8 | 0
- getter 
    - node.pdo().tpdos[2].commParameter.getData
- setter 
    - node.pdo().tpdos[2].commParameter.setData
## Object 1803 Transmit PDO 4 communication parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 6
| COB-ID | uint32 | 2147484801
| Transmission type | uint8 | 0
| Inhibit time | uint16 | 0
| Compatibility entry | uint8 | 0
| Event timer | uint16 | 0
| Sync start value | uint8 | 0
- getter 
    - node.pdo().tpdos[3].commParameter.getData
- setter 
    - node.pdo().tpdos[3].commParameter.setData
## Object 1a00 Transmit PDO 1 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().tpdos[0].mapParameter.getData
- setter 
    - node.pdo().tpdos[0].mapParameter.setData
## Object 1a01 Transmit PDO 2 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().tpdos[1].mapParameter.getData
- setter 
    - node.pdo().tpdos[1].mapParameter.setData
## Object 1a02 Transmit PDO 3 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().tpdos[2].mapParameter.getData
- setter 
    - node.pdo().tpdos[2].mapParameter.setData
## Object 1a03 Transmit PDO 4 mapping parameter | 

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| Number of of entries | uint8 | 0
| 1st object to be mapped | uint32 | 0
| 2nd object to be mapped | uint32 | 0
| 3rd object to be mapped | uint32 | 0
| 4th object to be mapped | uint32 | 0
| 5th object to be mapped | uint32 | 0
| 6th object to be mapped | uint32 | 0
| 7th object to be mapped | uint32 | 0
| 8th object to be mapped | uint32 | 0
- getter 
    - node.pdo().tpdos[3].mapParameter.getData
- setter 
    - node.pdo().tpdos[3].mapParameter.setData
## Object 6040 Controlword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Controlword | uint16 | 0
- getter 
    - sys.axis[0].getControlword
- setter 
    - sys.axis[0].setControlword
## Object 6041 Statusword | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Statusword | uint16 | 0
- getter 
    - sys.axis[0].getStatusword
- setter 
    - none
## Object 6060 Modes of operation | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Modes of operation | int8 | 0
- getter 
    - sys.axis[0].getOperationMode
- setter 
    - sys.axis[0].setOperationMode
## Object 6061 Modes of operation display | state-machine

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Modes of operation display | int8 | 0
- getter 
    - sys.axis[0].getOperationModeDisplay
- setter 
    - none
## Object 6062 Position demand value | position-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Position demand value | int32 | 0
- getter 
    - sys.axis[0].getDemandPosition
- setter 
    - none
## Object 6064 Position actual value | position-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Position actual value | int32 | 0
- getter 
    - sys.axis[0].getActualPosition
- setter 
    - none
## Object 606b Velocity demand value | velocity-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Velocity demand value | int32 | 0
- getter 
    - sys.axis[0].getDemandVelocity
- setter 
    - none
## Object 606c Velocity actual value | velocity-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Velocity actual value | int32 | 0
- getter 
    - sys.axis[0].getActualVelocity
- setter 
    - none
## Object 6071 Target torque | torque-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Target torque | int16 | 0
- getter 
    - sys.axis[0].getTargetTorque
- setter 
    - sys.axis[0].setTargetTorque
## Object 6075 motor rated current | motor

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| motor rated current | uint32 | 0
- getter 
    - sys.config.getMotorRatedCurrent
- setter 
    - sys.config.setMotorRatedCurrent
## Object 6076 motor rated torque | motor

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| motor rated torque | uint32 | 0
- getter 
    - sys.config.getMotorRatedTorque
- setter 
    - sys.config.setMotorRatedTorque
## Object 6077 Torque actual value | torque-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Torque actual value | int16 | 0
- getter 
    - sys.axis[0].getActualTorque
- setter 
    - none
## Object 6078 Current actual value | torque-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Current actual value | int16 | 0
- getter 
    - sys.axis[0].getActualCurrent
- setter 
    - none
## Object 607a Target position | position-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Target position | int32 | 0
- getter 
    - sys.axis[0].getTargetPosition
- setter 
    - sys.axis[0].setTargetPosition
## Object 6080 Max motor speed | motor

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Max motor speed | uint32 | 0
- getter 
    - sys.config.getMaxMotorSpeed
- setter 
    - sys.config.setMaxMotorSpeed
## Object 6081 Profile velocity | velocity-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Profile velocity | uint32 | 0
- getter 
    - sys.axis[0].getProfileVelocity
- setter 
    - sys.axis[0].setProfileVelocity
## Object 6083 Profile acceleration | velocity-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Profile acceleration | uint32 | 0
- getter 
    - sys.axis[0].getProfileAcceleration
- setter 
    - sys.axis[0].setProfileAcceleration
## Object 6084 Profile deceleration | velocity-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Profile deceleration | uint32 | 0
- getter 
    - sys.axis[0].getProfileDeceleration
- setter 
    - sys.axis[0].setProfileDeceleration
## Object 608f Position encoder resolution | position-loop

[x] Remote
| Name | Type | default |
| --- | --- | --- |
| Highes sub-index supported | uint8 | 2
| Encoder increments | uint32 | 0
| motor revolutions | uint32 | 0
- getter 
    - none
- setter 
    - none
## Object 60a8 SI unit Position | position-loop

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| SI unit Position | uint32 | 4244701184
- getter 
    - none
- setter 
    - none
## Object 60a9 SI unit Velocity | velocity-loop

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| SI unit Velocity | uint32 | 4244701952
- getter 
    - none
- setter 
    - none
## Object 60aa SI unit Acceleration | velocity-loop

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| SI unit Acceleration | uint32 | 4244723456
- getter 
    - none
- setter 
    - none
## Object 60ab SI unit Jerk | torque-loop

[ ] Remote
| Name | Type | default |
| --- | --- | --- |
| SI unit Jerk | uint32 | 4244742144
- getter 
    - none
- setter 
    - none
