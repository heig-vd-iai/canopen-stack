#pragma once
#include "../object.hpp"
#define X1011_INDEX_COUNT 0
#define X1011_LOAD_SIGNATURE 0x64616F6C

namespace CANopen
{
    class Object1011 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;

    public:
        Object1011(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
    };
}
