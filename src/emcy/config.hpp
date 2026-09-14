#pragma once

#include <cstdint>

#include "emcy/codes.hpp"

namespace CANopen {
namespace emcy {

constexpr uint16_t ERROR_REGISTER_INDEX = 0x1001;
constexpr uint16_t ERROR_HISTORY_INDEX = 0x1003;
constexpr uint16_t ERROR_BEHAVIOR_INDEX = 0x1029;

constexpr uint8_t HISTORY_SIZE = 8;

/** Sub-indices of the error behavior record. CiA301:2011§7.5.2.27 */
enum BehaviorSubindex : uint8_t {
    BEHAVIOR_COUNT = 0,
    BEHAVIOR_COMMUNICATION = 1,
    BEHAVIOR_DEVICE = 2
};
constexpr uint8_t BEHAVIOR_ENTRIES = 2;

constexpr uint8_t DLC = 8;
constexpr uint8_t CODE_OFFSET = 0;
constexpr uint8_t REGISTER_OFFSET = 2;
constexpr uint8_t MANUFACTURER_OFFSET = 3;

constexpr uint16_t COMMUNICATION_CODES_FIRST = 0x8100;
constexpr uint16_t COMMUNICATION_CODES_LAST = 0x81FF;

constexpr uint8_t registerMask(ErrorRegisterBits bit) {
    return static_cast<uint8_t>(1u << bit);
}

constexpr uint32_t historyEntry(uint16_t errorCode, uint16_t manufacturerCode) {
    return (static_cast<uint32_t>(manufacturerCode) << 16) | errorCode;
}

}  // namespace emcy
}  // namespace CANopen
