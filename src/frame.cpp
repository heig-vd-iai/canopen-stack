#include "frame.hpp"
using namespace CANopen;

uint16_t CANopen::Frame::getCobID()
{
    return ((uint16_t)functionCode & 0xF) << 7 | (nodeId & 0x7F);
}

void CANopen::Frame::setCobID(uint16_t cobId)
{
    nodeId = cobId & 0x7F;
    functionCode = (cobId >> 7) & 0xF;
}
