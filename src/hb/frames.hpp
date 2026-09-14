#pragma once

#include <cstdint>

#include "enums.hpp"
#include "frame.hpp"
#include "hb/config.hpp"
#include "nmt/states.hpp"
#include "utils/bits.hpp"

namespace CANopen {
namespace hb {

/**
 * View on a heartbeat, boot-up or node guarding reply.
 * CiA301:2011§7.2.8.3.2.2 and §7.2.8.3.2.1
 */
class Message {
   public:
    explicit Message(const Frame &frame) : frame(frame) {}

    bool isWellFormed() const { return !frame.rtr && frame.dlc == DLC; }
    NMTStates state() const {
        return static_cast<NMTStates>(frame.data[STATE_OFFSET] & STATE_MASK);
    }
    bool toggle() const {
        return bits::get(frame.data[STATE_OFFSET], TOGGLE_BIT);
    }

   private:
    const Frame &frame;
};

inline Frame makeMessage(uint8_t nodeId, NMTStates state, bool toggle) {
    Frame frame(nodeId, FunctionCode_HEARTBEAT);
    frame.dlc = DLC;
    frame.data[STATE_OFFSET] =
        bits::set(static_cast<uint8_t>(state), TOGGLE_BIT, toggle);
    return frame;
}

/** The remote request a node guarding master sends. */
inline Frame makeGuardRequest(uint8_t nodeId) {
    Frame frame(nodeId, FunctionCode_HEARTBEAT);
    frame.rtr = true;
    return frame;
}

}  // namespace hb
}  // namespace CANopen
