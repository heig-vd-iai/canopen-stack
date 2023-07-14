#pragma once
#include <cstdint>

struct CANopen_Frame
{
    uint8_t data[8];
    uint8_t dlc;
    uint8_t functionCode;
    uint8_t nodeId;
    uint8_t rtr;
    CANopen_Frame() : data{0}, dlc(0), functionCode(0), nodeId(0), rtr(0) {}
};
