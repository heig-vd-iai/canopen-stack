#pragma once

#include <cstdint>

namespace CANopen {
namespace sync {

constexpr uint16_t COBID_INDEX = 0x1005;
constexpr uint16_t COUNTER_OVERFLOW_INDEX = 0x1019;

constexpr uint16_t DEFAULT_COBID = 0x80;
constexpr uint32_t COBID_ID_MASK = 0x7FFu;

constexpr uint8_t COUNTER_OFFSET = 0;
constexpr uint8_t COUNTER_NONE = 0;
constexpr uint8_t COUNTER_MIN = 2;
constexpr uint8_t COUNTER_MAX = 240;

constexpr bool isValidOverflow(uint8_t value) {
    return value == COUNTER_NONE ||
           (value >= COUNTER_MIN && value <= COUNTER_MAX);
}

}  // namespace sync
}  // namespace CANopen
