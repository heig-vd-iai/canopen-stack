#include "sync.hpp"

#include "enums.hpp"
#include "frame.hpp"
#include "node.hpp"
// #include "objects/object_1019.hpp"
using namespace CANopen;

SYNC::SYNC() {}

void SYNC::init() {
    odID = node.od().findObject(SYNC_INDEX);
    if (odID < 0) {
        maxCounter = MAX_COUNTER;
    } else {
        Data tmp;
        SDOAbortCodes aborteCode;
        getLocalData_uint8_t(tmp, odID, aborteCode);
        maxCounter = tmp.u8 < MIN_COUNTER ? MAX_COUNTER : tmp.u8;
    }
}

void SYNC::enable() { enabled = true; }

void SYNC::disable() { enabled = false; }

void SYNC::receiveFrame(SYNCFrame &frame, uint32_t timestamp_us) {
    if (!enabled || frame.nodeId != 0) return;
    // In case a sync frame is received without a value, increment internal
    // counter, otherwise copy.
    internalCounter = frame.isCounter() ? frame.getCounter()
                                        : internalCounter % maxCounter + 1;
    if (onSyncFunc) onSyncFunc(internalCounter);
    node._pdo.onSync(internalCounter, timestamp_us);
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
