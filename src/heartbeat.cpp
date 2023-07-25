#include "heartbeat.hpp"
#include "node.hpp"
#include <cstdlib>

CANopen_HB::CANopen_HB(CANopen_Node &node) : node(node)
{
    heartbeatTimeObject = node.od.findObject(HB_OBJECT_1017);
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
    lastPublish = node.getTime_us();
}

void CANopen_HB::update(uint32_t timestamp_us)
{
    if (heartbeatTimeObject == NULL)
        return;
    uint16_t heartbeatTime = 0;
    heartbeatTimeObject->getValue(0, &heartbeatTime);
    if (heartbeatTime != 0 && timestamp_us - lastPublish >= ((uint32_t)(heartbeatTime)) * 1000)
    {
        publishState(node.nmt.getState());
    }
}
