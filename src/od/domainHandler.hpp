#pragma once

#include <cstdint>

#include "enums.hpp"

namespace CANopen {

/**
 * Application hook for DOMAIN objects.
 * The SDO server never stores a DOMAIN in RAM: download data is handed over
 * chunk by chunk and upload data is pulled chunk by chunk. The bootloader
 * connects this interface to its flash driver.
 */
class DomainHandler {
   public:
    virtual ~DomainHandler() = default;

    /**
     * @param size Total size announced by the client, 0 when unknown.
     */
    virtual SDOAbortCodes beginDownload(int32_t id, uint32_t size) = 0;
    virtual SDOAbortCodes downloadChunk(int32_t id, uint32_t offset,
                                        const uint8_t *bytes,
                                        uint32_t length) = 0;
    virtual SDOAbortCodes endDownload(int32_t id, uint32_t size) = 0;
    virtual void abortDownload(int32_t id) = 0;

    virtual uint32_t uploadSize(int32_t id) = 0;
    virtual SDOAbortCodes uploadChunk(int32_t id, uint32_t offset,
                                      uint8_t *bytes, uint32_t length) = 0;
};

}  // namespace CANopen
