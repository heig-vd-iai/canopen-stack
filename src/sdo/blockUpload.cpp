#include "sdo/frames.hpp"
#include "sdo/sdoServer.hpp"

using namespace CANopen;
using namespace CANopen::sdo;

void SDO::onBlockUploadInitiate(const Frame &frame, uint32_t now_us) {
    const BlockUploadRequest request(frame);
    if (request.subcommand() != BLOCK_CS_INITIATE) {
        abort(request.index(), request.subindex(),
              SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    const SDOAbortCodes code =
        openObject(request.index(), request.subindex(), false);
    if (code != SDOAbortCode_OK) {
        abort(request.index(), request.subindex(), code, now_us);
        return;
    }
    const uint8_t blockSize = request.blockSize();
    if (blockSize < 1 || blockSize > BLOCK_SEQNO_MAX) {
        abort(SDOAbortCode_InvalidBlockSize, now_us);
        return;
    }
    transfer.crcEnabled = request.crcSupported();
    transfer.blockSize = blockSize;
    if (transfer.domain) {
        continuation = Continuation::BlockUploadInitiate;
        resume(now_us);
        return;
    }
    awaitOD(false, Continuation::BlockUploadInitiate, now_us);
}

void SDO::sendBlockUploadInitiate(uint32_t now_us) {
    transfer.size = uploadSize();
    transfer.sizeKnown = true;
    transfer.offset = 0;
    transfer.blockStart = 0;
    transfer.seqno = 1;
    transfer.lastSeen = false;
    send(blockUploadInitiateFrame(nodeId, accessor.index(), accessor.subindex(),
                                  transfer.crcEnabled, transfer.size),
         now_us);
    state = State::BlockUploadReady;
}

void SDO::onBlockUploadStart(const Frame &frame, uint32_t now_us) {
    const BlockUploadRequest request(frame);
    if (request.subcommand() != BLOCK_CS_UPLOAD_START) {
        abort(SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    lastActivity_us = now_us;
    state = State::BlockUploadSending;
}

void SDO::sendBlockUploadSegment(uint32_t now_us) {
    const uint32_t remaining = transfer.size - transfer.offset;
    const uint8_t length = static_cast<uint8_t>(
        remaining < SEGMENT_MAX_BYTES ? remaining : SEGMENT_MAX_BYTES);
    uint8_t data[SEGMENT_MAX_BYTES] = {0};
    const SDOAbortCodes code = readChunk(transfer.offset, data, length);
    if (code != SDOAbortCode_OK) {
        abort(code, now_us);
        return;
    }
    transfer.offset += length;
    const bool last = transfer.offset == transfer.size;
    send(blockUploadSegmentFrame(nodeId, transfer.seqno, last, data, length),
         now_us);
    transfer.lastSeen = last;
    if (last || transfer.seqno == transfer.blockSize) {
        state = State::BlockUploadAwaitAck;
        return;
    }
    transfer.seqno = static_cast<uint8_t>(transfer.seqno + 1);
}

void SDO::onBlockUploadAck(const Frame &frame, uint32_t now_us) {
    const BlockUploadRequest request(frame);
    if (request.subcommand() != BLOCK_CS_UPLOAD_RESPONSE) {
        abort(SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    const uint8_t nextBlockSize = request.nextBlockSize();
    if (nextBlockSize < 1 || nextBlockSize > BLOCK_SEQNO_MAX) {
        abort(SDOAbortCode_InvalidBlockSize, now_us);
        return;
    }
    const uint8_t ackseq = request.ackseq();
    if (ackseq > transfer.seqno) {
        abort(SDOAbortCode_InvalidSequenceNumber, now_us);
        return;
    }
    const bool complete = ackseq == transfer.seqno && transfer.lastSeen;
    lastActivity_us = now_us;
    if (complete) {
        sendBlockUploadEnd(now_us);
        return;
    }
    transfer.offset =
        transfer.blockStart + static_cast<uint32_t>(ackseq) * SEGMENT_MAX_BYTES;
    transfer.blockStart = transfer.offset;
    transfer.blockSize = nextBlockSize;
    transfer.seqno = 1;
    transfer.lastSeen = false;
    state = State::BlockUploadSending;
}

uint16_t SDO::uploadCrc() {
    Crc16Ccitt crc;
    uint8_t data[SEGMENT_MAX_BYTES];
    for (uint32_t offset = 0; offset < transfer.size;
         offset += SEGMENT_MAX_BYTES) {
        const uint32_t remaining = transfer.size - offset;
        const uint32_t length =
            remaining < SEGMENT_MAX_BYTES ? remaining : SEGMENT_MAX_BYTES;
        if (readChunk(offset, data, length) != SDOAbortCode_OK) return 0;
        crc.feed(data, length);
    }
    return crc.result();
}

void SDO::sendBlockUploadEnd(uint32_t now_us) {
    const uint32_t lastLength =
        transfer.size == 0 ? 0 : ((transfer.size - 1) % SEGMENT_MAX_BYTES) + 1;
    const uint8_t unused = static_cast<uint8_t>(SEGMENT_MAX_BYTES - lastLength);
    const uint16_t crc = transfer.crcEnabled ? uploadCrc() : 0;
    send(blockUploadEndFrame(nodeId, unused, crc), now_us);
    state = State::BlockUploadEnding;
}

void SDO::onBlockUploadEnd(const Frame &frame, uint32_t now_us) {
    const BlockUploadRequest request(frame);
    if (request.subcommand() != BLOCK_CS_END) {
        abort(SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    reset();
}
