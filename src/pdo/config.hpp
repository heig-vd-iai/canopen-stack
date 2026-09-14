#pragma once

#include <cstdint>

#include "enums.hpp"
#include "od_common.hpp"

namespace CANopen {
namespace pdo {

constexpr uint16_t RPDO_COMM_INDEX = 0x1400;
constexpr uint16_t RPDO_MAP_INDEX = 0x1600;
constexpr uint16_t TPDO_COMM_INDEX = 0x1800;
constexpr uint16_t TPDO_MAP_INDEX = 0x1A00;
constexpr uint16_t SYNC_WINDOW_INDEX = 0x1007;

constexpr uint8_t DLC = 8;
constexpr uint8_t MAPPING_MAX = OD_PDO_MAPPING_MAX;
constexpr uint8_t COMM_ENTRIES_MAX = 6;

/** Sub-indices of a PDO communication parameter record. CiA301:2011§7.5.2.35 */
enum CommSubindex : uint8_t {
    COMM_COUNT = 0,
    COMM_COBID = 1,
    COMM_TRANSMISSION = 2,
    COMM_INHIBIT = 3,
    COMM_RESERVED = 4,
    COMM_EVENT_TIMER = 5,
    COMM_SYNC_START = 6
};

constexpr uint32_t COBID_INVALID = 0x80000000u;
constexpr uint32_t COBID_RTR_NOT_ALLOWED = 0x40000000u;
constexpr uint32_t COBID_ID_MASK = 0x7FFu;
constexpr uint32_t COBID_LOCKED_MASK = 0x3FFFFFFFu;

/** Transmission types. CiA301:2011§7.5.2.35 table 66 */
constexpr uint8_t TRANSMISSION_SYNC_ACYCLIC = 0x00;
constexpr uint8_t TRANSMISSION_SYNC_CYCLIC_MAX = 0xF0;
constexpr uint8_t TRANSMISSION_RTR_SYNC = 0xFC;
constexpr uint8_t TRANSMISSION_RTR_EVENT = 0xFD;
constexpr uint8_t TRANSMISSION_EVENT_MANUFACTURER = 0xFE;
constexpr uint8_t TRANSMISSION_EVENT_PROFILE = 0xFF;

constexpr uint32_t INHIBIT_UNIT_US = 100;
constexpr uint32_t EVENT_TIMER_UNIT_US = 1000;
constexpr uint32_t SYNC_WINDOW_UNIT_US = 1000;

/** Array size for a PDO count that may be zero. */
constexpr unsigned slots(unsigned count) { return count == 0 ? 1 : count; }

constexpr uint16_t mappedIndex(uint32_t entry) {
    return static_cast<uint16_t>(entry >> 16);
}

constexpr uint8_t mappedSubindex(uint32_t entry) {
    return static_cast<uint8_t>((entry >> 8) & 0xFFu);
}

constexpr uint8_t mappedBits(uint32_t entry) {
    return static_cast<uint8_t>(entry & 0xFFu);
}

constexpr uint32_t mapEntry(uint16_t index, uint8_t subindex, uint8_t bits) {
    return (static_cast<uint32_t>(index) << 16) |
           (static_cast<uint32_t>(subindex) << 8) | bits;
}

constexpr bool isValidCobId(uint32_t cobId) {
    return (cobId & COBID_INVALID) == 0;
}

constexpr uint16_t canIdOf(uint32_t cobId) {
    return static_cast<uint16_t>(cobId & COBID_ID_MASK);
}

constexpr bool isSynchronousType(uint8_t transmission) {
    return transmission <= TRANSMISSION_SYNC_CYCLIC_MAX;
}

constexpr bool isEventType(uint8_t transmission) {
    return transmission >= TRANSMISSION_EVENT_MANUFACTURER;
}

constexpr bool isReservedType(uint8_t transmission) {
    return transmission > TRANSMISSION_SYNC_CYCLIC_MAX &&
           transmission < TRANSMISSION_RTR_SYNC;
}

}  // namespace pdo
}  // namespace CANopen
