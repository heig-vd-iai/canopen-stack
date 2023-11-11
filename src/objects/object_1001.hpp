/**
 * Contains the declaration of the "Error register" Object1001 class.
 */
#pragma once
#include "../object.hpp"

namespace CANopen
{
    /**
     * This class represents the Error register (0x1001) object.
     */
    class Object1001 : public Object
    {
    public:
        /**
         * Constructor for the Object class.
         * @param index Index of the object.
         * @param subNumber Number of subentries in the object.
         * @param entries Array of pointers to object entries belonging to that object.
         */
        Object1001(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

        /**
         * Get the value of the error register.
         * @return The error register value.
         */
        uint8_t getValue();

        /**
         * Set a specific error by toggling the specified bit to 1.
         * @param bit The index of the error bit to be set.
         */
        void setErrorBit(unsigned bit);

        /**
         * Clear a specific error by toggling the specified bit to 0.
         * @param bit The index of the error bit to be cleared.
         */
        void clearErrorBit(unsigned bit);

        /**
         * Check if the register is error free.
         * @return True if all errors are cleared, false otherwise.
         */
        bool isErrorfree();

        /**
         * Reset the error register to 0, clearing all errors.
         */
        void reset();
    };
}
