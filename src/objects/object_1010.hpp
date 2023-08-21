#pragma once
#include "../object.hpp"
#define X1010_SAVE_SIGNATURE 0x65766173

namespace CANopen
{
    class Object1010 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1010(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
    };
}
