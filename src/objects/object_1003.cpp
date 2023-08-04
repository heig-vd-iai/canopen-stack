#include "object_1003.hpp"
#include "../node.hpp"
using namespace CANopen;

uint8_t Object1003::getCount() { return *(uint8_t *)entries[X1003_INDEX_COUNT].dataSrc; }

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
        shiftErrors();
    return SDOAbortCode_OK;
}

void Object1003::clearErrors()
{
    for (unsigned i = 1; i < subNumber; i++)
        setValue(i, (uint32_t)0);
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
