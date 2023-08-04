#include "node.hpp"
#include "enums.hpp"
#include "unions.hpp"
#include <cstring>
using namespace CANopen;

Object *ObjectDictionnary::findObject(uint16_t index)
{
    int32_t lower = 0;
    int32_t upper = length - 1;
    while (lower <= upper)
    {
        int32_t mid = lower + (upper - lower) / 2;
        if (objectsArray[mid]->index == index)
            return objectsArray[mid];
        else if (objectsArray[mid]->index < index)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return NULL;
}

SDOAbortCodes Object::writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, Node &node)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntry entry = entries[subindex];
    if (!entry.accessType.bits.w)
        return SDOAbortCode_AttemptWriteOnReadOnly;
    if (size != entry.size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    memcpy((void *)entry.dataSrc, bytes, size);
    return SDOAbortCode_OK;
}

SDOAbortCodes Object::readBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntry entry = entries[subindex];
    if (!entry.accessType.bits.r)
        return SDOAbortCode_AttemptReadOnWriteOnly;
    if (size + offset > entry.size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    memcpy(bytes, (uint8_t *)entry.dataSrc + offset, size);
    return SDOAbortCode_OK;
}

bool Object::isSubValid(uint8_t subindex)
{
    return subindex < subNumber;
}

uint8_t Object::getSize(uint8_t subindex)
{
    if (!isSubValid(subindex))
        return 0;
    return entries[subindex].size;
}

AccessType Object::getAccessType(uint8_t subindex)
{
    if (!isSubValid(subindex))
        return AccessType{0};
    return entries[subindex].accessType;
}

bool Object::getValue(uint8_t subindex, uint8_t *value)
{
    if (!isSubValid(subindex) || sizeof(uint8_t) != entries[subindex].size)
        return false;
    *value = *(uint8_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, uint8_t value)
{
    if (!isSubValid(subindex) || sizeof(uint8_t) != entries[subindex].size)
        return false;
    *(uint8_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, int8_t *value)
{
    if (!isSubValid(subindex) || sizeof(int8_t) != entries[subindex].size)
        return false;
    *value = *(int8_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, int8_t value)
{
    if (!isSubValid(subindex) || sizeof(int8_t) != entries[subindex].size)
        return false;
    *(int8_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, uint16_t *value)
{
    if (!isSubValid(subindex) || sizeof(uint16_t) != entries[subindex].size)
        return false;
    *value = *(uint16_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, uint16_t value)
{
    if (!isSubValid(subindex) || sizeof(uint16_t) != entries[subindex].size)
        return false;
    *(uint16_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, int16_t *value)
{
    if (!isSubValid(subindex) || sizeof(int16_t) != entries[subindex].size)
        return false;
    *value = *(int16_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, int16_t value)
{
    if (!isSubValid(subindex) || sizeof(int16_t) != entries[subindex].size)
        return false;
    *(int16_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, uint32_t *value)
{
    if (!isSubValid(subindex) || sizeof(uint32_t) != entries[subindex].size)
        return false;
    *value = *(uint32_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, uint32_t value)
{
    if (!isSubValid(subindex) || sizeof(uint32_t) != entries[subindex].size)
        return false;
    *(uint32_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, int32_t *value)
{
    if (!isSubValid(subindex) || sizeof(int32_t) != entries[subindex].size)
        return false;
    *value = *(int32_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, int32_t value)
{
    if (!isSubValid(subindex) || sizeof(int32_t) != entries[subindex].size)
        return false;
    *(int32_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, uint64_t *value)
{
    if (!isSubValid(subindex) || sizeof(uint64_t) != entries[subindex].size)
        return false;
    *value = *(uint64_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, uint64_t value)
{
    if (!isSubValid(subindex) || sizeof(uint64_t) != entries[subindex].size)
        return false;
    *(uint64_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, int64_t *value)
{
    if (!isSubValid(subindex) || sizeof(int64_t) != entries[subindex].size)
        return false;
    *value = *(int64_t *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, int64_t value)
{
    if (!isSubValid(subindex) || sizeof(int64_t) != entries[subindex].size)
        return false;
    *(int64_t *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, float *value)
{
    if (!isSubValid(subindex) || sizeof(float) != entries[subindex].size)
        return false;
    *value = *(float *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, float value)
{
    if (!isSubValid(subindex) || sizeof(float) != entries[subindex].size)
        return false;
    *(float *)entries[subindex].dataSrc = value;
    return true;
}

bool Object::getValue(uint8_t subindex, double *value)
{
    if (!isSubValid(subindex) || sizeof(double) != entries[subindex].size)
        return false;
    *value = *(double *)entries[subindex].dataSrc;
    return true;
}

bool Object::setValue(uint8_t subindex, double value)
{
    if (!isSubValid(subindex) || sizeof(double) != entries[subindex].size)
        return false;
    *(double *)entries[subindex].dataSrc = value;
    return true;
}
