#pragma once
#include "od_classes.hpp"
#define X1A00_INDEX_COUNT 0

class TPDOMappingObject : public Object
{
public:
    TPDOMappingObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}

    uint8_t getCount() { return *(uint8_t *)entries[X1A00_INDEX_COUNT].dataSrc; }
    uint32_t getMappedValue(uint8_t index) { return *(uint32_t *)entries[index + 1].dataSrc; }
};
