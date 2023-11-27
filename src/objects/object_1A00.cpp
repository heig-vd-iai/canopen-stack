/**
 * Contains the definitions of the "TPDO mapping parameter" Object1A00 class.
 */
#include "object_1A00.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes Object1A00::preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t /*sizeBytes*/, Node &node)
{
    if (subindex == OBJECT_INDEX_COUNT)
    {
        uint8_t value = bytes[0];
        if (value > OD_PDO_MAPPING_MAX)
            return SDOAbortCode_DownloadValueTooHigh;
        if (value > X1A00_MAP_DISABLED)
        {
            uint32_t sizeSum = 0;
            for (unsigned i = 0; i < value; i++)
            {
                PDOMapEntry entry = {getMappedValue(i)};
                sizeSum += node.od().findObject(entry.bits.index)->getSize(entry.bits.subindex);
            }
            if (sizeSum > PDO_DLC)
                return SDOAbortCode_MappedPDOLengthExceeded;
        }
    }
    else
    {
        PDOMapEntry entry = {*(uint32_t *)bytes};
        Object *object = node.od().findObject(entry.bits.index);
        if (!object || !object->isSubValid(entry.bits.subindex))
            return SDOAbortCode_ObjectNonExistent;
        AccessType access = object->getAccessType(entry.bits.subindex);
        if (!access.bits.mappable || !access.bits.readable)
            return SDOAbortCode_CannotMapToPDO;
    }
    return SDOAbortCode_OK;
}

uint32_t Object1A00::getMappedValue(uint8_t index)
{
    uint32_t value;
    getValue(index + 1, &value);
    return value;
}
