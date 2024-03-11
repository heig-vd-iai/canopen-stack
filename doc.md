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
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Device type | uint32 | 0 | none | none |
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
| Standard error field | uint32 | 0 | none | none |
| Standard error field | uint32 | 0 | none | none |
## Object 1010 Store parameters | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of entries | uint8 | 4 | node.od().getSave | node.od().saveData |
| save all parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save communication parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save application parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
| save manufacturer specific parameters | uint32 | 0 | node.od().getSave | node.od().saveData |
## Object 1011 Restore default parameters | 

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Number of entries | uint8 | 4 | node.od().getRestore | node.od().restoreData |
| restore all parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore communication parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore application parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
| restore manufacturer specific parameters | uint32 | 0 | node.od().getRestore | node.od().restoreData |
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
## Object 1a00 Transmit PDO 1 mapping parameter | 

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
## Object 1a01 Transmit PDO 2 mapping parameter | 

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
## Object 1a02 Transmit PDO 3 mapping parameter | 

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
## Object 1a03 Transmit PDO 4 mapping parameter | 

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
## Object 606b Velocity demand value | velocity-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Velocity demand value | int32 | 0 | sys.axis[0].getDemandVelocity | none |
## Object 606c Velocity actual value | velocity-loop

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
## Object 607a Target position | position-loop

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
## Object 608f Position encoder resolution | position-loop

[x] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| Highes sub-index supported | uint8 | 2 | none | none |
| Encoder increments | uint32 | 0 | sys.config.getPositionEncoderIncrements | sys.config.setPositionEncoderIncrements |
| motor revolutions | uint32 | 0 | sys.config.getPositionEncoderMotorRevolutions | sys.config.setPositionEncoderMotorRevolutions |
## Object 60a8 SI unit Position | position-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Position | uint32 | 4244701184 | none | none |
## Object 60a9 SI unit Velocity | velocity-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Velocity | uint32 | 4244701952 | none | none |
## Object 60aa SI unit Acceleration | velocity-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Acceleration | uint32 | 4244723456 | none | none |
## Object 60ab SI unit Jerk | torque-loop

[ ] Remote
| Name | Type | default | get | set |
| --- | --- | --- | --- | --- |
| SI unit Jerk | uint32 | 4244742144 | none | none |
