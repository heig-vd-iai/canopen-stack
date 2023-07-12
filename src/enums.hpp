#pragma once

enum FunctionCodes
{
    FunctionCode_NMT = 0b0000,
    FunctionCode_SYNC = 0b0001,
    FunctionCode_EMCY = 0b0001,
    FunctionCode_TIME = 0b0010,
    FunctionCode_HEARTBEAT = 0b1110
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
