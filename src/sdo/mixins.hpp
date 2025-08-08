#pragma once

#include <cstdint>
#include <cstring>

#include "endian_utils.hpp"
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
    void setIndexSubIndex(uint16_t index, uint8_t subindex) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        write_le16(d + 1, index);
        d[3] = subindex;
    }

    void setIndex(uint16_t index) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        write_le16(d + 1, index);
    }

    void setSubindex(uint8_t subindex) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[3] = subindex;
    }

    uint16_t index() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return read_le16(d + 1);
    }
    uint8_t subindex() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return d[3];
    }
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
struct CCSMixin : private ClaimBit<0>,
                  private ClaimBit<1>,
                  private ClaimBit<2>,
                  private SdoBitGuard<Derived, 0, 1, 2> {
    void set_ccs(CCS ccs) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = static_cast<uint8_t>((d[0] & ~uint8_t(0xE0)) |
                                    ((static_cast<uint8_t>(ccs) & 0x07) << 5));
    }
    CCS ccs() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return static_cast<CCS>((d[0] >> 5) & 0x07);
    }
};

template <class Derived>
struct SCSMixin : private ClaimBit<0>,
                  private ClaimBit<1>,
                  private ClaimBit<2>,
                  private SdoBitGuard<Derived, 0, 1, 2> {
    void set_scs(SCS scs) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = static_cast<uint8_t>((d[0] & ~uint8_t(0xE0)) |
                                    ((static_cast<uint8_t>(scs) & 0x07) << 5));
    }
    SCS scs() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return static_cast<SCS>((d[0] >> 5) & 0x07);
    }
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
struct ExpeditedTransferMixin : private ClaimBit<6>,
                                private SdoBitGuard<Derived, 6> {
    void set_expedited_transfer(bool expedited) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x40) | (expedited ? 0x40 : 0x00);
    }
    bool is_expedited_transfer() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x40) != 0;
    }
};

/**
 * Mixin for SDO frames that have a specified size bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | | | |s|               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *                ^
 */
template <class Derived>
struct SizeSpecifiedMixin : private ClaimBit<7>,
                            private SdoBitGuard<Derived, 7> {
    void set_size_specified(bool specified) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x80) | (specified ? 0x80 : 0x00);
    }
    bool is_size_specified() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x80) != 0;
    }
};

/**
 * Mixin for SDO frames that have a specified size bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | | | |c|               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *                ^
 */
template <class Derived>
struct FinalTransferMixin : private ClaimBit<7>,
                            private SdoBitGuard<Derived, 7> {
    void set_final_transfer(bool final_transfer) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x80) | (final_transfer ? 0x80 : 0x00);
    }
    bool is_final_transfer() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x80) != 0;
    }
};

/**
 * Mixin for SDO frames that have a toggle bit.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | |t| | | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *        ^
 */
template <class Derived>
struct ToggleBitMixin : private ClaimBit<3>, private SdoBitGuard<Derived, 3> {
    void set_toggle_bit(bool toggle) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x10) | (toggle ? 0x10 : 0x00);
    }
    bool is_toggle_bit() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x10) != 0;
    }
};

/**
 * Mixin for SDO frames that have a 2-bit unused bytes field.
 *
 *  0 1 2 3 4 5 6 7
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | n | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *          ^^^
 */
template <class Derived>
struct UnusedBytes2Mixin : private ClaimBit<4>,
                           private ClaimBit<5>,
                           private SdoBitGuard<Derived, 4, 5> {
    void set_unused_bytes(uint8_t value) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x30) | ((value & 0x03) << 4);
    }
    uint8_t unused_bytes() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x30) >> 4;
    }
};

/**
 * Mixin for SDO frames that have a 2-bit unused bytes field.
 *
 *  0 1 2 3 4 5 6 7
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       |
 * | | | | | n | | |               |               |               |
 * +---------------+---------------+---------------+---------------+---
 *          ^^^
 */
template <class Derived>
struct UnusedBytes3Mixin : private ClaimBit<4>,
                           private ClaimBit<5>,
                           private SdoBitGuard<Derived, 4, 5> {
    void set_unused_bytes(uint8_t value) {
        uint8_t* d = static_cast<Derived*>(this)->bytes();
        d[0] = (d[0] & ~0x70) | ((value & 0x03) << 4);
    }
    uint8_t unused_bytes() const {
        const uint8_t* d = static_cast<const Derived*>(this)->bytes();
        return (d[0] & 0x70) >> 4;
    }
};

template <class Derived>
struct FrameSegmentMixin : public IndexSubindexMixin<Derived>,
                           public FinalTransferMixin<Derived>,
                           public ToggleBitMixin<Derived>,
                           public UnusedBytes3Mixin<Derived> {
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

    void getData(uint8_t* dataPtr) { memcpy(dataPtr, &bytes()[1], getSize()); }
};
