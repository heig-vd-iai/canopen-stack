#include "heartbeat.hpp"
#include "node.hpp"

CANopen_HB::CANopen_HB(CANopen_Node &node) : node(node)
{
}

void CANopen_HB::receiveFrame(CANopen_Frame frame)
{
}

void CANopen_HB::publishState(NMTStates state)
{
    CANopen_Frame frame;
    frame.nodeId = node.nodeId;
    frame.dlc = 1;
    frame.data[0] = state;
    frame.functionCode = FunctionCode_HEARTBEAT;
    node.sendFrame(frame);
}

void CANopen_HB::update()
{
}
