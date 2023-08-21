#include "object_1003.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes Object1003::preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset)
{
    if (subindex > getCount())
        return SDOAbortCode_NoDataAvailable;
    return SDOAbortCode_OK;
}

SDOAbortCodes Object1003::preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node)
{
    if (subindex == OBJECT_INDEX_COUNT)
    {
        if (bytes[0] != 0)
            return SDOAbortCode_InvalidDownloadParameterValue;
        clearErrors();
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

void Object1003::pushError(uint16_t errorCode, uint32_t manufacturerCode)
{
    uint32_t newError = manufacturerCode << 16 | errorCode;
    uint32_t firstError = newError;
    getValue(1, &firstError);
    if (firstError != newError)
    {
        shiftErrors();
        uint8_t count = getCount();
        if (count < subNumber - 1)
            setValue(0, ++count);
        setValue(1, newError);
    }
}

void Object1003::clearErrors()
{
    setValue(0, (uint8_t)0);
    for (unsigned i = 1; i < subNumber; i++)
        setValue(i, (uint32_t)0);
}
