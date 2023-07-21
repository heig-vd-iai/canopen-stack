#pragma once
#include "od_classes.hpp"

struct TPDOMappingObject : public Object
{
    TPDOMappingObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
};
