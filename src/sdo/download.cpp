#include "sdo/frames.hpp"
#include "sdo/sdoServer.hpp"

using namespace CANopen;
using namespace CANopen::sdo;

void SDO::onInitiateDownload(const Frame &frame, uint32_t now_us) {
    const InitiateDownloadRequest request(frame);
    const SDOAbortCodes code =
        openObject(request.index(), request.subindex(), true);
    if (code != SDOAbortCode_OK) {
        abort(request.index(), request.subindex(), code, now_us);
        return;
    }
    if (request.expedited()) {
        uint32_t length = request.expeditedBytes();
        if (!transfer.domain && !request.sizeIndicated())
            length = transfer.size < length ? transfer.size : length;
        if (transfer.domain) {
            const SDOAbortCodes domainCode = beginDomainDownload(length);
            if (domainCode != SDOAbortCode_OK) {
                abort(domainCode, now_us);
                return;
            }
        }
        const SDOAbortCodes writeCode =
            writeChunk(0, request.payload(4), length);
        if (writeCode != SDOAbortCode_OK) {
            abort(writeCode, now_us);
            return;
        }
        transfer.offset = length;
        finishDownload(Continuation::DownloadExpeditedDone, now_us);
        return;
    }
    const uint32_t announced = request.announcedSize();
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
    send(downloadInitiateFrame(nodeId, accessor.index(), accessor.subindex()),
         now_us);
    state = State::DownloadSegmented;
}

void SDO::sendDownloadInitiate(uint32_t now_us) {
    send(downloadInitiateFrame(nodeId, accessor.index(), accessor.subindex()),
         now_us);
    reset();
}

void SDO::onDownloadSegment(const Frame &frame, uint32_t now_us) {
    const SegmentRequest request(frame);
    if (request.toggle() != transfer.toggle) {
        abort(SDOAbortCode_ToggleBitNotAlternated, now_us);
        return;
    }
    const uint8_t length = request.dataBytes();
    const SDOAbortCodes code =
        writeChunk(transfer.offset, request.data(), length);
    if (code != SDOAbortCode_OK) {
        abort(code, now_us);
        return;
    }
    transfer.offset += length;
    if (request.last()) {
        finishDownload(Continuation::DownloadSegmentedDone, now_us);
        return;
    }
    send(downloadSegmentFrame(nodeId, transfer.toggle), now_us);
    transfer.toggle = !transfer.toggle;
}

void SDO::sendDownloadSegmentResponse(uint32_t now_us) {
    send(downloadSegmentFrame(nodeId, transfer.toggle), now_us);
    reset();
}
