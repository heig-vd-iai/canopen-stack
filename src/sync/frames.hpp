#pragma once

#include <cstdint>

#include "enums.hpp"
#include "frame.hpp"
#include "sync/config.hpp"

namespace CANopen {
namespace sync {

/** View on a SYNC message, with or without counter. CiA301:2011§7.2.5.3.1 */
class Message {
   public:
    explicit Message(const Frame &frame) : frame(frame) {}

    bool isWellFormed() const { return !frame.rtr && frame.dlc <= 1; }
    bool hasCounter() const { return frame.dlc == 1; }
    uint8_t counter() const { return frame.data[COUNTER_OFFSET]; }

   private:
    const Frame &frame;
};

inline Frame makeSync(uint16_t cobId = DEFAULT_COBID) {
    return Frame::fromCobId(cobId);
}

inline Frame makeSync(uint8_t counter, uint16_t cobId) {
    Frame frame = Frame::fromCobId(cobId);
    frame.dlc = 1;
    frame.data[COUNTER_OFFSET] = counter;
    return frame;
}

}  // namespace sync
}  // namespace CANopen
