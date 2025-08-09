/**
 * Protocol SDO download.
 *
 * SDOFrameDownloadInitateRequest -> SDOFrameDownloadInitiateResponse
 * (if more data to receive)
 * SDOFrameDownloadSegmentRequest -> SDOFrameDownloadSegmentResponse
 *             ^                                      |
 *             '------- (if more data to receive) <---'
 */
#include "sdoServer.hpp"

/**
 * Client wants to write an object.
 * This is the first step in the SDO download process.
 */
void SDO::handleInitiateDownloadRequest(
    SDOFrameInitiateDownloadRequest &request) {
    uint32_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();

    SDOAbortCodes errorCode = SDOAbortCode_OK;

    if ((errorCode = odAccessor.lookup(index, subindex)) != SDOAbortCode_OK) {
        abortOperation(errorCode);
        return;
    }

    if (!odAccessor.isWritable()) {
        abortOperation(SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }

    uint8_t *dataPtr;
    if (objectData.isDomain()) {
        remainingBytes = odData.domain.size;
        dataPtr = domainBuffer;
    } else {
        remainingBytes = sdoObject.size;
        dataPtr = &odData.u8;
    }

    toggleBit = false;  // For segmented transfer, toggle bit starts at false

    remainingBytes -= request.getSize();
    if (remainingBytes < 0) {
        abortOperation(SDOAbortCode_InvalidDownloadParameterValue);
        return;
    }

    dataPtr += request.getData(dataPtr);

    if (request.finalTransfer()) {
        if (remainingBytes != 0) {
            abortOperation(SDOAbortCode_InvalidDownloadParameterValue);
            return;
        }
        asyncWrite(State::DownloadInitiateEnd);
    } else {
        downloadInitiateEnd();
    }
}

/**
 * For segmented uploads, we handle subsequent segments.
 * This is the second step in the SDO upload process.
 */
void SDO::handleDownloadSubsequentSegment(
    SDOFrameDownloadSegmentRequest &request) {
    if (toggleBit != request.toggleBit()) {
        abortOperation(SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    toggleBit = !toggleBit;

    remainingBytes -= request.getSize();
    if (remainingBytes < 0) {
        abortOperation(SDOAbortCode_InvalidDownloadParameterValue);
        return;
    }
    dataPtr += request.getData(dataPtr);

    if (request.finalTransfer()) {
        if (remainingBytes != 0) {
            abortOperation(SDOAbortCode_InvalidDownloadParameterValue);
            return;
        }
        asyncWrite(State::DownloadSegmentEnd);
    } else {
        downloadSegmentEnd();
    }
}

void SDO::downloadInitiateEnd() {
    sendFrame(SDOFrameInitiateDownloadResponse(node.nodeId, sdoObject.index,
                                               sdoObject.subindex));
}

void SDO::downloadSegmentEnd() {
    sendFrame(SDOFrameDownloadSegmentResponse(node.nodeId, toggleBit));
}