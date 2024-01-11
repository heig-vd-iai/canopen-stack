/**
 * Contains the definitions of the Object class.
 */
#include "object.hpp"
#include "enums.hpp"
#include "node.hpp"
#include "unions.hpp"
#include <cstring>
using namespace CANopen;

IObjectEntry::IObjectEntry(uint32_t sizeBytes, uint16_t uid, uint8_t metaData) : metaData{metaData}, size(sizeBytes), uid(uid)
{
}

Object::Object(uint16_t index, uint8_t subNumber, IObjectEntry *entries[]) : index(index), subNumber(subNumber), entries(entries)
{
}

SDOAbortCodes Object::beforeRead(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer)
{
    return SDOAbortCode_OK;
}

SDOAbortCodes Object::readBytes(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    IObjectEntry *entry = entries[subindex];
    if (!entry->metaData.bits.readable)
        return SDOAbortCode_AttemptReadOnWriteOnly;
    if (readSize + offset > entry->size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    SDOAbortCodes code = beforeRead(subindex, readSize, offset, buffer);
    if (code != SDOAbortCode_OK)
        return code;
    entry->readBytes(readSize, offset, buffer);
    afterRead(subindex, readSize, offset, buffer);
    return SDOAbortCode_OK;
}

void Object::afterRead(uint8_t subindex, uint32_t readSize, uint32_t offset, uint8_t *buffer)
{
}

SDOAbortCodes Object::beforeWrite(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer)
{
    return SDOAbortCode_OK;
}

SDOAbortCodes Object::writeBytes(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    IObjectEntry *entry = entries[subindex];
    if (!entry->metaData.bits.writeable)
        return SDOAbortCode_AttemptWriteOnReadOnly;
    if (writeSize + offset > entry->size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;

    // TODO: implement limits
    // int limits = entry->checkLimits(bytes);
    // if (limits < 0)
    //     return SDOAbortCode_DownloadValueTooLow;
    // if (limits > 0)
    //     return SDOAbortCode_DownloadValueTooHigh;

    SDOAbortCodes code = beforeWrite(subindex, writeSize, offset, buffer);
    switch (code)
    {
    case SDOAbortCode_OK:
        entry->writeBytes(writeSize, offset, buffer);
        afterWrite(subindex, writeSize, offset, buffer);
        return SDOAbortCode_OK;
    case SDOAbortCode_CancelWrite:
        return SDOAbortCode_OK;
    default:
        return code;
    }
}

void Object::afterWrite(uint8_t subindex, uint32_t writeSize, uint32_t offset, uint8_t *buffer)
{
}

bool Object::isSubValid(uint8_t subindex) const
{
    return subindex < subNumber;
}

uint32_t Object::getSize(uint8_t subindex) const
{
    return isSubValid(subindex) ? entries[subindex]->size : 0;
}

int32_t Object::getUid(uint8_t subindex) const
{
    return isSubValid(subindex) ? entries[subindex]->uid : -1;
}

MetaBitfield Object::getMetadata(uint8_t subindex) const
{
    return isSubValid(subindex) ? entries[subindex]->metaData : MetaBitfield{0};
}
