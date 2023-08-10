#pragma once
#include <cstdint>
// TODO: add defines for numbers

namespace CANopen
{
    struct Frame
    {
        uint8_t nodeId;
        uint8_t functionCode;
        uint8_t data[8] = {0};
        uint8_t dlc = 0;
        bool rtr = false;
        // Frame(uint8_t nodeId, uint8_t functionCode) : nodeId(nodeId), functionCode(functionCode) {}
        uint16_t getCobID() { return ((uint16_t)functionCode & 0xF) << 7 | (nodeId & 0x7F); }
        void setCobID(uint16_t cobId)
        {
            nodeId = cobId & 0x7F;
            functionCode = (cobId >> 7) & 0xF;
        }
    };
}
