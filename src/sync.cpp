#include "sync.hpp"

#include "enums.hpp"
#include "frame.hpp"
#include "od_common.hpp"
#include "pdo/pdo.hpp"

using namespace CANopen;

namespace {
SYNC *boundSync = nullptr;
}

void CANopen::bindSync(SYNC &sync) { boundSync = &sync; }

int8_t syncGetData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundSync == nullptr) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return boundSync->getData(data, id, abortCode);
}

int8_t syncSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundSync == nullptr) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return boundSync->setData(data, id, abortCode);
}

SYNC::SYNC(ObjectDictionnary &od, PDO *pdo) : od(od), pdo(pdo) {}

void SYNC::init() {
    odID = od.findObject(SYNC_INDEX);
    if (odID < 0) {
        maxCounter = MAX_COUNTER;
    } else {
        Data tmp;
        SDOAbortCodes abortCode;
        getLocalData_uint8_t(tmp, odID, abortCode);
        maxCounter = tmp.u8 < MIN_COUNTER ? MAX_COUNTER : tmp.u8;
    }
}

void SYNC::update(uint32_t) {}

void SYNC::onNmtState(NMTStates state) {
    enabled = state == NMTState_PreOperational || state == NMTState_Operational;
}

void SYNC::onFrame(Frame &frame, uint32_t now_us) {
    SYNCFrame &syncFrame = static_cast<SYNCFrame &>(frame);
    if (!enabled || syncFrame.nodeId != 0) return;
    // In case a sync frame is received without a value, increment internal
    // counter, otherwise copy.
    internalCounter = syncFrame.isCounter() ? syncFrame.getCounter()
                                            : internalCounter % maxCounter + 1;
    if (onSyncFunc) onSyncFunc(internalCounter);
    if (pdo != nullptr) pdo->onSync(internalCounter, now_us);
}

void SYNC::onSync(std::function<void(unsigned)> callback) {
    onSyncFunc = callback;
}

int8_t SYNC::getData(Data &data, int32_t odID, SDOAbortCodes &abortCode) {
    if (odID != this->odID) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    data.u8 = maxCounter;
    abortCode = SDOAbortCode_OK;
    return 0;
}

int8_t SYNC::setData(const Data &data, int32_t odID, SDOAbortCodes &abortCode) {
    if (odID != this->odID) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (maxCounter != 0) {
        abortCode = SDOAbortCode_CannotStoreOrTransfer_DeviceState;
    }
    if (!(data.u8 == 0 || (MIN_COUNTER <= data.u8 && data.u8 <= MAX_COUNTER))) {
        abortCode = SDOAbortCode_InvalidDownloadParameterValue;
        return -1;
    }
    maxCounter = data.u8;
    abortCode = SDOAbortCode_OK;
    return 0;
}
