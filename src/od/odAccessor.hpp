#pragma once

#include <cstdint>

#include "iobject-dictionnary.hpp"
#include "od.hpp"
#include "unions.hpp"

namespace CANopen {

class ODAccessor {
    ObjectDictionnary &od;

    uint32_t oid = -1;
    uint16_t index = 0;
    uint8_t subindex = 0;
    uint16_t size = 0;
    Data odData = {0};
    Metadata *metadata = nullptr;

    bool operationPending = false;
    bool operationCompleted = false;

   public:
    ODAccessor(ObjectDictionnary &od) : od(od) {}

    bool isReadable() const { return metadata->access.bits.readable; }
    bool isWriteable() const { return metadata->access.bits.writeable; }
    bool isDomain() const { return metadata->dataType == DataType::DOMAIN; }

    uint32_t getIndex() const { return index; }
    uint8_t getSubindex() const { return subindex; }
    uint16_t getSize() const { return size; }

    SDOAbortCodes lookup(uint32_t index, uint8_t subindex) {
        this->index = index;
        this->subindex = subindex;

        oid = od().findObject(index, subindex);

        if (oid == -1) {
            this->index = 0;
            this->subindex = 0;
            return SDOAbortCode_ObjectNonExistent;
        }
        if (oid == -2) {
            this->index = 0;
            this->subindex = 0;
            return SDOAbortCode_SubindexNonExistent;
        }

        // Fetch payload size
        size = od().getSize(oid);

        // Fetch metadata
        metadata = od().getMetadata(oid);
        if (metadata == nullptr) {
            return SDOAbortCode_ObjectNonExistent;
        }
        return SDOAbortCode_OK;
    }

    bool operationCompleted() const {
        return operationCompleted && !operationPending;
    }

    SDOAbortCodes asyncRead() {
        SDOAbortCodes errorCode;
        operationCompleted = false;
        operationPending = false;

        auto ret = od().readData(odData, oid, errorCode);
        if (errorCode != SDOAbortCode_OK) {
            operationPending = false;
            return errorCode;
        }

        switch (ret) {
            case 0:
                operationCompleted = true;
                break;
            case 1:
                attempts = 0;
                operationPending = true;
                break;
            case -1:  // Not implemented
            default:
                return SDOAbortCode_ObjectNonExistent;
        }
        return SDOAbortCode_OK;
    }

    SDOAbortCodes update() {
        if (!operationPending) return SDOAbortCode_OK;

        int8_t ret = od().readData(promise->data, oid, errorCode);
        if (errorCode != SDOAbortCode_OK) return errorCode;

        if (ret == 0) {
            operationPending = false;
            operationCompleted = true;
            return SDOAbortCode_OK;
        }
        if (ret == 1) {
            attempts++;
            if (attempts > MAX_ATTEMPTS) {
                operationPending = false;
                return SDOAbortCode_TimedOut;
            }
            return SDOAbortCode_OK;  // Still pending
        }
        operationPending = false;
        return SDOAbortCode_ObjectNonExistent;
    }

    void clear() {
        oid = -1;
        index = 0;
        subindex = 0;
        size = 0;
        operationPending = false;
        operationCompleted = false;
        attempts = 0;
        odData = {0};
        metadata = nullptr;
    }

    int attempts = 0;
}
};  // namespace CANopen
