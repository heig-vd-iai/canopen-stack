#include "heartbeat.hpp"
#include "node.hpp"
#include "frame.hpp"
using namespace CANopen;

HB::HB(Node &node) : node(node) {}

void HB::receiveFrame(Frame frame)
{
}

void HB::publishState(NMTStates state)
{
    Frame frame;
    frame.cobId.bits.functionCode = FunctionCode_HEARTBEAT;
    frame.cobId.bits.nodeId = node.nodeId;
    frame.dlc = 1;
    frame.data[0] = state;
    node.sendFrame(frame);
    lastPublish = node.getTime_us();
}

void HB::update(uint32_t timestamp_us)
{
#ifdef OD_OBJECT_1017
    uint16_t heartbeatTime_ms = 0;
    node.at(OD_OBJECT_1017)->getValue(0, &heartbeatTime_ms);
    uint32_t heartbeatTime_us = (uint32_t)heartbeatTime_ms * 1000;
    if (heartbeatTime_ms > 0 && timestamp_us - lastPublish >= heartbeatTime_us)
        publishState(node.nmt.getState());
#endif
}
