#pragma once
#include "../object.hpp"
#define X1011_LOAD_SIGNATURE 0x64616F6C

namespace CANopen
{
    class Object1011 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1011(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
    };
}
