/**
 * Contains the declaration of the "Pre-defined error field" Object1003 class.
 */
#pragma once
#include "../object.hpp"

namespace CANopen
{
    /**
     * This class represents the Pre-defined error field (0x1003) object.
     */
    class Object1003 : public Object
    {
    private:
        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

        /**
         * Shifts the error history.
         * Older errors are shifted to higher subindexes.
         */
        void shiftErrors();

    public:
        /**
         * Constructor for the Object class.
         * @param index Index of the object.
         * @param subNumber Number of subentries in the object.
         * @param entries Array of pointers to object entries belonging to that object.
         */
        Object1003(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

        /**
         * Push an error code in the error history.
         * @param errorCode The error code.
         * @param manufacturerCode The manufacturer-specific error code.
         */
        void pushError(uint16_t errorCode, uint32_t manufacturerCode);

        /**
         * Clear the error history.
         */
        void clearErrors();
    };
}
