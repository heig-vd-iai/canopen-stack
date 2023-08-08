#pragma once
#include "../object.hpp"

namespace CANopen
{
    class Object1001 : public Object
    {
    private:
        void writeErrorBit(unsigned bit, bool value);

    public:
        Object1001(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
        uint8_t getValue();
        void setErrorBit(unsigned bit);
        void clearErrorBit(unsigned bit);
        bool isErrorfree();
    };
}
