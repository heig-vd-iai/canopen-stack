#pragma once

#include <cstdint>

namespace CANopen {
namespace bits {

inline bool get(uint8_t byte, unsigned bit) {
    return ((byte >> bit) & 1u) != 0;
}

inline uint8_t set(uint8_t byte, unsigned bit, bool value) {
    const uint8_t mask = static_cast<uint8_t>(1u << bit);
    return value ? static_cast<uint8_t>(byte | mask)
                 : static_cast<uint8_t>(byte & static_cast<uint8_t>(~mask));
}

inline uint8_t field(uint8_t byte, unsigned start, unsigned width) {
    const uint8_t mask = static_cast<uint8_t>((1u << width) - 1u);
    return static_cast<uint8_t>((byte >> start) & mask);
}

inline uint8_t setField(uint8_t byte, unsigned start, unsigned width,
                        uint8_t value) {
    const uint8_t mask = static_cast<uint8_t>(((1u << width) - 1u) << start);
    const uint8_t placed = static_cast<uint8_t>((value << start) & mask);
    return static_cast<uint8_t>((byte & static_cast<uint8_t>(~mask)) | placed);
}

}  // namespace bits
}  // namespace CANopen
