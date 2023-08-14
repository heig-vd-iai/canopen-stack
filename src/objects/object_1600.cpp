#include "object_1600.hpp"
#include "../node.hpp"
using namespace CANopen;

SDOAbortCodes Object1600::preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, Node &node)
{
    if (subindex == OBJECT_INDEX_COUNT)
    {
        uint8_t value = bytes[0];
        if (value > X1600_MAX_ENTRIES)
            return SDOAbortCode_DownloadValueTooHigh;
        if (value > X1600_MAP_DISABLED)
        {
            unsigned sizeSum = 0;
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
        if (entry.bits.length != ((uint32_t)object->getSize(entry.bits.subindex)) * 8 || !access.bits.mappable || !access.bits.writeable) // TODO: check for entry.bits.length ?
            return SDOAbortCode_CannotMapToPDO;
    }
    return SDOAbortCode_OK;
}

uint32_t Object1600::getMappedValue(uint8_t index)
{
    uint32_t value;
    getValue(index + 1, &value);
    return value;
}
