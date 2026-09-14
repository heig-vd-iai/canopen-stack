#include "frame.hpp"

using namespace CANopen;

Frame::Frame(uint8_t nodeId, uint8_t functionCode)
    : nodeId(nodeId), functionCode(functionCode) {}

Frame Frame::fromCobId(uint16_t cobId) {
    return Frame(
        static_cast<uint8_t>(cobId & NODEID_MASK),
        static_cast<uint8_t>((cobId >> FUNCTION_OFFSET) & FUNCTION_MASK));
}

uint16_t Frame::getCobID() const {
    return static_cast<uint16_t>(
        static_cast<unsigned>(functionCode & FUNCTION_MASK) << FUNCTION_OFFSET |
        static_cast<unsigned>(nodeId & NODEID_MASK));
}
