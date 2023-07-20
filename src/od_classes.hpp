#pragma once
#include <cstdint>

union AccessType
{
    uint8_t value;
    struct
    {
        uint8_t r : 1;
        uint8_t w : 1;
        uint8_t constant : 1;
    } bits;
};

struct ObjectEntry
{
    const void *dataSrc;
    const AccessType accessType;
    const uint8_t dataType;
    const uint8_t size;

    ObjectEntry(void *dataSrc, uint8_t accessType, uint8_t dataType, uint8_t size) : dataSrc(dataSrc), accessType{accessType}, dataType(dataType), size(size) {}
};

struct Object
{
    const uint16_t index;
    const uint8_t subNumber;
    const uint16_t objectType;
    const ObjectEntry *entries;

    Object(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : index(index), subNumber(subNumber), objectType(objectType), entries(entries) {}
    virtual bool writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode);
    virtual bool readBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset, uint32_t *errorCode);
};

struct TPDOCommunicationObject : public Object
{
    TPDOCommunicationObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
    bool writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode);
};

struct TPDOMappingObject : public Object
{
    TPDOMappingObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
};
