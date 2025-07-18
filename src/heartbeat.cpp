/**
 * Contains the definitions of the HB class.
 */
#include "heartbeat.hpp"

#include "frame.hpp"
#include "node.hpp"

using namespace CANopen;

HB::HB() { odID = node.od().findObject(HEARTBEAT_INDEX, 0); }

void HB::publishState(NMTStates state, uint8_t toggleBit) {
    HeartbeatFrame frame(node.nodeId, state | toggleBit << TOGGLE_OFFSET);
    node.hardware().sendFrame(frame);
    lastPublish = node.getTime_us();
}

void HB::update(uint32_t timestamp_us) {  // TODO: add local data
    if (heartbeatTime_ms == 0) return;
    uint32_t heartbeatTime_us = (uint32_t)heartbeatTime_ms * 1000;
    if (heartbeatTime_ms > 0 && timestamp_us - lastPublish >= heartbeatTime_us)
        publishState(node._nmt.getState());
}

void HB::receiveFrame(Frame &frame) {
    if (frame.nodeId != node.nodeId || !frame.rtr) return;
    publishState(node._nmt.getState(), toggleBit);
    toggleBit = !toggleBit;
}

void HB::resetToggleBit() { toggleBit = 0; }

int8_t HB::setData(const Data &data, uint32_t id, SDOAbortCodes &abortCode) {
    if (id == odID) {
        heartbeatTime_ms = data.u16;
    }
    return 0;  // TODO: Is this the expected return value?
}

int8_t HB::getData(Data &data, uint32_t id, SDOAbortCodes &abortCode) {
    if (id == odID) {
        data.u16 = heartbeatTime_ms;
    }
    return 0;  // TODO: Is this the expected return value?
}
