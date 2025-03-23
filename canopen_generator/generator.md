# CANOpen-Generator

This python programe generate the eds file, the od.hpp file and the od.cpp file for the canopen-stack.

## Profile.yaml

This file contain all definitions of standard object supported by the canopen-stack and application.

```yaml
# profile.yaml
functionalities:
  baudrate:
    10: true
    20: true
    50: true
    125: true
    250: true
    500: true
    800: true
    1000: true
  simpleBootUpMaster: false
  simpleBootUpSlave: true
  granularity: 8
  dynamicChannelsSupported: false
  compactPDO: false
  groupMessaging: false
  LSS_Supported: false

profiles:
  301:
    name: Application layer and communication profile
    objects:
      0x1000:
        name: Device type
        category: mandatory #optional default "optional"
        get: node.getDeviceType() #optional
        data:
        - type : uint32
          name: device type #optional if only one data
          access: ro
          pdoMapping: false #optional "default false"
          defaultValue: 0x12345678 #optional default 0
    402:
    name: Profile for drives and motion control
    objects:
      0x6040:
        name: Controlword
        category: mandatory
        data:
        - type: uint16
          access: rw
          pdo_mapping: true
          default: 0
