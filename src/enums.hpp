/**
 * Contains all of the shared enumerations used in this project.
 */
#pragma once

namespace CANopen {
/**
 * Enumeration representing the CANopen function codes.
 * See CiA301:2011§7.3.3 (p. 81)
 */
enum FunctionCodes {
    FunctionCode_NMT = 0b0000,
    FunctionCode_SYNC = 0b0001,
    FunctionCode_EMCY = 0b0001,
    FunctionCode_TIME = 0b0010,
    FunctionCode_TPDO1 = 0b0011,
    FunctionCode_RPDO1 = 0b0100,
    FunctionCode_TPDO2 = 0b0101,
    FunctionCode_RPDO2 = 0b0110,
    FunctionCode_TPDO3 = 0b0111,
    FunctionCode_RPDO3 = 0b1000,
    FunctionCode_TPDO4 = 0b1001,
    FunctionCode_RPDO4 = 0b1010,
    FunctionCode_TSDO = 0b1011,
    FunctionCode_RSDO = 0b1100,
    FunctionCode_HEARTBEAT = 0b1110,
};

/**
 * Enumeration representing the states of the NMT state machine.
 * See CiA301:2011§7.2.8.3.2.2 (p. 76)
 */
enum NMTStates {
    NMTState_Initialisation = 0x00,
    NMTState_PreOperational = 0x7F,
    NMTState_Operational = 0x05,
    NMTState_Stopped = 0x04
};

/**
 * Enumeration representing the NMT initialisation state substeps.
 * See CiA301:2011§7.3.2.2.1 (p. 79)
 */
enum NMTResetStates {
    NMTResetState_Initialising,
    NMTResetState_ResetApplication,
    NMTResetState_ResetCommunication
};

/**
 * Enumeration representing the NMT command specifiers sent by the master.
 * See CiA301:2011§7.2.8.3.1 (p. 72)
 */
enum NMTServiceCommands {
    NMTServiceCommand_None = 0x00,
    NMTServiceCommand_Start = 0x01,
    NMTServiceCommand_Stop = 0x02,
    NMTServiceCommand_EnterPreOperational = 0x80,
    NMTServiceCommand_ResetNode = 0x81,
    NMTServiceCommand_ResetCommunication = 0x82
};

/**
 * Enumeration representing the command specifier part in the SDO command byte.
 * See CiA301:2011§7.2.4.3 (p. 49)
 */
enum SDOCommandSpecifiers {
    SDOCommandSpecifier_ClientDownloadInitiate = 0x01,
    SDOCommandSpecifier_ServerDownloadInitiate = 0x03,
    SDOCommandSpecifier_ClientDownloadSegment = 0x00,
    SDOCommandSpecifier_ServerDownloadSegment = 0x01,
    SDOCommandSpecifier_ClientUploadInitiate = 0x02,
    SDOCommandSpecifier_ServerUploadInitiate = 0x02,
    SDOCommandSpecifier_ClientUploadSegment = 0x03,
    SDOCommandSpecifier_ServerUploadSegment = 0x00,
    SDOCommandSpecifier_AbortTransfer = 0x04,
    SDOCommandSpecifier_ClientBlockDownload = 0x06,
    SDOCommandSpecifier_ServerBlockDownload = 0x05,
    SDOCommandSpecifier_ClientBlockUpload = 0x05,
    SDOCommandSpecifier_ServerBlockUpload = 0x06
};

/**
 * Enumeration representing the subcommand part in the SDO block command byte.
 * See CiA301:2011§7.2.4.3 (p. 54)
 */
enum SDOSubCommands {
    SDOSubCommand_ClientDownloadInitiate = 0x00,
    SDOSubCommand_ServerDownloadInitiate = 0x00,
    SDOSubCommand_ServerDownloadResponse = 0x02,
    SDOSubCommand_ClientDownloadEnd = 0x01,
    SDOSubCommand_ServerDownloadEnd = 0x01,
    SDOSubCommand_ClientUploadInitiate = 0x00,
    SDOSubCommand_ServerUploadInitiate = 0x00,
    SDOSubCommand_ClientUploadStart = 0x03,
    SDOSubCommand_ClientUploadResponse = 0x02,
    SDOSubCommand_ClientUploadEnd = 0x01,
    SDOSubCommand_ServerUploadEnd = 0x01
};

/**
 * Enumeration representing all of the available SDO abort codes available.
 * Some special abort codes were added, but are only used internally and should
 * never be sent. See CiA301:2011§7.2.4.3.17 (p. 61)
 */
enum SDOAbortCodes {
    SDOAbortCode_OK = 0,
    SDOAbortCode_CancelWrite = 1,
    SDOAbortCode_ToggleBitNotAlternated = 0x05030000,
    SDOAbortCode_TimedOut = 0x05040000,
    SDOAbortCode_CommandSpecifierInvalid = 0x05040001,
    SDOAbortCode_InvalidBlockSize = 0x05040002,
    SDOAbortCode_InvalidSequenceNumber = 0x05040003,
    SDOAbortCode_CRCError = 0x05040004,
    SDOAbortCode_OutOfMemory = 0x05040005,
    SDOAbortCode_UnsupportedObjectAccess = 0x06010000,
    SDOAbortCode_AttemptReadOnWriteOnly = 0x06010001,
    SDOAbortCode_AttemptWriteOnReadOnly = 0x06010002,
    SDOAbortCode_ObjectNonExistent = 0x06020000,
    SDOAbortCode_CannotMapToPDO = 0x06040041,
    SDOAbortCode_MappedPDOLengthExceeded = 0x06040042,
    SDOAbortCode_GeneralParameterIncompatibility = 0x06040043,
    SDOAbortCode_GeneralInternalIncompatibility = 0x06040047,
    SDOAbortCode_AccessFailedHardwareError = 0x06060000,
    SDOAbortCode_DataTypeMismatch_LengthParameterMismatch = 0x06070010,
    SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh = 0x06070012,
    SDOAbortCode_DataTypeMismatch_LengthParameterTooLow = 0x06070013,
    SDOAbortCode_SubindexNonExistent = 0x06090011,
    SDOAbortCode_InvalidDownloadParameterValue = 0x06090030,
    SDOAbortCode_DownloadValueTooHigh = 0x06090031,
    SDOAbortCode_DownloadValueTooLow = 0x06090032,
    SDOAbortCode_MaxValueLessThanMinimumValue = 0x06090036,
    SDOAbortCode_ResourceNotAvailable = 0x060A0023,
    SDOAbortCode_GeneralError = 0x08000000,
    SDOAbortCode_CannotStoreOrTransfer = 0x08000020,
    SDOAbortCode_CannotStoreOrTransfer_LocalControl = 0x08000021,
    SDOAbortCode_CannotStoreOrTransfer_DeviceState = 0x08000022,
    SDOAbortCode_NoDataAvailable = 0x08000024
};

/*
* Enumeration representing the physical units of the data types.
* See CiA890
*/
enum PhysUnit {
    PhysUnit_NONE = 0x00,  // dimensionless
    PhysUnit_METER = 0x01,
    PhysUnit_KILOGRAM = 0x02,
    PhysUnit_SECOND = 0x03,
    PhysUnit_AMPERE = 0x04,
    PhysUnit_KELVIN = 0x05,
    PhysUnit_MOLE = 0x06,
    PhysUnit_CANDELA = 0x07,
    PhysUnit_RADIAN = 0x10,
    PhysUnit_STERADIAN = 0x11,
    PhysUnit_HERTZ = 0x20,
    PhysUnit_NEWTON = 0x21,
    PhysUnit_PASCAL = 0x22,
    PhysUnit_JOULE = 0x23,
    PhysUnit_WATT = 0x24,
    PhysUnit_COULOMB = 0x25,
    PhysUnit_VOLT = 0x26,
    PhysUnit_FARAD = 0x27,
    PhysUnit_OHM = 0x28,
    PhysUnit_SIEMENS = 0x29,
    PhysUnit_WEBER = 0x2A,
    PhysUnit_TESLA = 0x2B,
    PhysUnit_HENRY = 0x2C,
    PhysUnit_DEGREE_CELSIUS = 0x2D,
    PhysUnit_LUMEN = 0x2E,
    PhysUnit_LUX = 0x2F,
    PhysUnit_BECQUEREL = 0x30,
    PhysUnit_GRAY = 0x31,
    PhysUnit_SIEVERT = 0x32,
    PhysUnit_KATAL = 0x33,
    PhysUnit_GRADE = 0x40,  // angle
    PhysUnit_DEGREE = 0x41,
    PhysUnit_MINUTE_ANGLE = 0x42,
    PhysUnit_SECOND_ANGLE = 0x43,
    PhysUnit_LITRE = 0x44,
    PhysUnit_ARE = 0x45,
    PhysUnit_HECTARE = 0x46,
    PhysUnit_MINUTE = 0x47,
    PhysUnit_HOUR = 0x48,
    PhysUnit_DAY = 0x49,
    PhysUnit_YEAR = 0x4A,
    PhysUnit_GRAM = 0x4B,
    PhysUnit_TONNE = 0x4C,
    PhysUnit_BAR = 0x4E,
    PhysUnit_POISE = 0x4F,
    PhysUnit_STOKES = 0x50,
    PhsyUnit_ELECTRONVOLT = 0x51,
    PhysUnit_ATOMIC_MASS_UNIT = 0x52,
    PhysUnit_ASTRONOMICAL_UNIT = 0x53,
    PhysUnit_PARSEC = 0x54,
    PhysUnit_METER_SQUARE_SECOND = 0x55,
    PhysUnit_NEWTON_METER = 0x56,
    // Device profile specific units see CiA402 §9.2.9
    PhysUnit_CUBIC_SECOND = 0xA0,
    PhysUnit_REVOLUTION = 0xB4,
    PhysUnit_INC = 0xB5,
    PhysUnit_STEP = 0xB6
};
}  // namespace CANopen

/**
 * Enumeration representing all of the available EMCY error codes.
 * See CiA301:2011§7.2.7.1 (p. 64)
 */
enum EMCYErrorCodes {
    EMCYErrorCode_Reset = 0x0000,
    EMCYErrorCode_Generic = 0x1000,
    EMCYErrorCode_Current = 0x2000,
    EMCYErrorCode_Current_InputSide = 0x2100,
    EMCYErrorCode_Current_InsideDevice = 0x2200,
    EMCYErrorCode_Current_OutputSide = 0x2300,
    EMCYErrorCode_Voltage = 0x3000,
    EMCYErrorCode_Voltage_Main = 0x3100,
    EMCYErrorCode_Voltage_InsideDevice = 0x3200,
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

/**
 * Enumeration representing the bit number of each bit in the error register.
 * See CiA301:2011§7.5.2.2 (p. 92)
 */
enum ErrorRegisterBits {
    ErrorRegisterBit_Generic = 0,
    ErrorRegisterBit_Current = 1,
    ErrorRegisterBit_Voltage = 2,
    ErrorRegisterBit_Temperature = 3,
    ErrorRegisterBit_Communication = 4,
    ErrorRegisterBit_DeviceProfile = 5,
    ErrorRegisterBit_Manufacturer = 7
};

enum ErrorBehaviorValue {
    ErrorBehaviorValue_PreOperational = 0x00,
    ErrorBehaviorValue_None = 0x01,
    ErrorBehaviorValue_Stop = 0x02,
};

/**
 * Enumeration representing the parameter groups for saving and restoring the
 * object dictionnary. This enum only accounts for 3 first default groups. Up to
 * 255 groups can be implemented, depending on the device profile or
 * manufacturer. See CiA301:2011§7.5.2.13 (p. 101)
 */
enum ParameterGroups { //TODO: move to od.hpp and metagenerate manufacturer specific groups
    ParameterGroup_All = 1,
    ParameterGroup_Communication = 2,
    ParameterGroup_Application = 3,
    ParameterGroup_ManufacturerA = 4
};

enum AccessType {
    Metadata_ReadOnlyNotMappable = 0x01,
    Metadata_WriteOnlyNotMappable = 0x02,
    Metadata_ReadWriteNotMappable = 0x03,
    Metadata_ReadOnlyMappable = 0x05,
    Metadata_WriteOnlyMappable = 0x06,
    Metadata_ReadWriteMappable = 0x07
};

enum DataType {
    BOOL = 0x01,
    INTEGER8 = 0x02,
    INTEGER16 = 0x03,
    INTEGER32 = 0x04,
    INTEGER64 = 0x15,
    UNSIGNED8 = 0x05,
    UNSIGNED16 = 0x06,
    UNSIGNED32 = 0x07,
    UNSIGNED64 = 0x1B,
    REAL32 = 0x08,
    REAL64 = 0x11,
    VISIBLE_STRING = 0x09,
    ARRAY
};
