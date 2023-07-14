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

enum SDOClientCommandSpecifiers
{
    SDOClientCommandSpecifier_SegmentDownload = 0x00,
    SDOClientCommandSpecifier_InitiatingDownload = 0x01,
    SDOClientCommandSpecifier_InitiatingUpload = 0x02,
    SDOClientCommandSpecifier_SegmentUpload = 0x03,
    SDOClientCommandSpecifier_AbortTransfer = 0x04,
    SDOClientCommandSpecifier_BlockUpload = 0x05,
    SDOClientCommandSpecifier_BlockDownload = 0x06,
};
