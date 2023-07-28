#pragma once
#include <cstdint>
#define FRAME_DATA_LENGTH 8

namespace CANopen
{
    struct Frame
    {
        uint8_t data[8] = {0};
        uint8_t dlc = 0;
        union
        {
            uint16_t value;
            struct
            {
                uint16_t nodeId : 7;
                uint16_t functionCode : 4;
            } bits;
        } cobId = {0};
        bool rtr = false;
    };
}
