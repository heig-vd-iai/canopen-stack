#pragma once

#include <cstdint>

namespace CANopen {

/**
 * Non-volatile storage of the object dictionary, one image per parameter
 * group (objects 0x1010 and 0x1011). The signature identifies the dictionary
 * layout so that an image saved by another firmware is rejected.
 */
class Persistence {
   public:
    virtual ~Persistence() = default;
    virtual void init() = 0;
    virtual bool saveGroup(uint8_t parameterGroup) = 0;
    virtual bool loadGroup(uint8_t parameterGroup) = 0;
    virtual bool saveSignature(uint64_t signature) = 0;
    virtual bool loadSignature(uint64_t &signature) = 0;
};
}  // namespace CANopen
