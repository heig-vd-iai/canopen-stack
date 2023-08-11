#pragma once
#include "../object.hpp"

namespace CANopen
{
    class Object1001 : public Object
    {
    public:
        Object1001(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        uint8_t getValue();
        void setErrorBit(unsigned bit);
        void clearErrorBit(unsigned bit);
        bool isErrorfree();
        void reset();
    };
}
