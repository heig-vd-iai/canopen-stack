#pragma once
#include "unions.hpp"
#include "enums.hpp"
#include <cstdint>

namespace CANopen
{
    struct ObjectEntry
    {
        const void *dataSrc;
        const AccessType accessType;
        const uint8_t dataType;
        const uint8_t size;

        ObjectEntry(void *dataSrc, uint8_t accessType, uint8_t dataType, uint8_t size) : dataSrc(dataSrc), accessType{accessType}, dataType(dataType), size(size) {}
    };

    class Object
    {
    protected:
        const ObjectEntry *entries;

    public:
        const uint16_t index;
        const uint8_t subNumber;
        const uint16_t objectType;

        Object(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : entries(entries), index(index), subNumber(subNumber), objectType(objectType) {}
        bool isSubValid(uint8_t subindex);
        uint8_t getSize(uint8_t subindex);
        AccessType getAccessType(uint8_t subindex);
        // Methods called mainly by SDO
        SDOAbortCodes readBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset);
        virtual SDOAbortCodes writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node);
        // Methods called mainly by application
        bool getValue(uint8_t subindex, uint8_t *value);
        bool setValue(uint8_t subindex, uint8_t value);
        bool getValue(uint8_t subindex, int8_t *value);
        bool setValue(uint8_t subindex, int8_t value);
        bool getValue(uint8_t subindex, uint16_t *value);
        bool setValue(uint8_t subindex, uint16_t value);
        bool getValue(uint8_t subindex, int16_t *value);
        bool setValue(uint8_t subindex, int16_t value);
        bool getValue(uint8_t subindex, uint32_t *value);
        bool setValue(uint8_t subindex, uint32_t value);
        bool getValue(uint8_t subindex, int32_t *value);
        bool setValue(uint8_t subindex, int32_t value);
        bool getValue(uint8_t subindex, uint64_t *value);
        bool setValue(uint8_t subindex, uint64_t value);
        bool getValue(uint8_t subindex, int64_t *value);
        bool setValue(uint8_t subindex, int64_t value);
        bool getValue(uint8_t subindex, float *value);
        bool setValue(uint8_t subindex, float value);
        bool getValue(uint8_t subindex, double *value);
        bool setValue(uint8_t subindex, double value);
    };
}
