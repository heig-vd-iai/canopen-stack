#pragma once
#include <cstdint>
#define FRAME_DATA_LENGTH 8

struct CANopen_Frame
{
    uint8_t data[8] = {0};
    uint8_t dlc = 0;
    uint8_t functionCode = 0;
    uint8_t nodeId = 0;
    bool rtr = false;
};
