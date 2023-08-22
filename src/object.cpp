#include "object.hpp"
#include "node.hpp"
#include "enums.hpp"
#include "unions.hpp"
#include <cstring>
using namespace CANopen;

SDOAbortCodes Object::preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset)
{
    return SDOAbortCode_OK;
}

void Object::postReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset)
{
}

SDOAbortCodes Object::preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node)
{
    return SDOAbortCode_OK;
}

void Object::postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node)
{
}

SDOAbortCodes Object::writeBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, Node &node)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntryBase *entry = (ObjectEntryBase *)entries[subindex];
    if (!entry->accessType.bits.writeable)
        return SDOAbortCode_AttemptWriteOnReadOnly;
    if (size != entry->size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    SDOAbortCodes code = preWriteBytes(subindex, bytes, size, node);
    switch (code)
    {
    case SDOAbortCode_OK:
        memcpy((void *)entry->dataSrc, bytes, size);
        postWriteBytes(subindex, bytes, size, node);
        return SDOAbortCode_OK;
    case SDOAbortCode_CancelWrite:
        return SDOAbortCode_OK;
    default:
        return code;
    }
}

SDOAbortCodes Object::readBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset)
{
    if (!isSubValid(subindex))
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntryBase *entry = (ObjectEntryBase *)entries[subindex];
    if (!entry->accessType.bits.readable)
        return SDOAbortCode_AttemptReadOnWriteOnly;
    if (size + offset > entry->size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    SDOAbortCodes code = preReadBytes(subindex, bytes, size, offset);
    if (code != SDOAbortCode_OK)
        return code;
    memcpy(bytes, (uint8_t *)entry->dataSrc + offset, size);
    postReadBytes(subindex, bytes, size, offset);
    return SDOAbortCode_OK;
}

bool Object::isSubValid(uint8_t subindex)
{
    return subindex < subNumber;
}

uint32_t Object::getSize(uint8_t subindex)
{
    if (!isSubValid(subindex))
        return 0;
    return entries[subindex]->size;
}

AccessType Object::getAccessType(uint8_t subindex)
{
    if (!isSubValid(subindex))
        return AccessType{0};
    return entries[subindex]->accessType;
}

uint8_t Object::getCount()
{
    return *(uint8_t *)entries[OBJECT_INDEX_COUNT]->dataSrc;
}
