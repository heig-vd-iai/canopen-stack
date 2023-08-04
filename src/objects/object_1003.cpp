#include "object_1003.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes Object1003::preReadBytes(uint8_t subindex, uint8_t *bytes, unsigned size, unsigned offset)
{
    if (subindex > getCount())
        return SDOAbortCode_NoDataAvailable;
    return SDOAbortCode_OK;
}

SDOAbortCodes Object1003::preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node)
{
    if (subindex == X1003_INDEX_COUNT)
    {
        if (bytes[0] != 0)
            return SDOAbortCode_InvalidDownloadParameterValue;
        clearErrors();
    }
    else
    {
        shiftErrors();
        incrCount();
    }
    return SDOAbortCode_OK;
}

void Object1003::shiftErrors()
{
    uint32_t value = 0;
    for (unsigned i = subNumber - 1; i > 1; i--)
    {
        getValue(i - 1, &value);
        setValue(i, value);
    }
}

void Object1003::incrCount()
{
    uint8_t count = getCount();
    if (count < subNumber - 1)
        setValue(0, ++count);
}

uint8_t Object1003::getCount() { return *(uint8_t *)entries[X1003_INDEX_COUNT].dataSrc; }

void Object1003::pushError(uint32_t value)
{
    shiftErrors();
    incrCount();
    setValue(1, value);
}

void Object1003::clearErrors()
{
    setValue(0, (uint8_t)0);
    for (unsigned i = 1; i < subNumber; i++)
        setValue(i, (uint32_t)0);
}
