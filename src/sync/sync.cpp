#include "sync/sync.hpp"

#include "od_common.hpp"
#include "sync/frames.hpp"

using namespace CANopen;

namespace {

SYNC *boundSync = nullptr;

int8_t missingSync(SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_ObjectNonExistent;
    return -1;
}

int8_t fail(SDOAbortCodes &abortCode, SDOAbortCodes code) {
    abortCode = code;
    return -1;
}

}  // namespace

void CANopen::bindSync(SYNC &sync) { boundSync = &sync; }

int8_t syncGetData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundSync == nullptr) return missingSync(abortCode);
    return boundSync->readCounterOverflow(data, id, abortCode);
}

int8_t syncSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundSync == nullptr) return missingSync(abortCode);
    return boundSync->writeCounterOverflow(data, id, abortCode);
}

SYNC::SYNC(ObjectDictionnary &od, SyncListener *listener)
    : od(od), listener(listener) {}

void SYNC::init() {
    overflowId = od.findObject(sync::COUNTER_OVERFLOW_INDEX);
    const Metadata *metadata = od.getMetadata(overflowId);
    overflow = metadata == nullptr ? sync::COUNTER_NONE
                                   : metadata->getDefaultValue().u8;
    if (!sync::isValidOverflow(overflow)) overflow = sync::COUNTER_NONE;
    loadCobId();
}

void SYNC::loadCobId() {
    Data data;
    data.u64 = 0;
    syncCobId = od.readData(data, sync::COBID_INDEX, 0) == 0
                    ? static_cast<uint16_t>(data.u32 & sync::COBID_ID_MASK)
                    : sync::DEFAULT_COBID;
}

void SYNC::onNmtState(NMTStates state) {
    if (state == NMTState_PreOperational) loadCobId();
    enabled = state == NMTState_PreOperational || state == NMTState_Operational;
}

void SYNC::onFrame(Frame &frame, uint32_t now_us) {
    if (!enabled || frame.getCobID() != syncCobId) return;
    const sync::Message message(frame);
    if (!message.isWellFormed()) return;
    if (message.hasCounter()) {
        counter = message.counter();
    } else {
        const uint8_t wrap =
            overflow == sync::COUNTER_NONE ? sync::COUNTER_MAX : overflow;
        counter = static_cast<uint8_t>(counter % wrap + 1);
    }
    if (callback) callback(counter);
    if (listener != nullptr) listener->onSync(counter, now_us);
}

void SYNC::onSync(std::function<void(unsigned)> function) {
    callback = function;
}

int8_t SYNC::readCounterOverflow(Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) {
    if (overflowId < 0 || id != overflowId)
        return fail(abortCode, SDOAbortCode_ObjectNonExistent);
    abortCode = SDOAbortCode_OK;
    data.u8 = overflow;
    return 0;
}

int8_t SYNC::writeCounterOverflow(const Data &data, int32_t id,
                                  SDOAbortCodes &abortCode) {
    if (overflowId < 0 || id != overflowId)
        return fail(abortCode, SDOAbortCode_ObjectNonExistent);
    if (!sync::isValidOverflow(data.u8))
        return fail(abortCode, SDOAbortCode_InvalidDownloadParameterValue);
    // CiA301:2011§7.5.2.34: only changes from or to 0 are allowed.
    if (overflow != sync::COUNTER_NONE && data.u8 != sync::COUNTER_NONE &&
        data.u8 != overflow)
        return fail(abortCode, SDOAbortCode_CannotStoreOrTransfer_DeviceState);
    abortCode = SDOAbortCode_OK;
    overflow = data.u8;
    return 0;
}
