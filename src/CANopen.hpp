#pragma once
#include "nmt.hpp"
#include "od.hpp"

enum FunctionCodes
{
    FunctionCode_NMT = 0b0000,
    FunctionCode_SYNC = 0b0001,
    FunctionCode_EMCY = 0b0001,
    FunctionCode_TIME = 0b0010,
    FunctionCode_HEARTBEAT = 0b1110
};
