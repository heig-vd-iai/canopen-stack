#pragma once

namespace CANopen {

/** Emergency error codes. CiA301:2011§7.2.7.1 table 26, CiA402 */
enum EMCYErrorCodes {
    EMCYErrorCode_Reset = 0x0000,
    EMCYErrorCode_Generic = 0x1000,
    EMCYErrorCode_Current = 0x2000,
    EMCYErrorCode_Current_InputSide = 0x2100,
    EMCYErrorCode_Current_InsideDevice = 0x2200,
    EMCYErrorCode_Continuous_Over_Current_Device_Internal = 0x2220,
    EMCYErrorCode_Current_OutputSide = 0x2300,
    EMCYErrorCode_Voltage = 0x3000,
    EMCYErrorCode_Voltage_Main = 0x3100,
    EMCYErrorCode_Voltage_InsideDevice = 0x3200,
    EMCYErrorCode_DC_Link_Over_Voltage = 0x3210,
    EMCYErrorCode_DC_Link_Under_Voltage = 0x3220,
    EMCYErrorCode_Voltage_Output = 0x3300,
    EMCYErrorCode_Temperature = 0x4000,
    EMCYErrorCode_Temperature_Ambient = 0x4100,
    EMCYErrorCode_Temperature_Device = 0x4200,
    EMCYErrorCode_DeviceHardware = 0x5000,
    EMCYErrorCode_Software = 0x6000,
    EMCYErrorCode_Software_Internal = 0x6100,
    EMCYErrorCode_Software_User = 0x6200,
    EMCYErrorCode_Software_Dataset = 0x6300,
    EMCYErrorCode_Modules = 0x7000,
    EMCYErrorCode_Monitoring = 0x8000,
    EMCYErrorCode_Communication = 0x8100,
    EMCYErrorCode_Communication_CANOverrun = 0x8110,
    EMCYErrorCode_Communication_CANErrorPassive = 0x8120,
    EMCYErrorCode_Communication_HeartbeatError = 0x8130,
    EMCYErrorCode_Communication_RecoveredBusOFF = 0x8140,
    EMCYErrorCode_Communication_CANIDCollision = 0x8150,
    EMCYErrorCode_Protocol = 0x8200,
    EMCYErrorCode_Protocol_PDOLengthError = 0x8210,
    EMCYErrorCode_Protocol_PDOLengthExceeded = 0x8220,
    EMCYErrorCode_Protocol_DAMMPDODestinationNotAvailable = 0x8230,
    EMCYErrorCode_Protocol_SyncDataLength = 0x8240,
    EMCYErrorCode_Protocol_RPDOTimeout = 0x8250,
    EMCYErrorCode_ExternalError = 0x9000,
    EMCYErrorCode_AdditionalFunctions = 0xF000,
    EMCYErrorCode_DeviceSpecific = 0xFF00
};

/** Bits of the error register, object 0x1001. CiA301:2011§7.5.2.3 */
enum ErrorRegisterBits {
    ErrorRegisterBit_Generic = 0,
    ErrorRegisterBit_Current = 1,
    ErrorRegisterBit_Voltage = 2,
    ErrorRegisterBit_Temperature = 3,
    ErrorRegisterBit_Communication = 4,
    ErrorRegisterBit_DeviceProfile = 5,
    ErrorRegisterBit_Manufacturer = 7
};

/** Values of the error behavior object 0x1029. CiA301:2011§7.5.2.27 */
enum ErrorBehaviorValue {
    ErrorBehaviorValue_PreOperational = 0x00,
    ErrorBehaviorValue_None = 0x01,
    ErrorBehaviorValue_Stop = 0x02,
};
}  // namespace CANopen
