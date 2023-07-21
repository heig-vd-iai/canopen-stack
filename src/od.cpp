#include "od.hpp"
#include "enums.hpp"
#include "unions.hpp"
#include <fstream>
#include <cstdlib>
#include <cstring>

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
    std::ofstream f("file.dat", std::ios::out | std::ios::binary);
    if (!f)
        return;
    f.write((char *)this, sizeof(*this));
    f.close();
}

void ObjectDictionnary::loadData()
{
    std::ifstream f("file.dat", std::ios::in | std::ios::binary);
    if (!f)
        return;
    f.read((char *)this, sizeof(*this));
    f.close();
}

bool Object::writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode)
{
    if (subindex > subNumber)
    {
        *errorCode = SDOAbortCode_SubindexNonExistent;
        return false;
    }
    ObjectEntry entry = entries[subindex];
    if (size != entry.size)
    {
        *errorCode = SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
        return false;
    }
    memcpy((void *)entry.dataSrc, bytes, size);
    return true;
}

bool Object::readBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset, uint32_t *errorCode)
{
    if (subindex > subNumber)
    {
        *errorCode = SDOAbortCode_SubindexNonExistent;
        return false;
    }
    ObjectEntry entry = entries[subindex];
    if (size + offset > entry.size)
    {
        *errorCode = SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
        return false;
    }
    memcpy(bytes, (uint8_t *)entry.dataSrc + offset, size);
    return true;
}
