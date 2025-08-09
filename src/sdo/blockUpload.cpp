#include "sdoServer.hpp"

void handleBlockUploadInitialRequest(SDOFrameBlockUploadInitiateRequest &request) {
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

    // Allow to switch to upload mode if the data size is small enough
    if (request.pst() > 0 && odAccessor.getSize() <= request.pst()) {
        state = State::UploadInitialRequest;
        asyncRead(State::UploadFirstSegment);
        return;
    }

    computeCRC = request.isCRCSupported();
    blockSize = request.blockSize();
    currentBlock = 0;

    if (blockSize == 0) {
        abortOperation(SDOAbortCode_InvalidBlockSize);
        return;
    }

    asyncRead(State::BlockUploadResponse);
}

void uploadBlockFirstSegment() {
    if (odAccessor.isDomain()) {
        remainingBytes = odData.domain.size;
        dataPtr = domainBuffer;
    } else {
        remainingBytes = sdoObject.size;
        dataPtr = &odData.u8;
    }

    SDOFrameBlockUploadInitiateResponse response(
        node.nodeId, sdoObject.index, sdoObject.subindex);
    response.setCRCSupport(computeCRC);
    response.setSize(remainingBytes); // Size always supported
    state = State::BlockUploadReadyToStart;
}

void handleBlockUploadStartRequest(SDOFrameBlockUploadStartRequest &request) {
    state = State::BlockUploadStarted;
}

void blockUpload() {
    if (currntBlock == blockSize) {
        state = State::BlockUploadWaitConfirmation;
        return;
    }

    SDOFrameUploadSubBlockRequest request(node.nodeId, currentBlock);

    auto transferBytes = min(remainingBytes, 7u);
    blockBuffer.read(request.getDataPtr(), transferBytes);
    request.setFinalTransfer(transferBytes == 0);

    sendFrame(request);
    remainingBytes -= transferBytes;
}

void blockUploadWaitConfirmation(SDOFrameBlockSubBlockConfirmation &confirmation) {
    if (confirmation.getBlockNumber() != currentBlock) {
        abortOperation(SDOAbortCode_InvalidBlockNumber);
        return;
    }

    if (confirmation.isFinalTransfer()) {
        if (remainingBytes != 0) {
            abortOperation(SDOAbortCode_InvalidBlockSize);
            return;
        }
        currentBlock = 0; // Reset for next block
        state = State::Idle; // Ready for next operation
    } else {
        currentBlock++;
        blockUpload();
    }

}

struct BlockBuffer {
    uint8_t buffer[1024]; // > 127 segments of 7 bytes each, power of 2
    unsigned head = 0;
    unsigned tail = 0;

    unsigned freeSpace() const {
        return sizeof(buffer) - (head - tail);
    }

    uint8_t *tailPtr() {
        return &buffer[tail & (sizeof(buffer) - 1)];
    }

    uint8_t *headPtr() {
        return &buffer[head & (sizeof(buffer) - 1)];
    }

    bool write(const uint8_t *data, unsigned size) {
        if (size > freeSpace()) {
            return false; // Not enough space
        }
        memcpy(headPtr(), data, size);
        head += size;
        return true;
    }

    bool read(uint8_t *data, unsigned size) {
        if (size > (head - tail)) {
            return false; // Not enough data
        }
        memcpy(data, tailPtr(), size);
        tail += size;
        return true;
    }
};