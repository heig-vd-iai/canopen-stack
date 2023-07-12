#pragma once
#include <cstdint>

struct CANopen_Frame
{
    uint8_t data[8];
    uint8_t dlc;
    uint8_t functionCode;
    uint8_t nodeId;
    uint8_t rtr;
};
