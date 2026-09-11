#pragma once

#include <cstdint>

#ifndef CANOPEN_SDO_TIMEOUT_US
#define CANOPEN_SDO_TIMEOUT_US 1000000
#endif
#ifndef CANOPEN_SDO_BLOCK_TIMEOUT_US
#define CANOPEN_SDO_BLOCK_TIMEOUT_US 100000
#endif
#ifndef CANOPEN_SDO_REMOTE_TIMEOUT_US
#define CANOPEN_SDO_REMOTE_TIMEOUT_US 100000
#endif
#ifndef CANOPEN_SDO_BLOCK_SIZE
#define CANOPEN_SDO_BLOCK_SIZE 127
#endif

namespace CANopen {
namespace sdo {

constexpr uint32_t TIMEOUT_US = CANOPEN_SDO_TIMEOUT_US;
constexpr uint32_t BLOCK_TIMEOUT_US = CANOPEN_SDO_BLOCK_TIMEOUT_US;
constexpr uint32_t REMOTE_TIMEOUT_US = CANOPEN_SDO_REMOTE_TIMEOUT_US;
constexpr uint8_t BLOCK_SIZE = CANOPEN_SDO_BLOCK_SIZE;

constexpr uint8_t EXPEDITED_MAX_BYTES = 4;
constexpr uint8_t SEGMENT_MAX_BYTES = 7;
constexpr uint8_t BLOCK_SEQNO_MAX = 127;
constexpr uint32_t BLOCK_BUFFER_SIZE =
    static_cast<uint32_t>(BLOCK_SIZE) * SEGMENT_MAX_BYTES;

static_assert(BLOCK_SIZE >= 1 && BLOCK_SIZE <= BLOCK_SEQNO_MAX,
              "blksize must be in 1..127");

}  // namespace sdo
}  // namespace CANopen
