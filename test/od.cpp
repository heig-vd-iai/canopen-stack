#include "od.hpp"
#include <cstring>
#include <cstdio>
using namespace od;

Object *ObjectDictionnary::findObject(uint16_t index)
{
    for (unsigned i = 0; i < length; i++)
        if (objectsArray[i]->index == index)
            return objectsArray[i];
    printf("[OD] entry 0x%04X not found\n", index);
    return NULL;
}

void ObjectDictionnary::saveData()
{
}

void ObjectDictionnary::loadData()
{
}

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
