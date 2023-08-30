#pragma once
#include "object_1A00.hpp"
#define X1600_MAP_DISABLED X1A00_MAP_DISABLED
#define X1600_SAM_MPDO X1A00_SAM_MPDO
#define X1600_DAM_MPDO X1A00_DAM_MPDO

namespace CANopen
{
    /**
     * @brief This class represents the RPDO mapping parameter (0x1600) object.
     */
    class Object1600 : public Object
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
        Object1600(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

        /**
         * @brief Get the RPDO map value for the specified index.
         * The subindex is equal to index + 1, so index 0 is first RPDO map.
         * @param index The index of the map value.
         * @return The mapped value for the specified index.
         */
        uint32_t getMappedValue(uint8_t index);
    };
}
