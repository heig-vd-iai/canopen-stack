#include "sdo/frames.hpp"
#include "sdo/sdoServer.hpp"

using namespace CANopen;
using namespace CANopen::sdo;

void SDO::onInitiateUpload(const Frame &frame, uint32_t now_us) {
    const Request request(frame);
    const SDOAbortCodes code =
        openObject(request.index(), request.subindex(), false);
    if (code != SDOAbortCode_OK) {
        abort(request.index(), request.subindex(), code, now_us);
        return;
    }
    if (transfer.domain) {
        continuation = Continuation::UploadInitiate;
        resume(now_us);
        return;
    }
    awaitOD(false, Continuation::UploadInitiate, now_us);
}

void SDO::sendUploadInitiate(uint32_t now_us) {
    transfer.size = uploadSize();
    transfer.sizeKnown = true;
    if (transfer.size <= EXPEDITED_MAX_BYTES) {
        uint8_t data[EXPEDITED_MAX_BYTES] = {0};
        const SDOAbortCodes code = readChunk(0, data, transfer.size);
        if (code != SDOAbortCode_OK) {
            abort(code, now_us);
            return;
        }
        send(uploadExpeditedFrame(nodeId, accessor.index(), accessor.subindex(),
                                  data, static_cast<uint8_t>(transfer.size)),
             now_us);
        reset();
        return;
    }
    send(uploadSegmentedFrame(nodeId, accessor.index(), accessor.subindex(),
                              transfer.size),
         now_us);
    state = State::UploadSegmented;
}

void SDO::onUploadSegment(const Frame &frame, uint32_t now_us) {
    const SegmentRequest request(frame);
    if (request.toggle() != transfer.toggle) {
        abort(SDOAbortCode_ToggleBitNotAlternated, now_us);
        return;
    }
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
    send(uploadSegmentFrame(nodeId, transfer.toggle, last, data, length),
         now_us);
    transfer.toggle = !transfer.toggle;
    if (last) reset();
}
