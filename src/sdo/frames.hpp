/**
 * Typed views over the 8 data bytes of an SDO frame.
 * Requests wrap a received Frame, responses build a Frame to send.
 * Layouts follow CiA301:2011 §7.2.4.3.
 */
#pragma once

#include <cstdint>
#include <cstring>

#include "enums.hpp"
#include "frame.hpp"
#include "sdo/config.hpp"
#include "utils/bits.hpp"
#include "utils/endian.hpp"

namespace CANopen {
namespace sdo {

enum ClientCommand : uint8_t {
    CCS_DOWNLOAD_SEGMENT = 0,
    CCS_INITIATE_DOWNLOAD = 1,
    CCS_INITIATE_UPLOAD = 2,
    CCS_UPLOAD_SEGMENT = 3,
    CCS_ABORT = 4,
    CCS_BLOCK_UPLOAD = 5,
    CCS_BLOCK_DOWNLOAD = 6,
    CCS_COUNT = 8
};

enum ServerCommand : uint8_t {
    SCS_UPLOAD_SEGMENT = 0,
    SCS_DOWNLOAD_SEGMENT = 1,
    SCS_INITIATE_UPLOAD = 2,
    SCS_INITIATE_DOWNLOAD = 3,
    SCS_ABORT = 4,
    SCS_BLOCK_DOWNLOAD = 5,
    SCS_BLOCK_UPLOAD = 6
};

enum BlockClientSubcommand : uint8_t {
    BLOCK_CS_INITIATE = 0,
    BLOCK_CS_END = 1,
    BLOCK_CS_UPLOAD_RESPONSE = 2,
    BLOCK_CS_UPLOAD_START = 3
};

enum BlockServerSubcommand : uint8_t {
    BLOCK_SS_INITIATE = 0,
    BLOCK_SS_END = 1,
    BLOCK_SS_DOWNLOAD_RESPONSE = 2
};

/**
 * Byte 0: ccs (bits 7..5), then fields that depend on the frame kind.
 * Bytes 1..2: index, byte 3: subindex, bytes 4..7: d.
 */
class Request {
   public:
    explicit Request(const Frame &frame) : frame(frame) {}

    uint8_t byte0() const { return frame.data[0]; }
    uint8_t command() const { return bits::field(byte0(), 5, 3); }
    uint16_t index() const { return endian::readLe16(frame.data + 1); }
    uint8_t subindex() const { return frame.data[3]; }
    uint32_t data32() const { return endian::readLe32(frame.data + 4); }
    const uint8_t *payload(unsigned offset) const {
        return frame.data + offset;
    }

   protected:
    const Frame &frame;
};

/** §7.2.4.3.3: e (bit 1), s (bit 0), n (bits 3..2), d = data or size. */
class InitiateDownloadRequest : public Request {
   public:
    using Request::Request;
    bool expedited() const { return bits::get(byte0(), 1); }
    bool sizeIndicated() const { return bits::get(byte0(), 0); }
    uint8_t expeditedBytes() const {
        if (!sizeIndicated()) return EXPEDITED_MAX_BYTES;
        return static_cast<uint8_t>(EXPEDITED_MAX_BYTES -
                                    bits::field(byte0(), 2, 2));
    }
    uint32_t announcedSize() const { return sizeIndicated() ? data32() : 0; }
};

/** §7.2.4.3.4: t (bit 4), n (bits 3..1), c (bit 0), 7 data bytes. */
class SegmentRequest : public Request {
   public:
    using Request::Request;
    bool toggle() const { return bits::get(byte0(), 4); }
    bool last() const { return bits::get(byte0(), 0); }
    uint8_t dataBytes() const {
        return static_cast<uint8_t>(SEGMENT_MAX_BYTES -
                                    bits::field(byte0(), 1, 3));
    }
    const uint8_t *data() const { return payload(1); }
};

/** §7.2.4.3.8: cc (bit 2), s (bit 1), cs (bit 0), d = size. */
class BlockDownloadInitiateRequest : public Request {
   public:
    using Request::Request;
    bool crcSupported() const { return bits::get(byte0(), 2); }
    bool sizeIndicated() const { return bits::get(byte0(), 1); }
    uint8_t subcommand() const { return bits::field(byte0(), 0, 1); }
};

/** §7.2.4.3.9: c (bit 7), seqno (bits 6..0), 7 data bytes. */
class BlockSegment : public Request {
   public:
    using Request::Request;
    bool last() const { return bits::get(byte0(), 7); }
    uint8_t seqno() const { return bits::field(byte0(), 0, 7); }
    const uint8_t *data() const { return payload(1); }
};

/** §7.2.4.3.10: n (bits 4..2), cs (bit 0) = 1, crc in bytes 1..2. */
class BlockDownloadEndRequest : public Request {
   public:
    using Request::Request;
    uint8_t unusedBytes() const { return bits::field(byte0(), 2, 3); }
    uint16_t crc() const { return endian::readLe16(frame.data + 1); }
};

/** §7.2.4.3.12/14/16: cc (bit 2), cs (bits 1..0), blksize, pst, ackseq. */
class BlockUploadRequest : public Request {
   public:
    using Request::Request;
    uint8_t subcommand() const { return bits::field(byte0(), 0, 2); }
    bool crcSupported() const { return bits::get(byte0(), 2); }
    uint8_t blockSize() const { return frame.data[4]; }
    uint8_t protocolSwitchThreshold() const { return frame.data[5]; }
    uint8_t ackseq() const { return frame.data[1]; }
    uint8_t nextBlockSize() const { return frame.data[2]; }
};

/**
 * Builds an outgoing SDO frame on the TSDO COB-ID of the server.
 */
class Response {
   public:
    explicit Response(uint8_t nodeId) : frame(nodeId, FunctionCode_TSDO) {
        frame.dlc = CAN_DATA_LENGTH;
    }

    const Frame &get() const { return frame; }

    Response &command(ServerCommand scs) {
        return field(5, 3, static_cast<uint8_t>(scs));
    }
    Response &bit(unsigned position, bool value) {
        frame.data[0] = bits::set(frame.data[0], position, value);
        return *this;
    }
    Response &field(unsigned start, unsigned width, uint8_t value) {
        frame.data[0] = bits::setField(frame.data[0], start, width, value);
        return *this;
    }
    Response &multiplexer(uint16_t index, uint8_t subindex) {
        endian::writeLe16(frame.data + 1, index);
        frame.data[3] = subindex;
        return *this;
    }
    Response &byte(unsigned offset, uint8_t value) {
        frame.data[offset] = value;
        return *this;
    }
    Response &data32(uint32_t value) {
        endian::writeLe32(frame.data + 4, value);
        return *this;
    }
    Response &bytes(unsigned offset, const uint8_t *source, uint32_t length) {
        memcpy(frame.data + offset, source, length);
        return *this;
    }

   private:
    Frame frame;
};

inline Frame abortFrame(uint8_t nodeId, uint16_t index, uint8_t subindex,
                        SDOAbortCodes code) {
    return Response(nodeId)
        .command(SCS_ABORT)
        .multiplexer(index, subindex)
        .data32(static_cast<uint32_t>(code))
        .get();
}

inline Frame uploadExpeditedFrame(uint8_t nodeId, uint16_t index,
                                  uint8_t subindex, const uint8_t *data,
                                  uint8_t length) {
    return Response(nodeId)
        .command(SCS_INITIATE_UPLOAD)
        .bit(1, true)
        .bit(0, true)
        .field(2, 2, static_cast<uint8_t>(EXPEDITED_MAX_BYTES - length))
        .multiplexer(index, subindex)
        .bytes(4, data, length)
        .get();
}

inline Frame uploadSegmentedFrame(uint8_t nodeId, uint16_t index,
                                  uint8_t subindex, uint32_t size) {
    return Response(nodeId)
        .command(SCS_INITIATE_UPLOAD)
        .bit(0, true)
        .multiplexer(index, subindex)
        .data32(size)
        .get();
}

inline Frame uploadSegmentFrame(uint8_t nodeId, bool toggle, bool last,
                                const uint8_t *data, uint8_t length) {
    return Response(nodeId)
        .command(SCS_UPLOAD_SEGMENT)
        .bit(4, toggle)
        .field(1, 3, static_cast<uint8_t>(SEGMENT_MAX_BYTES - length))
        .bit(0, last)
        .bytes(1, data, length)
        .get();
}

inline Frame downloadInitiateFrame(uint8_t nodeId, uint16_t index,
                                   uint8_t subindex) {
    return Response(nodeId)
        .command(SCS_INITIATE_DOWNLOAD)
        .multiplexer(index, subindex)
        .get();
}

inline Frame downloadSegmentFrame(uint8_t nodeId, bool toggle) {
    return Response(nodeId).command(SCS_DOWNLOAD_SEGMENT).bit(4, toggle).get();
}

inline Frame blockDownloadInitiateFrame(uint8_t nodeId, uint16_t index,
                                        uint8_t subindex, bool crc,
                                        uint8_t blockSize) {
    return Response(nodeId)
        .command(SCS_BLOCK_DOWNLOAD)
        .bit(2, crc)
        .field(0, 2, BLOCK_SS_INITIATE)
        .multiplexer(index, subindex)
        .byte(4, blockSize)
        .get();
}

inline Frame blockDownloadAckFrame(uint8_t nodeId, uint8_t ackseq,
                                   uint8_t blockSize) {
    return Response(nodeId)
        .command(SCS_BLOCK_DOWNLOAD)
        .field(0, 2, BLOCK_SS_DOWNLOAD_RESPONSE)
        .byte(1, ackseq)
        .byte(2, blockSize)
        .get();
}

inline Frame blockDownloadEndFrame(uint8_t nodeId) {
    return Response(nodeId)
        .command(SCS_BLOCK_DOWNLOAD)
        .field(0, 2, BLOCK_SS_END)
        .get();
}

inline Frame blockUploadInitiateFrame(uint8_t nodeId, uint16_t index,
                                      uint8_t subindex, bool crc,
                                      uint32_t size) {
    return Response(nodeId)
        .command(SCS_BLOCK_UPLOAD)
        .bit(2, crc)
        .bit(1, true)
        .bit(0, false)
        .multiplexer(index, subindex)
        .data32(size)
        .get();
}

inline Frame blockUploadSegmentFrame(uint8_t nodeId, uint8_t seqno, bool last,
                                     const uint8_t *data, uint8_t length) {
    return Response(nodeId)
        .bit(7, last)
        .field(0, 7, seqno)
        .bytes(1, data, length)
        .get();
}

inline Frame blockUploadEndFrame(uint8_t nodeId, uint8_t unusedBytes,
                                 uint16_t crc) {
    Response response(nodeId);
    response.command(SCS_BLOCK_UPLOAD)
        .field(2, 3, unusedBytes)
        .field(0, 2, BLOCK_SS_END);
    uint8_t crcBytes[2];
    endian::writeLe16(crcBytes, crc);
    return response.bytes(1, crcBytes, 2).get();
}

}  // namespace sdo
}  // namespace CANopen
