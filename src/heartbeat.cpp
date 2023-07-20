#include "heartbeat.hpp"
#include "node.hpp"
#include <cstdlib>

CANopen_HB::CANopen_HB(CANopen_Node &node) : node(node), lastPublish(0), producerHeartbeatTime(NULL)
{
    Object *object = node.od.findObject(0x1017); // TODO
    if (object == NULL)
        return;
    producerHeartbeatTime = (uint16_t *)object->entries[0].dataSrc;
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
    if (producerHeartbeatTime == NULL || *producerHeartbeatTime == 0)
        return;
    if (timestamp_us - lastPublish >= *producerHeartbeatTime * 1000)
    {
        publishState(node.nmt.getState());
    }
}
