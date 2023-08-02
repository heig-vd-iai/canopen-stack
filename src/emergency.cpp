#include "emergency.hpp"
#include "node.hpp"
#include "frame.hpp"
using namespace CANopen;

EMCY::EMCY(Node &node) : node(node) {}

void EMCY::enable() { enabled = true; }

void EMCY::disable() { enabled = false; }

void EMCY::receiveFrame(Frame frame)
{
    if (!enabled)
        return;
}

void EMCY::publishError(uint16_t errorCode, uint8_t errorRegister)
{
    if (!enabled)
        return;
    Frame frame;
    frame.cobId.bits.functionCode = FunctionCode_EMCY;
    frame.cobId.bits.nodeId = node.nodeId;
    frame.dlc = 8;
    *(uint16_t *)frame.data = errorCode;
    frame.data[2] = errorRegister;
    node.sendFrame(frame);
}

void EMCY::publishResetError()
{
    // if (!enabled)
    //     return;
    publishError(0, 0);
}
