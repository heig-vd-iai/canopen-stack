#include "sdoServer.hpp"

/**
 * Client wants to read an object.
 * This is the first step in the SDO upload process.
 */
void SDO::handleInitiateUploadRequest(SDOFrameInitiateUploadRequest &request) {
    uint32_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();

    SDOAbortCodes errorCode = SDOAbortCode_OK;

    if ((errorCode = odAccessor.lookup(index, subindex)) != SDOAbortCode_OK) {
        abortOperation(errorCode);
        return;
    }

    if (!odAccessor.isReadable()) {
        abortOperation(SDOAbortCode_AttemptReadOnWriteOnly);
        return;
    }

    asyncRead(State::UploadFirstSegment);
}

/**
 * Once the object data is ready,
 * we can send the first segment of the upload.
 */
void SDO::uploadFirstSegment(SDOFrameInitiateUploadRequest &request) {
    uint8_t *dataPtr;
    if (objectData.isDomain()) {
        remainingBytes = odData.domain.size;
        dataPtr = domainBuffer;
    } else {
        remainingBytes = sdoObject.size;
        dataPtr = &odData.u8;
    }

    SDOFrameInitiateUploadResponse response(node.nodeId, sdoObject.index,
                                            sdoObject.subindex);

    // If data size overflows the frame, segmented transfer is enabled
    // thus, 0 bytes are sent in the first segment, but the size is set
    // to the total size of the object.
    auto transferSize = response.setData(&odData.u8, remainingBytes);
    remainingBytes -= transferSize;

    // If the transfer size is 0, we are done otherwise
    // we switch to segmented transfer state.
    state = remainingBytes ? State::UploadSubsequentSegment : State::Idle;

    sendFrame(response);
    toggleBit = false;  // For segmented transfer, toggle bit starts at false
}

/**
 * For segmented uploads, we handle subsequent segments.
 * This is the second step in the SDO upload process.
 */
void SDO::handleUploadSubsequentSegment(SDOFrameUploadSegmentRequest &request) {
    // Client must start the upload with a toggle bit set to false.
    if (toggleBit != request.toggleBit()) {
        abortOperation(SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    toggleBit = !toggleBit;

    SDOFrameUploadSegmentResponse response(node.nodeId, toggleBit);
    remainingBytes -= response.setData(&odData.u8, remainingBytes);

    sendFrame(response);

    if (response.finalTransfer()) state = State::Idle;
}
