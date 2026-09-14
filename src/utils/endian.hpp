#pragma once

#include <cstdint>

namespace CANopen {
namespace endian {

inline uint16_t readLe16(const uint8_t *p) {
    return static_cast<uint16_t>(static_cast<uint16_t>(p[0]) |
                                 static_cast<uint16_t>(p[1] << 8));
}

inline uint32_t readLe32(const uint8_t *p) {
    return static_cast<uint32_t>(p[0]) | (static_cast<uint32_t>(p[1]) << 8) |
           (static_cast<uint32_t>(p[2]) << 16) |
           (static_cast<uint32_t>(p[3]) << 24);
}

inline void writeLe16(uint8_t *p, uint16_t value) {
    p[0] = static_cast<uint8_t>(value & 0xFFu);
    p[1] = static_cast<uint8_t>((value >> 8) & 0xFFu);
}

inline void writeLe32(uint8_t *p, uint32_t value) {
    p[0] = static_cast<uint8_t>(value & 0xFFu);
    p[1] = static_cast<uint8_t>((value >> 8) & 0xFFu);
    p[2] = static_cast<uint8_t>((value >> 16) & 0xFFu);
    p[3] = static_cast<uint8_t>((value >> 24) & 0xFFu);
}

}  // namespace endian
}  // namespace CANopen
