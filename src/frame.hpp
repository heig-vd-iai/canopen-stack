#pragma once
#include <cstdint>
// TODO: add defines for numbers

namespace CANopen
{
    struct Frame
    {
        uint8_t nodeId;
        uint8_t functionCode;
        uint8_t data[8] = {0};  //TODO define 8
        uint8_t dlc = 0;
        bool rtr = false;
        // Frame(uint8_t nodeId, uint8_t functionCode) : nodeId(nodeId), functionCode(functionCode) {}
        uint16_t getCobID();
        void setCobID(uint16_t cobId);
    };
}
