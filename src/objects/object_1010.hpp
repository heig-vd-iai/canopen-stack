/**
 * Contains the declaration of the "Store parameters" Object1010 class.
 */
#pragma once
#include "../object.hpp"
#define X1010_SAVE_SIGNATURE 0x65766173

namespace CANopen
{
/**
 * Store parameters (0x1010) object.
 */
class Object1010 : public Object
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
    Object1010(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}
};
}
