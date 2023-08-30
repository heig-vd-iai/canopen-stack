/******************************************************************************
 * [Filename]:      enums.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains all of the shared enumerations used in this project.
 *****************************************************************************/
#pragma once

namespace CANopen
{
    /**
     * @brief Enumeration representing the CANopen function codes.
     * See p. 81 of CIA301 for more details.
     */
    enum FunctionCodes
    {
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
     * @brief Enumeration representing the states of the %NMT state machine.
     * See p. 76 of CIA301 for more details.
     */
    enum NMTStates
    {
        NMTState_Initialisation = 0x00,
        NMTState_PreOperational = 0x7F,
        NMTState_Operational = 0x05,
        NMTState_Stopped = 0x04
    };

    /**
     * @brief Enumeration representing the %NMT initialisation state substeps.
     * See p. 79 of CIA301 for more details.
     */
    enum NMTResetStates
    {
        NMTResetState_Initialising,
        NMTResetState_ResetApplication,
        NMTResetState_ResetCommunication
    };

    /**
     * @brief Enumeration representing the %NMT command specifiers sent by the master.
     * See p. 72 of CIA301 for more details.
     */
    enum NMTServiceCommands
    {
        NMTServiceCommand_None = 0x00,
        NMTServiceCommand_Start = 0x01,
        NMTServiceCommand_Stop = 0x02,
        NMTServiceCommand_EnterPreOperational = 0x80,
        NMTServiceCommand_ResetNode = 0x81,
        NMTServiceCommand_ResetCommunication = 0x82
    };

    /**
     * @brief Enumeration representing the command specifier part in the %SDO command byte.
     * See p. 49 of CIA301 for more details.
     */
    enum SDOCommandSpecifiers
    {
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
     * @brief Enumeration representing the subcommand part in the %SDO block command byte.
     * See p. 54 of CIA301 for more details.
     */
    enum SDOSubCommands
    {
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
     * @brief Enumeration representing all of the available %SDO abort codes available.
     * Some special abort codes were added, but are only used internally and should never be sent.
     * See p. 61 of CIA301 for more details.
     */
    enum SDOAbortCodes
    {
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
}

/**
 * @brief Enumeration representing all of the available %EMCY error codes.
 * See p. 64 of CIA301 for more details.
 */
enum EMCYErrorCodes
{
    EMCYErrorCode_Reset = 0x0000,
    EMCYErrorCode_Generic = 0x1000,
    EMCYErrorCode_Current = 0x2000,
    EMCYErrorCode_Current_InputSide = 0x2100,
    EMCYErrorCode_Current_InsideDevice = 0x2200,
    EMCYErrorCode_Current_OutputSide = 0x2300,
    EMCYErrorCode_Voltage = 0x3000,
    EMCYErrorCode_Voltage_Main = 0x3100,
    EMCYErrorCode_Voltage_InsideDevice = 0x3200,
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
 * @brief Enumeration representing the bit number of each bit in the error register.
 * See p. 92 of CIA301 for more details.
 */
enum ErrorRegisterBits
{
    ErrorRegisterBit_Generic = 0,
    ErrorRegisterBit_Current = 1,
    ErrorRegisterBit_Voltage = 2,
    ErrorRegisterBit_Temperature = 3,
    ErrorRegisterBit_Communication = 4,
    ErrorRegisterBit_DeviceProfile = 5,
    ErrorRegisterBit_Manufacturer = 7
};

/**
 * @brief Enumeration representing the parameter groups for saving and restoring the object dictionnary.
 * This enum only accounts for 3 first default groups.
 * Up to 255 groups can be implemented, depending on the device profile or manufacturer.
 * See p. 101 of CIA301 for more details.
 */
enum ParameterGroups
{
    ParameterGroup_All = 1,
    ParameterGroup_Communication = 2,
    ParameterGroup_Application = 3
};
