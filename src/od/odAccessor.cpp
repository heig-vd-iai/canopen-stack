#include "od/odAccessor.hpp"

#include <cstring>

#include "sdo/config.hpp"

using namespace CANopen;

SDOAbortCodes ODAccessor::lookup(uint16_t index, uint8_t subindex) {
    clear();
    objectIndex = index;
    objectSubindex = subindex;
    objectId = od.findObject(index, subindex);
    if (objectId < 0) {
        objectId = -1;
        return od.findObject(index, 0) < 0 ? SDOAbortCode_ObjectNonExistent
                                           : SDOAbortCode_SubindexNonExistent;
    }
    metadata = od.getMetadata(objectId);
    if (metadata == nullptr) {
        objectId = -1;
        return SDOAbortCode_ObjectNonExistent;
    }
    objectSize = isDomain() ? 0 : od.getSize(objectId);
    return SDOAbortCode_OK;
}

void ODAccessor::clear() {
    metadata = nullptr;
    objectId = -1;
    objectIndex = 0;
    objectSubindex = 0;
    objectSize = 0;
    memset(&data, 0, sizeof(data));
    state = Status::Idle;
    abortCode = SDOAbortCode_OK;
}

ODAccessor::Status ODAccessor::startRead(uint32_t now_us) {
    operation = Operation::Read;
    deadline_us = now_us + sdo::REMOTE_TIMEOUT_US;
    abortCode = SDOAbortCode_OK;
    return finish(od.readData(data, objectId, abortCode));
}

ODAccessor::Status ODAccessor::startWrite(uint32_t now_us) {
    operation = Operation::Write;
    deadline_us = now_us + sdo::REMOTE_TIMEOUT_US;
    abortCode = SDOAbortCode_OK;
    return finish(od.writeData(data, objectId, abortCode));
}

ODAccessor::Status ODAccessor::poll(uint32_t now_us) {
    if (state != Status::Pending) return state;
    abortCode = SDOAbortCode_OK;
    const int8_t result = operation == Operation::Read
                              ? od.readData(data, objectId, abortCode)
                              : od.writeData(data, objectId, abortCode);
    finish(result);
    if (state == Status::Pending &&
        static_cast<int32_t>(now_us - deadline_us) >= 0) {
        state = Status::Failed;
        abortCode = SDOAbortCode_AccessFailedHardwareError;
    }
    return state;
}

ODAccessor::Status ODAccessor::finish(int8_t result) {
    if (abortCode != SDOAbortCode_OK) {
        state = Status::Failed;
    } else if (result == 0) {
        state = Status::Done;
    } else if (result == 1) {
        state = Status::Pending;
    } else {
        state = Status::Failed;
        abortCode = SDOAbortCode_UnsupportedObjectAccess;
    }
    return state;
}
