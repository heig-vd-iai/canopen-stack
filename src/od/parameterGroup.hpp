#pragma once

#include <cstdint>

namespace CANopen {

/** Parameter groups of objects 0x1010 and 0x1011. CiA301:2011§7.5.2.15 */
enum ParameterGroups {
    ParameterGroup_All = 1,
    ParameterGroup_Communication = 2,
    ParameterGroup_Application = 3,
    ParameterGroup_ManufacturerA = 4,
};
}  // namespace CANopen

namespace CANopen {

struct IndexRange {
    uint16_t first;
    uint16_t last;
};

inline IndexRange parameterGroupRange(uint8_t parameterGroup) {
    switch (static_cast<ParameterGroups>(parameterGroup)) {
        case ParameterGroup_Communication:
            return {0x1000, 0x1FFF};
        case ParameterGroup_Application:
            return {0x6000, 0x9FFF};
        case ParameterGroup_ManufacturerA:
            return {0x2000, 0x4FFF};
        default:
            return {0x0000, 0xFFFF};
    }
}

inline bool inParameterGroup(uint16_t index, uint8_t parameterGroup) {
    const IndexRange range = parameterGroupRange(parameterGroup);
    return index >= range.first && index <= range.last;
}
}  // namespace CANopen
