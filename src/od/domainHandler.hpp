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

    /**
     * Called when an upload is initiated, before uploadSize(). The server
     * then calls pollUpload() until it returns 0, so the handler can fetch
     * the data asynchronously. A non-OK abortCode aborts the transfer.
     */
    virtual SDOAbortCodes beginUpload(int32_t id) {
        (void)id;
        return SDOAbortCode_OK;
    }
    /**
     * @return 0 when the data is ready or the fetch failed, 1 while pending.
     */
    virtual int8_t pollUpload(int32_t id, SDOAbortCodes &abortCode) {
        (void)id;
        abortCode = SDOAbortCode_OK;
        return 0;
    }
    /**
     * Called when the transfer is dropped while pollUpload() is pending.
     */
    virtual void abortUpload(int32_t id) { (void)id; }

    virtual uint32_t uploadSize(int32_t id) = 0;
    virtual SDOAbortCodes uploadChunk(int32_t id, uint32_t offset,
                                      uint8_t *bytes, uint32_t length) = 0;
};

}  // namespace CANopen
