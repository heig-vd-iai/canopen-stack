/**
 * Contains the declarations of OD objects and entries.
 */
#pragma once
#include "enums.hpp"
#include "unions.hpp"
#include <cstdint>
#define OBJECT_INDEX_COUNT 0

namespace CANopen
{
    /**
     * base object entry in the Object Dictionnary.
     * An object entry is identified by a sub-index, and belongs to an object.
     */
    struct ObjectEntryBase
    {
        const void *dataSrc;
        const AccessType accessType;
        const uint32_t size;

        /**
         * TODO: We see it is a constructor: remove word constructor, documentation useless :(
         * TODO: rename size with sizeInBytes or sizeBytes to avoid comment.
         * Constructor for the base object entry.
         * @param src Pointer to the data source.
         * @param accessType Access type of the object entry.
         * @param size Size in bytes of the data associated with the entry.
         */
        ObjectEntryBase(void *src, uint8_t accessType, uint32_t size) : dataSrc(src), accessType{accessType}, size(size) {}

        /**
         * Check if incoming data is within defined range.
         * @param data Pointer to the raw data to be checked.
         * @return 0 for unbound entries. For limited entries, 0 if data is within limits, -1 if below, 1 if above.
         */
        // TODO: Liskov substitution principle not respected here. All derived classes should return 0.
        inline int checkLimits(void *data) const { return 0; }
    };

    /**
     * This class represents an object entry in the Object Dictionary.
     * An object entry is identified by a sub-index, and belongs to an object.
     * @tparam T Data type associated with the object entry.
     */
    template <typename T>
    struct ObjectEntry : public ObjectEntryBase
    {
        /**
         * Constructor for the ObjectEntry.
         * @param src Pointer to the data source.
         * @param accessType Access type of the object entry.
         */
        ObjectEntry(void *src, uint8_t accessType) : ObjectEntryBase(src, accessType, sizeof(T)) {}
    };

    /**
     * This class represents a limited object entry in the Object Dictionary with value constraints.
     * An object entry is identified by a sub-index, and belongs to an object.
     * @tparam T Data type associated with the object entry.
     */
    // TODO: Not all objects have limited values ? Should be easier to have a generic object with or without limits?
    template <typename T>
    struct LimitedObjectEntry : public ObjectEntryBase
    {
        const T minVal, maxVal;

        /**
         * Constructor for the LimitedObjectEntry.
         * @param src Pointer to the data source.
         * @param accessType Access type of the object entry.
         * @param minVal Minimum allowed value.
         * @param maxVal Maximum allowed value.
         */
        LimitedObjectEntry(void *src, uint8_t accessType, T minVal, T maxVal) : ObjectEntryBase(src, accessType, sizeof(T)), minVal(minVal), maxVal(maxVal) {}

        /**
         * Check if incoming data is within defined range.
         * @param data Pointer to the raw data to be checked.
         * @return 0 if data is within limits, -1 if below, 1 if above.
         */
        inline int checkLimits(void *data)
        {
            T value = *(T *)data;
            return value < minVal ? -1 : (value > maxVal ? 1 : 0);
        }
    };

    /**
     * This class represents an object in the Object Dictionnary.
     * An object is identified by an index, and owns multiple entries.
     */
    class Object
    {
    protected:
        const ObjectEntryBase **entries;

        /**
         * Pre-read operation hook for processing before reading bytes.
         * If the operation is allowed, the function must return SDOAbortCode_OK, otherwise readBytes will be cancelled.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the destination buffer.
         * @param size Number of bytes to read.
         * @param offset Offset within the object entry data.
         * @return SDOAbortCodes indicating operation status.
         */
        virtual SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);

        /**
         * Post-read operation hook for processing after reading bytes.
         * This function is called after a sucessful readBytes operation.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the destination buffer.
         * @param size Number of bytes read.
         * @param offset Offset within the object entry data.
         */
        virtual void postReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);

        /**
         * Pre-write operation hook for processing before writing bytes.
         * If the operation is allowed, the function must return SDOAbortCode_OK, otherwise writeBytes will be cancelled.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the source buffer.
         * @param size Number of bytes to write.
         * @param node Reference to the Node instance.
         * @return SDOAbortCodes indicating operation status.
         */
        virtual SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);

        /**
         * Post-write operation hook for processing after writing bytes.
         * Called after a sucessful writeBytes operation.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the source buffer.
         * @param size Number of bytes written.
         * @param node Reference to the Node instance.
         */
        virtual void postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);

    public:
        const uint16_t index;
        const uint8_t subNumber;

        /**
         * Constructor for the Object class.
         * @param index Index of the object.
         * @param subNumber Number of subentries in the object.
         * @param entries Array of pointers to object entries belonging to that object.
         */
        Object(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : entries(entries), index(index), subNumber(subNumber) {}

        /**
         * Check if the subindex exists.
         * @param subindex Subindex to check.
         * @return True if the subindex exists, false otherwise.
         */
        bool isSubValid(uint8_t subindex);

        /**
         * Get the size of an object's entry data.
         * @param subindex Subindex of the object entry.
         * @return Size in bytes of the object entry data.
         */
        uint32_t getSize(uint8_t subindex);

        /**
         * Get the access type of an object's entry.
         * @param subindex Subindex of the object entry.
         * @return Access type of the object entry.
         */
        AccessType getAccessType(uint8_t subindex);

        /**
         * Read data from an object entry.
         * This method should only be called by SDO class.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the destination buffer.
         * @param size Number of bytes to read.
         * @param offset Offset within the object entry data.
         * @return SDOAbortCodes indicating operation status.
         */
        SDOAbortCodes readBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset);

        /**
         * Write bytes to an object entry.
         * This method should only be called by SDO class.
         * @param subindex Subindex of the object entry.
         * @param bytes Pointer to the source buffer.
         * @param size Number of bytes to write.
         * @param node Reference to the Node instance.
         * @return SDOAbortCodes indicating operation status.
         */
        SDOAbortCodes writeBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node);

        /**
         * Get the number of entries in the object, **if object is not of type VAR**.
         * @return Number of entries.
         */
        uint8_t getCount();

        /**
         * Get the value of an object's entry.
         * If the size of the data type does not match that of the actual data, the operation will fail.
         * @tparam T Data type of the value.
         * @param subindex Subindex of the object entry.
         * @param value Pointer to store the retrieved value.
         * @return True if the value was retrieved successfully, false otherwise.
         */
        template <typename T>
        inline bool getValue(uint8_t subindex, T *value)
        {
            if (!isSubValid(subindex) || sizeof(T) != entries[subindex]->size)
                return false;
            *value = *(T *)entries[subindex]->dataSrc;
            return true;
        }

        /**
         * Set the value of an object's entry.
         * If the size of the data type does not match that of the actual data, the operation will fail.
         * @tparam T Data type of the value.
         * @param subindex Subindex of the object entry.
         * @param value New value to set.
         * @return True if the value was set successfully, false otherwise.
         */
        template <typename T>
        inline bool setValue(uint8_t subindex, T value)
        {
            if (!isSubValid(subindex) || sizeof(T) != entries[subindex]->size)
                return false;
            *(T *)entries[subindex]->dataSrc = value;
            return true;
        }
    };
}
