#pragma once
#include "unions.hpp"
#include "enums.hpp"
#include <cstdint>
#define OBJECT_INDEX_COUNT 0

namespace CANopen
{
    struct ObjectEntryBase
    {
        const void *dataSrc;
        const AccessType accessType;
        const uint32_t size;
        ObjectEntryBase(void *src, uint8_t accessType, uint32_t size) : dataSrc(src), accessType{accessType}, size(size) {}
        virtual bool check() = 0;
    };

    template <typename T>
    struct ObjectEntry : public ObjectEntryBase
    {
        ObjectEntry(void *src, uint8_t accessType) : ObjectEntryBase(src, accessType, sizeof(T)) {}
        bool check() { return true; }
    };

    class Object
    {
    protected:
        const ObjectEntryBase **entries;

        virtual SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);
        virtual void postReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);
        virtual SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);
        virtual void postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);

    public:
        const uint16_t index;
        const uint8_t subNumber;

        Object(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : entries(entries), index(index), subNumber(subNumber) {}
        virtual ~Object() {}
        bool isSubValid(uint8_t subindex);
        uint32_t getSize(uint8_t subindex);
        AccessType getAccessType(uint8_t subindex);
        // Methods called by SDO
        SDOAbortCodes readBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);
        SDOAbortCodes writeBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);
        // Methods called by application
        uint8_t getCount();
        template <typename T>
        bool getValue(uint8_t subindex, T *value)
        {
            if (!isSubValid(subindex) || sizeof(T) != entries[subindex]->size)
                return false;
            *value = *(T *)entries[subindex]->dataSrc;
            return true;
        }
        template <typename T>
        bool setValue(uint8_t subindex, T value)
        {
            if (!isSubValid(subindex) || sizeof(T) != entries[subindex]->size)
                return false;
            *(T *)entries[subindex]->dataSrc = value;
            return true;
        }
    };
}
