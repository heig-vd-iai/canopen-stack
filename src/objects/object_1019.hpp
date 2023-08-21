#pragma once
#include "../object.hpp"
#define X1019_NO_COUNTER 0
#define X1019_MIN_COUNTER 2
#define X1019_MAX_COUNTER 240

namespace CANopen
{
    class Object1019 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1019(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
    };
}
