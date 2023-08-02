#include "emergency.hpp"
#include "node.hpp"
#include "frame.hpp"
using namespace CANopen;

EMCY::EMCY(Node &node) : node(node) {}

void EMCY::receiveFrame(Frame frame)
{
}

void EMCY::publishError(uint16_t errorCode, uint8_t errorRegister)
{
}
