#pragma once
#include "../object.hpp"
#define X1019_NO_COUNTER 0
#define X1019_MIN_COUNTER 2
#define X1019_MAX_COUNTER 240

namespace CANopen
{
    /**
     * @brief This class represents the Synchronous counter overflow value (0x1019) object.
     */
    class Object1019 : public Object
    {
    private:
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        /**
         * @brief Constructor for the Object class.
         * @param index Index of the object.
         * @param subNumber Number of subentries in the object.
         * @param entries Array of pointers to object entries belonging to that object.
         */
        Object1019(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}
    };
}
