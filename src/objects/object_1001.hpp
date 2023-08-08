#pragma once
#include "../od_classes.hpp"
#define X1001_BIT_GENERIC 0
#define X1001_BIT_CURRENT 1
#define X1001_BIT_VOLTAGE 2
#define X1001_BIT_TEMPERATURE 3
#define X1001_BIT_COMMUNICATION 4
#define X1001_BIT_DEVICEPROFILE 5
#define X1001_BIT_MANUFACTURER 7

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
