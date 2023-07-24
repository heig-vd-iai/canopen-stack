#include "sync.hpp"
#include "node.hpp"

CANopen_SYNC::CANopen_SYNC(CANopen_Node &node) : node(node)
{
}

void CANopen_SYNC::receiveFrame(CANopen_Frame frame)
{
}
