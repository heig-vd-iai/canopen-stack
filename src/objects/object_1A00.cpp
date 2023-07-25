#include "object_1A00.hpp"
#include "node.hpp"
#include <cstring>

uint8_t TPDOMappingObject::getCount() { return *(uint8_t *)entries[X1A00_INDEX_COUNT].dataSrc; }

uint32_t TPDOMappingObject::getMappedValue(uint8_t index) { return *(uint32_t *)entries[index + 1].dataSrc; }

bool TPDOMappingObject::writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode, CANopen_Node &node)
{
    if (subindex >= subNumber)
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
    if (subindex == X1A00_INDEX_COUNT)
    {
        uint8_t value = bytes[0];
        if (value > X1A00_MAX_ENTRIES)
        {
            *errorCode = SDOAbortCode_DownloadValueTooHigh;
            return false;
        }
        if (value > 0)
        {
            unsigned sizeSumBytes = 0;
            for (unsigned i = 0; i < value; i++)
            {
                TPDOMapEntry entry = {getMappedValue(i)};
                sizeSumBytes += entry.bits.length / 8;
            }
            if (sizeSumBytes > PDO_DATA_LENGTH)
            {
                *(uint8_t *)entries[X1A00_INDEX_COUNT].dataSrc = 0;
                *errorCode = SDOAbortCode_MappedPDOLengthExceeded;
                return false;
            }
        }
    }
    else
    {
        TPDOMapEntry entry = {*(uint32_t *)bytes};
        Object *object = node.od.findObject(entry.bits.index);
        if (object == NULL || entry.bits.subindex >= object->subNumber)
        {
            *errorCode = SDOAbortCode_ObjectNonExistent;
            return false;
        }
        if (entry.bits.length != object->entries[entry.bits.subindex].size * 8)
        {
            *errorCode = SDOAbortCode_CannotMapToPDO;
            return false;
        }
    }
    memcpy((void *)entry.dataSrc, bytes, size);
    return true;
}