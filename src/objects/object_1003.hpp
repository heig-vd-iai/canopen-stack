#pragma once
#include "../object.hpp"

namespace CANopen
{
    class Object1003 : public Object
    {
    private:
        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;
        void shiftErrors();

    public:
        Object1003(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        void pushError(uint16_t errorCode, uint32_t manufacturerCode);
        void clearErrors();
    };
}
