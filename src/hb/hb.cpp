#include "hb/hb.hpp"

#include "hb/frames.hpp"
#include "od_common.hpp"

using namespace CANopen;

namespace {

HB *boundHb = nullptr;

int8_t missingHb(SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_ObjectNonExistent;
    return -1;
}

}  // namespace

void CANopen::bindHeartbeat(HB &hb) { boundHb = &hb; }

int8_t hbGetData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundHb == nullptr) return missingHb(abortCode);
    return boundHb->readProducerTime(data, id, abortCode);
}

int8_t hbSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundHb == nullptr) return missingHb(abortCode);
    return boundHb->writeProducerTime(data, id, abortCode);
}

HB::HB(ObjectDictionnary &od, CanTransport &transport, uint8_t nodeId)
    : od(od), transport(transport), nodeId(nodeId) {}

void HB::init() {
    producerTimeId = od.findObject(hb::PRODUCER_TIME_INDEX);
    const Metadata *metadata = od.getMetadata(producerTimeId);
    period_ms = metadata == nullptr ? 0 : metadata->getDefaultValue().u16;
}

void HB::publish(NMTStates published, bool toggleBit, uint32_t now_us) {
    transport.sendFrame(hb::makeMessage(nodeId, published, toggleBit));
    lastPublish_us = now_us;
}

void HB::update(uint32_t now_us) {
    if (period_ms == 0) return;
    if (now_us - lastPublish_us >= period_ms * hb::PERIOD_UNIT_US)
        publish(state, false, now_us);
}

void HB::onFrame(Frame &frame, uint32_t now_us) {
    if (frame.nodeId != nodeId || !frame.rtr) return;
    publish(state, toggle, now_us);
    toggle = !toggle;
}

void HB::onNmtState(NMTStates next) {
    if (next == NMTState_Initialisation) toggle = false;
    if (state == NMTState_Initialisation && next == NMTState_PreOperational)
        publish(NMTState_Initialisation, false, transport.getTime_us());
    state = next;
}

int8_t HB::readProducerTime(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (id != producerTimeId) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    data.u16 = period_ms;
    return 0;
}

int8_t HB::writeProducerTime(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) {
    if (id != producerTimeId) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    period_ms = data.u16;
    lastPublish_us = transport.getTime_us();
    return 0;
}
