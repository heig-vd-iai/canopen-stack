#pragma once
#include "od_classes.hpp"
#define X1A00_INDEX_COUNT 0
#define X1A00_MAX_ENTRIES 8
#define X1A00_SAM_MPDO 0xFE
#define X1A00_DAM_MPDO 0xFF

class TPDOMappingObject : public Object
{
public:
    TPDOMappingObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}

    uint8_t getCount();
    uint32_t getMappedValue(uint8_t index);
    bool writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode, class CANopen_Node &node);
};
