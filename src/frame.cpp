/**
 * Contains the definitions of all of the frame classes.
 */
#include "frame.hpp"

using namespace CANopen;

Frame::Frame(uint8_t nodeId, uint8_t functionCode)
    : nodeId(nodeId), functionCode(functionCode) {}

Frame Frame::fromCobId(uint16_t cobId) {
    return Frame(cobId & NODEID_MASK,
                 (cobId >> FUNCTION_OFFSET) & FUNCTION_MASK);
}

uint16_t Frame::getCobID() const {
    return ((uint16_t)functionCode & FUNCTION_MASK) << FUNCTION_OFFSET |
           (nodeId & NODEID_MASK);
}
