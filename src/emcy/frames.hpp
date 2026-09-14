#pragma once

#include <cstdint>

#include "emcy/config.hpp"
#include "enums.hpp"
#include "frame.hpp"
#include "utils/endian.hpp"

namespace CANopen {
namespace emcy {

/** View on an emergency message. CiA301:2011§7.2.7.3.1 */
class Message {
   public:
    explicit Message(const Frame &frame) : frame(frame) {}

    bool isWellFormed() const { return !frame.rtr && frame.dlc == DLC; }
    uint16_t errorCode() const {
        return endian::readLe16(frame.data + CODE_OFFSET);
    }
    uint8_t errorRegister() const { return frame.data[REGISTER_OFFSET]; }
    uint32_t manufacturerCode() const {
        return endian::readLe32(frame.data + MANUFACTURER_OFFSET);
    }

   private:
    const Frame &frame;
};

inline Frame makeMessage(uint8_t nodeId, uint16_t errorCode,
                         uint8_t errorRegister, uint32_t manufacturerCode) {
    Frame frame(nodeId, FunctionCode_EMCY);
    frame.dlc = DLC;
    endian::writeLe16(frame.data + CODE_OFFSET, errorCode);
    frame.data[REGISTER_OFFSET] = errorRegister;
    endian::writeLe32(frame.data + MANUFACTURER_OFFSET, manufacturerCode);
    return frame;
}

}  // namespace emcy
}  // namespace CANopen
