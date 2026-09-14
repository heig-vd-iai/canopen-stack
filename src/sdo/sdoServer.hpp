/**
 * SDO server: expedited, segmented and block transfers in both directions.
 * See CiA301:2011 §7.2.4.
 *
 * Remote objects answer asynchronously: the server parks in AwaitOD and
 * resumes from update() once the object dictionary access completes.
 * DOMAIN objects are streamed through a DomainHandler, never buffered whole.
 */
#pragma once

#include <cstdint>

#include "frame.hpp"
#include "hal/can-transport.hpp"
#include "od/odAccessor.hpp"
#include "sdo/config.hpp"
#include "utils/crc.hpp"

namespace CANopen {

class SDO {
   public:
    SDO(ODAccessor &accessor, CanTransport &transport, uint8_t nodeId);

    void enable() { enabled = true; }
    void disable();
    bool isIdle() const { return state == State::Idle; }

    void receiveFrame(const Frame &frame, uint32_t now_us);
    void update(uint32_t now_us);

   private:
    enum class State {
        Idle,
        AwaitOD,
        UploadSegmented,
        DownloadSegmented,
        BlockUploadReady,
        BlockUploadSending,
        BlockUploadAwaitAck,
        BlockUploadEnding,
        BlockDownloading,
        BlockDownloadEnding
    };

    enum class Continuation {
        None,
        UploadInitiate,
        DownloadExpeditedDone,
        DownloadSegmentedDone,
        BlockUploadInitiate,
        BlockDownloadDone
    };

    typedef void (SDO::*Handler)(const Frame &frame, uint32_t now_us);

    struct Route {
        State state;
        uint8_t command;
        Handler handler;
    };

    static constexpr uint8_t ANY_COMMAND = 0xFF;
    static const Route routes[];

    void onInitiateUpload(const Frame &frame, uint32_t now_us);
    void onUploadSegment(const Frame &frame, uint32_t now_us);
    void onInitiateDownload(const Frame &frame, uint32_t now_us);
    void onDownloadSegment(const Frame &frame, uint32_t now_us);
    void onBlockDownloadInitiate(const Frame &frame, uint32_t now_us);
    void onBlockDownloadSegment(const Frame &frame, uint32_t now_us);
    void onBlockDownloadEnd(const Frame &frame, uint32_t now_us);
    void onBlockUploadInitiate(const Frame &frame, uint32_t now_us);
    void onBlockUploadStart(const Frame &frame, uint32_t now_us);
    void onBlockUploadAck(const Frame &frame, uint32_t now_us);
    void onBlockUploadEnd(const Frame &frame, uint32_t now_us);
    void onClientAbort(const Frame &frame, uint32_t now_us);
    void onUnexpected(const Frame &frame, uint32_t now_us);

    SDOAbortCodes openObject(uint16_t index, uint8_t subindex, bool forWrite);
    void beginTransfer();
    uint32_t uploadSize();
    SDOAbortCodes readChunk(uint32_t offset, uint8_t *destination,
                            uint32_t length);
    SDOAbortCodes writeChunk(uint32_t offset, const uint8_t *source,
                             uint32_t length);
    SDOAbortCodes beginDomainDownload(uint32_t announcedSize);
    void finishDownload(Continuation continuation, uint32_t now_us);

    void awaitOD(bool write, Continuation continuation, uint32_t now_us);
    void resume(uint32_t now_us);
    void sendUploadInitiate(uint32_t now_us);
    void sendDownloadInitiate(uint32_t now_us);
    void sendDownloadSegmentResponse(uint32_t now_us);
    void sendBlockUploadInitiate(uint32_t now_us);
    void sendBlockUploadSegment(uint32_t now_us);
    void sendBlockUploadEnd(uint32_t now_us);
    void sendBlockDownloadEnd(uint32_t now_us);
    void deliverBlock(uint32_t now_us);
    uint16_t uploadCrc();

    void send(const Frame &frame, uint32_t now_us);
    void abort(SDOAbortCodes code, uint32_t now_us);
    void abort(uint16_t index, uint8_t subindex, SDOAbortCodes code,
               uint32_t now_us);
    void reset();

    ODAccessor &accessor;
    CanTransport &transport;
    const uint8_t nodeId;
    bool enabled = false;
    State state = State::Idle;
    Continuation continuation = Continuation::None;
    uint32_t lastActivity_us = 0;
    uint32_t blockDeadline_us = 0;
    bool blockTimeoutAcked = false;

    struct Transfer {
        bool domain = false;
        bool domainOpen = false;
        bool sizeKnown = false;
        uint32_t size = 0;
        uint32_t offset = 0;
        bool toggle = false;
        bool crcEnabled = false;
        uint8_t blockSize = 0;
        uint8_t seqno = 0;
        uint8_t ackseq = 0;
        bool gap = false;
        bool lastSeen = false;
        uint32_t blockStart = 0;
        Crc16Ccitt crc;
        uint8_t buffer[sdo::BLOCK_BUFFER_SIZE];
    } transfer;
};

}  // namespace CANopen
