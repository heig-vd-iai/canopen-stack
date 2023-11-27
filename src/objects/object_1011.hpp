/**
 * Contains the declaration of the "Restore default parameters" Object1011 class.
 */
#pragma once
#include "../object.hpp"
#define X1011_LOAD_SIGNATURE 0x64616F6C

namespace CANopen
{
/**
 * Restore default parameters (0x1011) object.
 */
class Object1011 : public Object
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
    Object1011(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}
};
}
