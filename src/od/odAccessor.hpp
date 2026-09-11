#pragma once

#include <cstdint>

#include "enums.hpp"
#include "iobject-dictionnary.hpp"
#include "od.hpp"
#include "od/domainHandler.hpp"
#include "unions.hpp"

namespace CANopen {

/**
 * Object dictionary access for the SDO server.
 * Resolves one object at a time and hides the asynchronous protocol of
 * remote objects: a read or write is started, then polled until it
 * completes, fails or times out.
 */
class ODAccessor {
   public:
    enum class Status { Idle, Pending, Done, Failed };

    explicit ODAccessor(ObjectDictionnary &od) : od(od) {}

    void setDomainHandler(DomainHandler *handler) { domainHandler = handler; }
    DomainHandler *domain() const { return domainHandler; }

    SDOAbortCodes lookup(uint16_t index, uint8_t subindex);
    void clear();

    int32_t id() const { return objectId; }
    uint16_t index() const { return objectIndex; }
    uint8_t subindex() const { return objectSubindex; }
    uint32_t size() const { return objectSize; }
    bool isReadable() const { return metadata->access.bits.readable; }
    bool isWriteable() const { return metadata->access.bits.writeable; }
    bool isDomain() const { return metadata->dataType == DataType::DOMAIN; }

    uint8_t *bytes() { return &data.u8; }
    const uint8_t *bytes() const { return &data.u8; }

    Status startRead(uint32_t now_us);
    Status startWrite(uint32_t now_us);
    Status poll(uint32_t now_us);
    Status status() const { return state; }
    SDOAbortCodes error() const { return abortCode; }

   private:
    enum class Operation { Read, Write };

    Status finish(int8_t result);

    ObjectDictionnary &od;
    DomainHandler *domainHandler = nullptr;
    Metadata *metadata = nullptr;
    int32_t objectId = -1;
    uint16_t objectIndex = 0;
    uint8_t objectSubindex = 0;
    uint32_t objectSize = 0;
    Data data;
    Operation operation = Operation::Read;
    Status state = Status::Idle;
    SDOAbortCodes abortCode = SDOAbortCode_OK;
    uint32_t deadline_us = 0;
};

}  // namespace CANopen
