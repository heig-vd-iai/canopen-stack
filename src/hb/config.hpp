#pragma once

#include <cstdint>

namespace CANopen {
namespace hb {

constexpr uint16_t PRODUCER_TIME_INDEX = 0x1017;

constexpr uint8_t DLC = 1;
constexpr uint8_t STATE_OFFSET = 0;
constexpr unsigned TOGGLE_BIT = 7;
constexpr uint8_t STATE_MASK = 0x7F;

constexpr uint32_t PERIOD_UNIT_US = 1000;

}  // namespace hb
}  // namespace CANopen
