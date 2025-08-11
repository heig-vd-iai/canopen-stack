/**
 * Frame mixins for SDO (Service Data Objects) in CANopen.
 *
 * Disclaimer: implementing these mixins would have been easier with
 * bitfields and nested structures, but bitfields are not portable.
 *
 * With the desire to be compatible with C++14, concepts cannot be used.
 *
 */
#pragma once

#include <cstdint>
#include <cstring>

#include "bitsUtils.hpp"
#include "endianUtils.hpp"
#include "enums.hpp"
#include "frameBase.hpp"

/**
 * Mixing guards.
 */
template <int N>
struct ClaimBit {
    template <int M, typename std::enable_if<M == N, int>::type = 0>
    using __bit = void;
};

template <class D, int... Bits>
struct SdoBitGuard {
    static constexpr int __enforce[] = {
        (sizeof(typename D::template __bit<Bits>), 0)...};
};

/**
 * Underlying type for CCS and SCS enums.
 */
template <class E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

/**
 * Mixin for SDO frames that have an index and subindex.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * |    command    |           index               |   subindex    |
 * +---------------+-------------------------------+---------------+---
 *                  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 */
template <class Derived>
struct IndexSubindexMixin {
    using Derived::bytes;

    void setIndexSubIndex(uint16_t index, uint8_t subindex) {
        endian::write_le16(bytes() + 1, index);
        bytes()[3] = subindex;
    }

    void setIndex(uint16_t index) { endian::write_le16(bytes() + 1, index); }
    void setSubindex(uint8_t subindex) { bytes()[3] = subindex; }

    uint16_t index() const { return endian::read_le16(bytes() + 1); }
    uint8_t subindex() const { return bytes()[3]; }
};

/**
 * Mixin for SDO frames that have a command specifier (CCS/SCS).
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | ccs | | | | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *  ^^^^^
 */
template <class Derived>
struct CSMixin : private ClaimBit<7>,
                  private ClaimBit<6>,
                  private ClaimBit<5>,
                  private SdoBitGuard<Derived, 5, 6, 7> {
    using Derived::bytes;

    void setCS(CS cs) {
        bytes()[0] = (bytes()[0] & ~uint8_t(0xE0)) |
                     ((to_underlying(cs) & 0x07) << 5);
    }
    unsigned cs() const { return (bytes()[0] >> 5) & 0x07; }
};

/**
 * Mixin for SDO frames that have a expedited transfer bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | | |e| |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *              ^
 */
template <class Derived>
struct ExpeditedTransferMixin : private ClaimBit<1>,
                                private SdoBitGuard<Derived, 1> {
    using Derived::bytes;

    void setExpeditedTransfer(bool exp) { setBit(bytes()[0], 1, exp); }
    bool isExpeditedTransfer() const { return getBit(bytes()[0], 1); }
};

/**
 * Mixin for SDO frames that have a specified size bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | | | |s|               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *                ^ uploads/downloads
 *              ^ block uploads/downloads
 *
 * CiA 301 is not consistent about the size specified bit...
 * In SDO uploads/downloads, bit 0 is used,
 * while in block uploads/downloads, bit 1 is used.
 */
template <class Derived, int Bit = 0>
struct SizeSpecifiedMixin : private ClaimBit<Bit>,
                            private SdoBitGuard<Derived, Bit> {
    using Derived::bytes;

    void setSizeSpecified(bool value) { setBit(bytes()[0], Bit, value); }
    bool isSizeSpecified() const { return getBit(bytes()[0], Bit); }
};

/**
 * Mixin for SDO frames that have a specified size bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * |c| | | | | | |c|               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *                ^ For Upload/Download CiA 301:2011 §7.2.4.3.4 p.50
 *  ^ For Block Upload/Download CiA 301:2011 §7.2.4.3.10 p.55
 */
template <class Derived, int Bit = 0>
struct FinalTransferMixin : private ClaimBit<Bit>,
                            private SdoBitGuard<Derived, Bit> {
    using Derived::bytes;

    void setFinalTransfer(bool final) { setBit(bytes()[0], Bit, final); }
    bool finalTransfer() const { return getBit(bytes()[0], Bit); }
};

/**
 * Mixin for SDO frames that have a toggle bit.
 * CiA 301:2011 §7.2.4.3.4 p.50
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | |t| | | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *        ^
 *
 * t: toggle bit. This bit shall alternate for each subsequent segment
 * that is downloaded. The first segment shall have the toggle-bit set to 0.
 * The toggle bit shall be equal for the request and the response message.
 */
template <class Derived, int Bit = 4>
struct ToggleBitMixin : private ClaimBit<Bit>,
                        private SdoBitGuard<Derived, Bit> {
    using Derived::bytes;

    void setToggleBit(bool toggle) { setBit(bytes()[0], Bit, toggle); }
    bool toggleBit() const { return getBit(bytes()[0], Bit); }
};

/**
 * Mixin for SDO frames that have a 2-bit unused bytes field.
 * CiA 301:2011 §7.2.4.3.6 p.51
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | n | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *          ^^^
 *
 * Only valid if e = 1 and s = 1, otherwise 0. If valid it indicates
 * the number of bytes in d that do not contain data.
 * Bytes [8-n, 7] do not contain segment data.
 */
template <class Derived>
struct UnusedBytes2Mixin : private ClaimBit<2>,
                           private ClaimBit<3>,
                           private SdoBitGuard<Derived, 2, 3> {
    using Derived::bytes;

    void setUnusedBytes(uint8_t value) {
        bits::set_field8(bytes()[0], 2u, 2u, value);  // implicit mask
    }

    uint8_t unusedBytes() const { return bits::get_field8(bytes()[0], 2u, 2u); }
};

/**
 * Mixin for SDO frames that have a 3-bit unused bytes field.
 * CiA 301:2011 §7.2.4.3.4 p.50
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | |  n  | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *          ^^^^^
 */
template <class Derived, unsigned start=1>
struct UnusedBytes3Mixin : private ClaimBit<start>,
                           private ClaimBit<start+1>,
                           private ClaimBit<start+2>,
                           private SdoBitGuard<Derived, start, start+1, start+2> {
    using Derived::bytes;

    void setUnusedBytes(uint8_t value) {
        bits::set_field8(bytes()[0], 1u, 3u, value);
    }

    uint8_t unusedBytes() const { return bits::get_field8(bytes()[0], 1u, 3u); }
};

/**
 * Mixin for SDO frames that have a client crc bit.
 * CiA 301:2011 §7.2.4.3.9 p.53
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | |C| | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *            ^
 * cc: Client CRC support bit
 *  0 : no CRC support, 1: CRC support
 * sc: Server CRC support bit
 *  0 : no CRC support, 1: CRC support
 */
template <class Derived, int Bit = 2>
struct CRCBitMixin : private ClaimBit<Bit>, private SdoBitGuard<Derived, Bit> {
    using Derived::bytes;

    void setCRCSupport(bool value) { setBit(bytes()[0], Bit, value); }
    bool isCRCSupported() const { return getBit(bytes()[0], Bit); }
};

template <class Derived>
struct InitiateDataMixin : public SDOFrame,
                           public IndexSubindexMixin<Derived>,
                           public CSMixin<Derived>,
                           public ExpeditedTransferMixin<Derived>,
                           public SizeSpecifiedMixin<Derived>,
                           public UnusedBytesMixin<Derived, 2> {
    unsigned setSize(uint32_t size) {
        auto available = 4;
        bool overflow = (size > available);
        auto payloadSize = overflow ? 0 : size;
        set_unused_bytes(available - payloadSize);
        set_final_transfer(!overflow);
        return payloadSize;
    }

    unsigned getSize() const {
        auto available = 4;
        return available - unused_bytes();
    }

    unsigned setData(uint8_t* dataPtr, uint8_t size) {
        auto payloadSize = setSize(size);
        memcpy(&bytes()[1], dataPtr, payloadSize);
        return payloadSize;
    }

    void getData(uint8_t* dataPtr) { memcpy(dataPtr, &bytes()[1], getSize()); }
};

/**
 * Mixin for SDO frames that have a command specifier (CCS/SCS).
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | |   seqno     |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *    ^^^^^^^^^^^^^
 */
template <class Derived>
struct SeqNoSMixin : private ClaimBit<6>,
                  private ClaimBit<5>,
                  private ClaimBit<4>,
                  private ClaimBit<3>,
                  private ClaimBit<2>,
                  private ClaimBit<1>,
                  private ClaimBit<0>,
                  private SdoBitGuard<Derived, 0, 1, 2, 3, 4, 5, 6> {
    using Derived::bytes;

    void setSequenceNumber(uint8_t seqNo) {
        set_field8(bytes()[0], 0u, 7u, seqNo);
    }
    unsigned sequenceNumber() const { return get_field8(bytes()[0], 0u, 7u); }
};

/**
 * Mixin for SDO frames that have a frame segment.
 * CiA 301:2011 §7.2.4.3.4 p.50
 */
template <class Derived>
struct FrameSegmentMixin : public FinalTransferMixin<Derived>,
                           public ToggleBitMixin<Derived>,
                           public UnusedBytes3Mixin<Derived> {
    using Derived::bytes;

    /**
     * Set payload size and data for the frame segment.
     *
     * Frame segment have 7 bytes of data.
     * Instead of manually setting the size and the final transfer bit,
     * this mixin provides a convenient way to manage the frame segment.
     */
    unsigned setSize(uint32_t size) {
        auto available = 7;
        bool overflow = (size > available);
        auto payloadSize = overflow ? 0 : size;
        set_unused_bytes(available - payloadSize);
        set_final_transfer(!overflow);
        return payloadSize;
    }

    unsigned getSize() const {
        auto available = 7;
        return available - unused_bytes();
    }

    unsigned setData(uint8_t* dataPtr, uint8_t size) {
        auto payloadSize = setSize(size);
        memcpy(&bytes()[1], dataPtr, payloadSize);
        return payloadSize;
    }

    void getData(uint8_t* dataPtr) const {
        memcpy(dataPtr, &bytes()[1], getSize());
    }
};

/**
 * Mixin for SDO Block: Transfer size
 * CiA 301:2011 §7.2.4.3.13 p.58
 */
template <class Derived>
struct TransferSizeMixin : public SizeSpecifiedMixin<Derived, 1> {
    using Derived::bytes;

    void setSize(uint32_t size) {
        endian::write_le32(bytes() + 4, size);
        setSizeSpecified(size > 0);
    }
    uint32_t size() const { return endian::read_le32(bytes() + 4); }
};

/**
 * Mixin for SDO Block: Block size.
 * CiA 301:2011 §7.2.4.3.13 p.58
 */
template <class Derived, unsigned Byte = 4>
struct BlockSizeMixin {
    using Derived::bytes;

    void setBlockSize(uint8_t blksize) {
        endian::write_le8(bytes() + Byte, blksize);
    }
    uint8_t blockSize() const { return endian::read_le8(bytes() + Byte); }
};

/**
 * Mixin for SDO Block: Acqseq.
 * CiA 301:2011 §7.2.4.3.10 p.55
 */
template <class Derived, unsigned Byte = 1>
struct AcqSeqMixin {
    using Derived::bytes;

    void setAcknowledgeSequence(uint8_t blksize) {
        endian::write_le8(bytes() + Byte, blksize);
    }
    uint8_t sequenceNumber() const { return endian::read_le8(bytes() + Byte); }
};

/**
 * Mixin for SDO Block: Pst
 * CiA 301:2011 §7.2.4.3.13 p.58
 */
template <class Derived, unsigned Byte = 5>
struct PstMixin {
    using Derived::bytes;

    void setPst(uint8_t pst) { endian::write_le8(bytes() + Byte, pst); }
    uint8_t pst() const { return endian::read_le8(bytes() + Byte); }
};