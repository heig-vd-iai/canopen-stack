#pragma once

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

enum NMTStates
{
    NMTState_Initialisation = 0x00,
    NMTState_PreOperational = 0x7F,
    NMTState_Operational = 0x05,
    NMTState_Stopped = 0x04
};

enum NMTServiceCommands
{
    NMTServiceCommand_Start = 0x01,
    NMTServiceCommand_Stop = 0x02,
    NMTServiceCommand_EnterPreOperational = 0x80,
    NMTServiceCommand_ResetNode = 0x81,
    NMTServiceCommand_ResetCommunication = 0x82
};

enum SDOCommandSpecifiers
{
    SDOCommandSpecifier_RequestDownloadInitiate = 0x01,
    SDOCommandSpecifier_ResponseDownloadInitiate = 0x03,
    SDOCommandSpecifier_RequestDownloadSegment = 0x00,
    SDOCommandSpecifier_ResponseDownloadSegment = 0x01,
    SDOCommandSpecifier_RequestUploadInitiate = 0x02,
    SDOCommandSpecifier_ResponseUploadInitiate = 0x02,
    SDOCommandSpecifier_RequestUploadSegment = 0x03,
    SDOCommandSpecifier_ResponseUploadSegment = 0x00,
    SDOCommandSpecifier_AbortTransfer = 0x04
    // TODO: block transfer specifiers
};

enum DataTypes
{
    DataType_BOOLEAN = 0x01,
    DataType_INTEGER8 = 0x02,
    DataType_INTEGER16 = 0x03,
    DataType_INTEGER32 = 0x04,
    DataType_UNSIGNED8 = 0x05,
    DataType_UNSIGNED16 = 0x06,
    DataType_UNSIGNED32 = 0x07,
    DataType_REAL32 = 0x08,
    // DataType_VISIBLE_STRING = 0x09,
    // DataType_OCTET_STRING = 0x0A,
    // DataType_UNICODE_STRING = 0x0B,
    // DataType_TIME_OF_DAY = 0x0C,
    // DataType_TIME_DIFFERENCE = 0x0D,
    // DataType_DOMAIN = 0x0F,
    DataType_INTEGER24 = 0x10,
    DataType_REAL64 = 0x11,
    DataType_INTEGER40 = 0x12,
    DataType_INTEGER48 = 0x13,
    DataType_INTEGER56 = 0x14,
    DataType_INTEGER64 = 0x15,
    DataType_UNSIGNED24 = 0x16,
    DataType_UNSIGNED40 = 0x18,
    DataType_UNSIGNED48 = 0x19,
    DataType_UNSIGNED56 = 0x1A,
    DataType_UNSIGNED64 = 0x1B
};

enum SDOAbortCodes
{
    SDOAbortCode_OK = 0,
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
    SDOAbortCode_GeneralError = 0x08000000
    // TODO: add missing (unused?) error codes
};
