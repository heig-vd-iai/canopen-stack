#pragma once
#include <cstdint>

struct CANopen_Frame
{
    uint8_t data[8] = {0};
    uint8_t dlc = 0;
    uint8_t functionCode = 0;
    uint8_t nodeId = 0;
    uint8_t rtr = 0;
};
