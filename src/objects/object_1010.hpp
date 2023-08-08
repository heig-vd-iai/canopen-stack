#pragma once
#include "../object.hpp"
#define X1010_INDEX_COUNT 0
#define X1010_SAVE_SIGNATURE 0x65766173

namespace CANopen
{
    class Object1010 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;

    public:
        Object1010(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
    };
}
