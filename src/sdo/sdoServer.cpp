#include "sdo/sdoServer.hpp"

#include <cstring>

#include "sdo/frames.hpp"

using namespace CANopen;
using namespace CANopen::sdo;

const SDO::Route SDO::routes[] = {
    {State::Idle, CCS_INITIATE_UPLOAD, &SDO::onInitiateUpload},
    {State::Idle, CCS_INITIATE_DOWNLOAD, &SDO::onInitiateDownload},
    {State::Idle, CCS_BLOCK_UPLOAD, &SDO::onBlockUploadInitiate},
    {State::Idle, CCS_BLOCK_DOWNLOAD, &SDO::onBlockDownloadInitiate},
    {State::Idle, CCS_ABORT, &SDO::onClientAbort},
    {State::UploadSegmented, CCS_UPLOAD_SEGMENT, &SDO::onUploadSegment},
    {State::DownloadSegmented, CCS_DOWNLOAD_SEGMENT, &SDO::onDownloadSegment},
    {State::BlockUploadReady, CCS_BLOCK_UPLOAD, &SDO::onBlockUploadStart},
    {State::BlockUploadAwaitAck, CCS_BLOCK_UPLOAD, &SDO::onBlockUploadAck},
    {State::BlockUploadEnding, CCS_BLOCK_UPLOAD, &SDO::onBlockUploadEnd},
    {State::BlockDownloading, ANY_COMMAND, &SDO::onBlockDownloadSegment},
    {State::BlockDownloadEnding, CCS_BLOCK_DOWNLOAD, &SDO::onBlockDownloadEnd},
    {State::Idle, ANY_COMMAND, &SDO::onUnexpected},
};

SDO::SDO(ODAccessor &accessor, CanTransport &transport, uint8_t nodeId)
    : accessor(accessor), transport(transport), nodeId(nodeId) {}

void SDO::disable() {
    enabled = false;
    reset();
}

void SDO::receiveFrame(const Frame &frame, uint32_t now_us) {
    if (!enabled || frame.nodeId != nodeId) return;
    const uint8_t command = Request(frame).command();
    if (command == CCS_ABORT && state != State::BlockDownloading) {
        onClientAbort(frame, now_us);
        return;
    }
    for (const Route &route : routes) {
        const bool stateMatches = route.state == state;
        const bool commandMatches =
            route.command == ANY_COMMAND || route.command == command;
        if (stateMatches && commandMatches) {
            (this->*route.handler)(frame, now_us);
            return;
        }
    }
    onUnexpected(frame, now_us);
}

void SDO::update(uint32_t now_us) {
    if (!enabled || state == State::Idle) return;
    if (now_us - lastActivity_us >= TIMEOUT_US) {
        abort(SDOAbortCode_TimedOut, now_us);
        return;
    }
    switch (state) {
        case State::AwaitOD: {
            const ODAccessor::Status status = accessor.poll(now_us);
            if (status == ODAccessor::Status::Done) resume(now_us);
            if (status == ODAccessor::Status::Failed)
                abort(accessor.error(), now_us);
            break;
        }
        case State::BlockUploadSending:
            sendBlockUploadSegment(now_us);
            break;
        case State::BlockDownloading:
            if (!blockTimeoutAcked &&
                static_cast<int32_t>(now_us - blockDeadline_us) >= 0) {
                blockTimeoutAcked = true;
                deliverBlock(now_us);
            }
            break;
        default:
            break;
    }
}

SDOAbortCodes SDO::openObject(uint16_t index, uint8_t subindex, bool forWrite) {
    const SDOAbortCodes code = accessor.lookup(index, subindex);
    if (code != SDOAbortCode_OK) return code;
    if (forWrite && !accessor.isWriteable())
        return SDOAbortCode_AttemptWriteOnReadOnly;
    if (!forWrite && !accessor.isReadable())
        return SDOAbortCode_AttemptReadOnWriteOnly;
    if (accessor.isDomain() && accessor.domain() == nullptr)
        return SDOAbortCode_UnsupportedObjectAccess;
    beginTransfer();
    return SDOAbortCode_OK;
}

void SDO::beginTransfer() {
    transfer.domain = accessor.isDomain();
    transfer.domainOpen = false;
    transfer.sizeKnown = !transfer.domain;
    transfer.size = accessor.size();
    transfer.offset = 0;
    transfer.toggle = false;
    transfer.crcEnabled = false;
    transfer.blockSize = 0;
    transfer.seqno = 0;
    transfer.ackseq = 0;
    transfer.gap = false;
    transfer.lastSeen = false;
    transfer.blockStart = 0;
    transfer.crc.reset();
    blockTimeoutAcked = false;
}

uint32_t SDO::uploadSize() {
    if (!transfer.domain) return accessor.size();
    return accessor.domain()->uploadSize(accessor.id());
}

SDOAbortCodes SDO::readChunk(uint32_t offset, uint8_t *destination,
                             uint32_t length) {
    if (transfer.domain)
        return accessor.domain()->uploadChunk(accessor.id(), offset,
                                              destination, length);
    memcpy(destination, accessor.bytes() + offset, length);
    return SDOAbortCode_OK;
}

SDOAbortCodes SDO::writeChunk(uint32_t offset, const uint8_t *source,
                              uint32_t length) {
    if (transfer.domain)
        return accessor.domain()->downloadChunk(accessor.id(), offset, source,
                                                length);
    if (offset + length > accessor.size())
        return SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh;
    memcpy(accessor.bytes() + offset, source, length);
    return SDOAbortCode_OK;
}

SDOAbortCodes SDO::beginDomainDownload(uint32_t announcedSize) {
    const SDOAbortCodes code =
        accessor.domain()->beginDownload(accessor.id(), announcedSize);
    transfer.domainOpen = code == SDOAbortCode_OK;
    return code;
}

void SDO::finishDownload(Continuation next, uint32_t now_us) {
    if (transfer.sizeKnown && transfer.offset != transfer.size) {
        abort(transfer.offset < transfer.size
                  ? SDOAbortCode_DataTypeMismatch_LengthParameterTooLow
                  : SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh,
              now_us);
        return;
    }
    if (transfer.domain) {
        transfer.domainOpen = false;
        const SDOAbortCodes code =
            accessor.domain()->endDownload(accessor.id(), transfer.offset);
        if (code != SDOAbortCode_OK) {
            abort(code, now_us);
            return;
        }
        continuation = next;
        resume(now_us);
        return;
    }
    awaitOD(true, next, now_us);
}

void SDO::awaitOD(bool write, Continuation next, uint32_t now_us) {
    continuation = next;
    const ODAccessor::Status status =
        write ? accessor.startWrite(now_us) : accessor.startRead(now_us);
    switch (status) {
        case ODAccessor::Status::Done:
            resume(now_us);
            break;
        case ODAccessor::Status::Pending:
            state = State::AwaitOD;
            lastActivity_us = now_us;
            break;
        default:
            abort(accessor.error(), now_us);
            break;
    }
}

void SDO::resume(uint32_t now_us) {
    const Continuation next = continuation;
    continuation = Continuation::None;
    switch (next) {
        case Continuation::UploadInitiate:
            sendUploadInitiate(now_us);
            break;
        case Continuation::DownloadExpeditedDone:
            sendDownloadInitiate(now_us);
            break;
        case Continuation::DownloadSegmentedDone:
            sendDownloadSegmentResponse(now_us);
            break;
        case Continuation::BlockUploadInitiate:
            sendBlockUploadInitiate(now_us);
            break;
        case Continuation::BlockDownloadDone:
            sendBlockDownloadEnd(now_us);
            break;
        default:
            reset();
            break;
    }
}

void SDO::send(const Frame &frame, uint32_t now_us) {
    lastActivity_us = now_us;
    transport.sendFrame(frame);
}

void SDO::abort(SDOAbortCodes code, uint32_t now_us) {
    abort(accessor.index(), accessor.subindex(), code, now_us);
}

void SDO::abort(uint16_t index, uint8_t subindex, SDOAbortCodes code,
                uint32_t now_us) {
    send(abortFrame(nodeId, index, subindex, code), now_us);
    reset();
}

void SDO::reset() {
    if (transfer.domainOpen) accessor.domain()->abortDownload(accessor.id());
    transfer.domainOpen = false;
    accessor.clear();
    continuation = Continuation::None;
    state = State::Idle;
}

void SDO::onClientAbort(const Frame &, uint32_t) { reset(); }

void SDO::onUnexpected(const Frame &frame, uint32_t now_us) {
    const Request request(frame);
    if (state == State::Idle) {
        abort(request.index(), request.subindex(),
              SDOAbortCode_CommandSpecifierInvalid, now_us);
        return;
    }
    abort(SDOAbortCode_CommandSpecifierInvalid, now_us);
}
