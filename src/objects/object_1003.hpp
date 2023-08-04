#pragma once
#include "../od_classes.hpp"
#define X1003_INDEX_COUNT 0

namespace CANopen
{
    class Object1003 : public Object
    {
    private:
        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;
        void clearErrors();
        void shiftErrors();

    public:
        Object1003(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
        uint8_t getCount();
    };
}
