#pragma once

#include <cstdint>

#include "enums.hpp"
#include "frame.hpp"
#include "nmt/states.hpp"

namespace CANopen {
namespace nmt {

constexpr uint8_t COMMAND_OFFSET = 0;
constexpr uint8_t TARGET_OFFSET = 1;
constexpr uint8_t DLC = 2;
constexpr uint8_t BROADCAST = 0;

/** View on an NMT command frame. CiA301:2011§7.2.8.3.1 */
class Command {
   public:
    explicit Command(const Frame &frame) : frame(frame) {}

    bool isWellFormed() const {
        return frame.nodeId == 0 && !frame.rtr && frame.dlc >= DLC;
    }
    uint8_t command() const { return frame.data[COMMAND_OFFSET]; }
    uint8_t target() const { return frame.data[TARGET_OFFSET]; }
    bool addresses(uint8_t nodeId) const {
        return target() == BROADCAST || target() == nodeId;
    }

   private:
    const Frame &frame;
};

inline Frame makeCommand(NMTServiceCommands command, uint8_t target) {
    Frame frame(0, FunctionCode_NMT);
    frame.dlc = DLC;
    frame.data[COMMAND_OFFSET] = static_cast<uint8_t>(command);
    frame.data[TARGET_OFFSET] = target;
    return frame;
}

}  // namespace nmt
}  // namespace CANopen
