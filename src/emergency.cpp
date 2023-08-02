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
}
