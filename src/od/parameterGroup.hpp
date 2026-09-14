#pragma once

#include <cstdint>

#include "enums.hpp"

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
