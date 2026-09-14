/**
 * Contains the definitions of the HB class.
 */
#include "heartbeat.hpp"

#include "frame.hpp"
#include "od_common.hpp"

using namespace CANopen;

namespace {
HB *boundHb = nullptr;
}

void CANopen::bindHeartbeat(HB &hb) { boundHb = &hb; }

int8_t hbGetData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundHb == nullptr) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return boundHb->getData(data, id, abortCode);
}

int8_t hbSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundHb == nullptr) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return boundHb->setData(data, id, abortCode);
}

HB::HB(ObjectDictionnary &od, CanTransport &transport, uint8_t nodeId)
    : od(od), transport(transport), nodeId(nodeId) {}

void HB::init() { odID = od.findObject(HEARTBEAT_INDEX, 0); }

void HB::publishState(NMTStates state, uint8_t toggleBit) {
    HeartbeatFrame frame(nodeId, state | toggleBit << TOGGLE_OFFSET);
    transport.sendFrame(frame);
    lastPublish = transport.getTime_us();
}

void HB::update(uint32_t now_us) {
    if (heartbeatTime_ms == 0) return;
    uint32_t heartbeatTime_us = static_cast<uint32_t>(heartbeatTime_ms) * 1000;
    if (now_us - lastPublish >= heartbeatTime_us) publishState(state);
}

void HB::onFrame(Frame &frame, uint32_t) {
    if (frame.nodeId != nodeId || !frame.rtr) return;
    publishState(state, toggleBit);
    toggleBit = !toggleBit;
}

void HB::onNmtState(NMTStates state) {
    if (state == NMTState_Initialisation) toggleBit = 0;
    if (this->state == NMTState_Initialisation &&
        state == NMTState_PreOperational)
        publishState(NMTState_Initialisation);
    this->state = state;
}

int8_t HB::setData(const Data &data, int32_t id, SDOAbortCodes &) {
    if (id == odID) {
        heartbeatTime_ms = data.u16;
    }
    return 0;  // TODO: Is this the expected return value?
}

int8_t HB::getData(Data &data, int32_t id, SDOAbortCodes &) {
    if (id == odID) {
        data.u16 = heartbeatTime_ms;
    }
    return 0;  // TODO: Is this the expected return value?
}
