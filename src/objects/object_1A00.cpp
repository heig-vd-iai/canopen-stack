#include "object_1A00.hpp"
#include "node.hpp"
#include <cstring>

uint8_t TPDOMappingObject::getCount() { return *(uint8_t *)entries[X1A00_INDEX_COUNT].dataSrc; }

uint32_t TPDOMappingObject::getMappedValue(uint8_t index) { return *(uint32_t *)entries[index + 1].dataSrc; }

SDOAbortCodes TPDOMappingObject::writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, CANopen_Node &node)
{
    if (subindex >= subNumber)
        return SDOAbortCode_SubindexNonExistent;
    ObjectEntry entry = entries[subindex];
    if (size != entry.size)
        return SDOAbortCode_DataTypeMismatch_LengthParameterMismatch;
    if (subindex == X1A00_INDEX_COUNT)
    {
        uint8_t value = bytes[0];
        if (value > X1A00_MAX_ENTRIES)
            return SDOAbortCode_DownloadValueTooHigh;
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
                return SDOAbortCode_MappedPDOLengthExceeded;
            }
        }
    }
    else
    {
        TPDOMapEntry entry = {*(uint32_t *)bytes};
        Object *object = node.od.findObject(entry.bits.index);
        if (object == NULL || !object->isSubValid(entry.bits.subindex))
            return SDOAbortCode_ObjectNonExistent;
        if (entry.bits.length != object->getSize(entry.bits.subindex) * 8) //TODO: uint32_t != uint8_t
            return SDOAbortCode_CannotMapToPDO;
    }
    memcpy((void *)entry.dataSrc, bytes, size);
    return SDOAbortCode_OK;
}
