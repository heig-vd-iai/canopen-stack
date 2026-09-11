#include "sdo/frames.hpp"
#include "sdo/sdoServer.hpp"

using namespace CANopen;
using namespace CANopen::sdo;

void SDO::onBlockDownloadInitiate(const Frame &frame, uint32_t now_us) {
    const BlockDownloadInitiateRequest request(frame);
    if (request.subcommand() != BLOCK_CS_INITIATE) {
        abort(request.index(), request.subindex(),
              SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    const SDOAbortCodes code =
        openObject(request.index(), request.subindex(), true);
    if (code != SDOAbortCode_OK) {
        abort(request.index(), request.subindex(), code, now_us);
        return;
    }
    const uint32_t announced = request.sizeIndicated() ? request.data32() : 0;
    if (request.sizeIndicated() && !transfer.domain &&
        announced != transfer.size) {
        abort(announced < transfer.size
                  ? SDOAbortCode_DataTypeMismatch_LengthParameterTooLow
                  : SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh,
              now_us);
        return;
    }
    if (transfer.domain) {
        const SDOAbortCodes domainCode = beginDomainDownload(announced);
        if (domainCode != SDOAbortCode_OK) {
            abort(domainCode, now_us);
            return;
        }
        transfer.sizeKnown = request.sizeIndicated();
        transfer.size = announced;
    }
    transfer.crcEnabled = request.crcSupported();
    transfer.blockSize = BLOCK_SIZE;
    transfer.seqno = 1;
    transfer.ackseq = 0;
    transfer.gap = false;
    transfer.lastSeen = false;
    send(blockDownloadInitiateFrame(nodeId, accessor.index(),
                                    accessor.subindex(), transfer.crcEnabled,
                                    transfer.blockSize),
         now_us);
    state = State::BlockDownloading;
    blockDeadline_us = now_us + BLOCK_TIMEOUT_US;
}

void SDO::onBlockDownloadSegment(const Frame &frame, uint32_t now_us) {
    const BlockSegment segment(frame);
    if (segment.byte0() == (CCS_ABORT << 5)) {
        reset();
        return;
    }
    const uint8_t seqno = segment.seqno();
    if (seqno == 0 || seqno > transfer.blockSize) {
        abort(SDOAbortCode_InvalidSequenceNumber, now_us);
        return;
    }
    lastActivity_us = now_us;
    blockDeadline_us = now_us + BLOCK_TIMEOUT_US;
    blockTimeoutAcked = false;
    if (!transfer.gap && seqno == transfer.seqno) {
        memcpy(transfer.buffer +
                   static_cast<uint32_t>(seqno - 1) * SEGMENT_MAX_BYTES,
               segment.data(), SEGMENT_MAX_BYTES);
        transfer.ackseq = seqno;
        transfer.seqno = static_cast<uint8_t>(seqno + 1);
        transfer.lastSeen = segment.last();
    } else {
        transfer.gap = true;
    }
    if (seqno == transfer.blockSize || segment.last()) deliverBlock(now_us);
}

/**
 * Hands the acknowledged segments over, except the last one when it closes
 * the transfer: its padding is only known from the end request.
 */
void SDO::deliverBlock(uint32_t now_us) {
    const uint8_t delivered = static_cast<uint8_t>(
        transfer.lastSeen ? transfer.ackseq - 1 : transfer.ackseq);
    const uint32_t length =
        static_cast<uint32_t>(delivered) * SEGMENT_MAX_BYTES;
    if (length > 0) {
        const SDOAbortCodes code =
            writeChunk(transfer.offset, transfer.buffer, length);
        if (code != SDOAbortCode_OK) {
            abort(code, now_us);
            return;
        }
        transfer.crc.feed(transfer.buffer, length);
        transfer.offset += length;
    }
    if (transfer.lastSeen)
        memmove(transfer.buffer, transfer.buffer + length, SEGMENT_MAX_BYTES);
    send(blockDownloadAckFrame(nodeId, transfer.ackseq, transfer.blockSize),
         now_us);
    transfer.seqno = 1;
    transfer.ackseq = 0;
    transfer.gap = false;
    blockDeadline_us = now_us + BLOCK_TIMEOUT_US;
    if (transfer.lastSeen) state = State::BlockDownloadEnding;
}

void SDO::onBlockDownloadEnd(const Frame &frame, uint32_t now_us) {
    const BlockDownloadEndRequest request(frame);
    if (bits::field(request.byte0(), 0, 1) != BLOCK_CS_END) {
        abort(SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    const uint8_t unused = request.unusedBytes();
    if (unused > SEGMENT_MAX_BYTES) {
        abort(SDOAbortCode_GeneralParameterIncompatibility, now_us);
        return;
    }
    const uint32_t length = static_cast<uint32_t>(SEGMENT_MAX_BYTES - unused);
    const SDOAbortCodes code =
        writeChunk(transfer.offset, transfer.buffer, length);
    if (code != SDOAbortCode_OK) {
        abort(code, now_us);
        return;
    }
    transfer.crc.feed(transfer.buffer, length);
    transfer.offset += length;
    if (transfer.crcEnabled && request.crc() != transfer.crc.result()) {
        abort(SDOAbortCode_CRCError, now_us);
        return;
    }
    finishDownload(Continuation::BlockDownloadDone, now_us);
}

void SDO::sendBlockDownloadEnd(uint32_t now_us) {
    send(blockDownloadEndFrame(nodeId), now_us);
    reset();
}
