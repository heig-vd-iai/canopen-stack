#pragma once
#include "../object.hpp"
#define X1A00_MAP_DISABLED 0
#define X1A00_SAM_MPDO 0xFE
#define X1A00_DAM_MPDO 0xFF

namespace CANopen
{
    class Object1A00 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1A00(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        uint32_t getMappedValue(uint8_t index);
    };
}
