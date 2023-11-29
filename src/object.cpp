/**
 * Contains the definitions of the Object class.
 */
#include "object.hpp"
#include "enums.hpp"
#include "node.hpp"
#include "unions.hpp"
#include <cstring>
using namespace CANopen;

SDOAbortCodes Object::preReadBytes(uint8_t /*subindex*/, uint8_t * /*bytes*/, uint32_t /*sizeBytes*/, uint32_t /*offset*/)
{
    return SDOAbortCode_OK;
}

void Object::postReadBytes(uint8_t /*subindex*/, uint8_t * /*bytes*/, uint32_t /*sizeBytes*/, uint32_t /*offset*/)
{
}

SDOAbortCodes Object::preWriteBytes(uint8_t /*subindex*/, uint8_t * /*bytes*/, uint32_t /*sizeBytes*/, class Node & /*node*/)
{
    return SDOAbortCode_OK;
}

void Object::postWriteBytes(uint8_t /*subindex*/, uint8_t * /*bytes*/, uint32_t /*sizeBytes*/, class Node & /*node*/)
{
}

SDOAbortCodes Object::writeBytes(uint8_t subindex, uint8_t *bytes, uint32_t sizeBytes, Node &node)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntryBase *entry = (ObjectEntryBase *)entries[subindex];
    if (!entry->metaData.bits.writeable)
        return SDOAbortCode_AttemptWriteOnReadOnly;
    if (sizeBytes != entry->sizeBytes)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    int limits = entry->checkLimits(bytes);
    if (limits < 0)
        return SDOAbortCode_DownloadValueTooLow;
    if (limits > 0)
        return SDOAbortCode_DownloadValueTooHigh;
    SDOAbortCodes code = preWriteBytes(subindex, bytes, sizeBytes, node);
    switch (code)
    {
    case SDOAbortCode_OK:
        memcpy((void *)entry->dataSrc, bytes, sizeBytes);
        postWriteBytes(subindex, bytes, sizeBytes, node);
        if (onWriteFunc)
            onWriteFunc(*this, subindex);
        return SDOAbortCode_OK;
    case SDOAbortCode_CancelWrite:
        return SDOAbortCode_OK;
    default:
        return code;
    }
}

// TODO: remove CANopen::
void CANopen::Object::requestUpdate(unsigned subindex)
{
    if (!isRemote() || !isSubValid(subindex))
        return;
    entries[subindex]->metaData.bits.updateFlag = true;
    onRequestUpdateFunc(*this, subindex);
}

SDOAbortCodes Object::readBytes(uint8_t subindex, uint8_t *bytes, uint32_t sizeBytes, uint32_t offset)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntryBase *entry = (ObjectEntryBase *)entries[subindex];
    if (!entry->metaData.bits.readable)
        return SDOAbortCode_AttemptReadOnWriteOnly;
    if (sizeBytes + offset > entry->sizeBytes)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    SDOAbortCodes code = preReadBytes(subindex, bytes, sizeBytes, offset);
    if (code != SDOAbortCode_OK)
        return code;
    memcpy(bytes, (uint8_t *)entry->dataSrc + offset, sizeBytes);
    postReadBytes(subindex, bytes, sizeBytes, offset);
    return SDOAbortCode_OK;
}

bool Object::isSubValid(uint8_t subindex)
{
    return subindex < subNumber;
}

uint32_t Object::getSize(uint8_t subindex)
{
    return isSubValid(subindex) ? entries[subindex]->sizeBytes : 0;
}

MetaBitfield Object::getMetadata(uint8_t subindex)
{
    return isSubValid(subindex) ? entries[subindex]->metaData : MetaBitfield{0};
}

uint8_t Object::getCount()
{
    return *(uint8_t *)entries[OBJECT_INDEX_COUNT]->dataSrc;
}

bool CANopen::Object::isRemote()
{
    return (bool)onRequestUpdateFunc;
}

void CANopen::Object::onWrite(std::function<void(Object &, unsigned)> callback)
{
    onWriteFunc = callback;
}

void CANopen::Object::onRequestUpdate(std::function<void(Object &, unsigned)> callback)
{
    onRequestUpdateFunc = callback;
}
