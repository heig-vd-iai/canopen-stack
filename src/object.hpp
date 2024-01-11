/**
 * Contains the declarations of OD objects and entries.
 */
#pragma once
#include "enums.hpp"
#include "unions.hpp"
#include <cstdint>
#include <functional>
#define OBJECT_INDEX_COUNT 0

namespace CANopen
{
struct IObjectEntry
{
    MetaBitfield metaData;
    const uint32_t size;
    const uint16_t uid;

    IObjectEntry(uint32_t sizeBytes, uint16_t uid, uint8_t metaData = 0xFF); // TODO: remote default value
    virtual void readBytes(uint32_t readSize, uint32_t offset, uint8_t *buff) = 0;
    virtual void writeBytes(uint32_t writeSize, uint32_t offset, uint8_t *buff) = 0;
};

template <typename T>
struct LocalObjectEntry : public IObjectEntry
{
    uint8_t value[sizeof(T)];

    LocalObjectEntry(const T &defaultValue) : IObjectEntry(sizeof(T), 0) { memcpy(value, &defaultValue, sizeof(T)); }
    LocalObjectEntry() : IObjectEntry(sizeof(T), 0), value() {}

    void readBytes(uint32_t readSize, uint32_t offset, uint64_t *buff)
    {
        if (readSize + offset <= size)
            memcpy(buff, value + offset, size < readSize ? size : readSize);
    }

    void writeBytes(uint32_t writeSize, uint32_t offset, uint64_t *buff)
    {
        if (writeSize + offset <= size)
            memcpy(value + offset, buff, size < writeSize ? size : writeSize);
    }
};

class Object
{
public: // TODO: set to private
    SDOAbortCodes readBytes(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer);
    SDOAbortCodes writeBytes(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer);

protected:
    IObjectEntry **entries;

    virtual SDOAbortCodes beforeRead(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer);
    virtual void afterRead(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer);
    virtual SDOAbortCodes beforeWrite(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer);
    virtual void afterWrite(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer);

public:
    const uint16_t index;
    const uint8_t subNumber;

    Object(uint16_t index, uint8_t subNumber, IObjectEntry *entries[]);

    /**
     * Check if the subindex exists.
     * @param subindex Subindex to check.
     * @return True if the subindex exists, false otherwise.
     */
    bool isSubValid(uint8_t subindex) const;

    /**
     * Get the size of an object's entry data.
     * @param subindex Subindex of the object entry.
     * @return Size in bytes of the object entry data, 0 if subindex is invalid.
     */
    uint32_t getSize(uint8_t subindex) const;

    /**
     * Returns the incrementally assigned unique identifier of an entry.
     * @param subindex Subindex of the object entry.
     * @return The uid of the entry, -1 if subindex is invalid.
     */
    int32_t getUid(uint8_t subindex) const;

    /**
     * Get the metadata of an object's entry.
     * @param subindex Subindex of the object entry.
     * @return Metadata bitfield of the object entry.
     */
    MetaBitfield getMetadata(uint8_t subindex) const;
};
}