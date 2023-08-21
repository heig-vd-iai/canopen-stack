#pragma once
#include "object_1A00.hpp"
#define X1600_MAP_DISABLED X1A00_MAP_DISABLED
#define X1600_SAM_MPDO X1A00_SAM_MPDO
#define X1600_DAM_MPDO X1A00_DAM_MPDO

namespace CANopen
{
    class Object1600 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1600(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        uint32_t getMappedValue(uint8_t index);
    };
}
