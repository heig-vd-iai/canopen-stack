#include "od_classes.hpp"
#include <cstring>

bool Object::writeBytes(uint8_t subindex, uint8_t bytes[], unsigned size)
{
    if (subindex > subNumber)
        return false;
    ObjectEntry entry = entries[subindex];
    if (size > entry.size)
        return false;
    memcpy((void *)entry.dataSrc, bytes, size);
    return true;
}
