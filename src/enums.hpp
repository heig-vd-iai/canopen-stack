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
    NMTServiceCommands_Start = 0x01,
    NMTServiceCommands_Stop = 0x02,
    NMTServiceCommands_EnterPreOperational = 0x80,
    NMTServiceCommands_ResetNode = 0x81,
    NMTServiceCommands_ResetCommunication = 0x82
};

enum SDOCommandSpecifiers
{
    SDOCommandSpecifiers_RequestDownloadInitiate = 0x01,
    SDOCommandSpecifiers_ResponseDownloadInitiate = 0x03,
    SDOCommandSpecifiers_RequestDownloadSegment = 0x00,
    SDOCommandSpecifiers_ResponseDownloadSegment = 0x01,
    SDOCommandSpecifiers_RequestUploadInitiate = 0x02,
    SDOCommandSpecifiers_ResponseUploadInitiate = 0x02,
    SDOCommandSpecifiers_RequestUploadSegment = 0x03,
    SDOCommandSpecifiers_ResponseUploadSegment = 0x00,
    SDOCommandSpecifiers_AbortTransfer = 0x04
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
