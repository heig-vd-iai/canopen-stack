/**
 * Contains the definitions of the HB class.
 */
#include "heartbeat.hpp"
#include "frame.hpp"
#include "node.hpp"
using namespace CANopen;

HB::HB(Node &node) : node(node) {}

void HB::publishState(NMTStates state, uint8_t toggleBit)
{
    HeartbeatFrame frame(node.nodeId, state | toggleBit << TOGGLE_OFFSET);
    node.sendFrame(frame);
    lastPublish = node.getTime_us();
}

void HB::update(uint32_t timestamp_us)
{
#ifdef OD_OBJECT_1017
    uint16_t heartbeatTime_ms = 0;
    node._od.at(OD_OBJECT_1017)->getValue(0, &heartbeatTime_ms);
    uint32_t heartbeatTime_us = (uint32_t)heartbeatTime_ms * 1000;
    if (heartbeatTime_ms > 0 && timestamp_us - lastPublish >= heartbeatTime_us)
        publishState(node._nmt.getState());
#endif
}

void HB::receiveFrame(Frame &frame)
{
    if (frame.nodeId != node.nodeId || !frame.rtr)
        return;
    publishState(node._nmt.getState(), toggleBit);
    toggleBit = !toggleBit;
}

void HB::resetToggleBit()
{
    toggleBit = 0;
}
