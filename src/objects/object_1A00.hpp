/**
 * Contains the declaration of the "TPDO mapping parameter" Object1A00 class.
 */
#pragma once
#include "../object.hpp"
#define X1A00_MAP_DISABLED 0
#define X1A00_SAM_MPDO 0xFE
#define X1A00_DAM_MPDO 0xFF

namespace CANopen
{
/**
 * TPDO mapping parameter (0x1A00) object.
 */
class Object1A00 : public Object
{
private:
    SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

public:
    /**
     * Constructor for the Object class.
     * @param index Index of the object.
     * @param subNumber Number of subentries in the object.
     * @param entries Array of pointers to object entries belonging to that object.
     */
    Object1A00(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

    /**
     * Get the TPDO map value for the specified index.
     * The subindex is equal to index + 1, so index 0 is first TPDO map.
     * @param index The index of the map value.
     * @return The mapped value for the specified index.
     */
    uint32_t getMappedValue(uint8_t index);
};
}
