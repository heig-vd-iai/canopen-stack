#include "object_1800.hpp"
#include "../node.hpp"
#include <cstring>
using namespace CANopen;

SDOAbortCodes Object1800::preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, Node &node)
{
    bool enabled = isEnabled();
    switch (subindex)
    {
    case X1800_INDEX_COBID: // TPDO COB-ID
    {
        TPDOCobidEntry current = {getCobId()};
        TPDOCobidEntry recent = {*(uint32_t *)bytes};
        // Check if bits 0 to 30 are modified
        if (enabled && (current.value ^ recent.value) & ~0x80000000)
            return SDOAbortCode_InvalidDownloadParameterValue;
        // If a PDO was enabled
        if (current.bits.valid && !recent.bits.valid)
            remap = true;
        break;
    }
    case X1800_INDEX_TRANSMISSION: // transmission type
    {
        uint8_t value = bytes[0];
        if (X1800_SYNC_MAX < value && value < X1800_RTR_SYNC)
            return SDOAbortCode_InvalidDownloadParameterValue;
        break;
    }
    case X1800_INDEX_INHIBIT: // inhibit time
    {
        if (enabled)
            return SDOAbortCode_UnsupportedObjectAccess;
        break;
    }
    case X1800_INDEX_RESERVED: // reserved
        return SDOAbortCode_SubindexNonExistent;
    case X1800_INDEX_EVENT: // event timer
        break;
    case X1800_INDEX_SYNC: // SYNC start value
    {
        if (enabled)
            return SDOAbortCode_UnsupportedObjectAccess;
        break;
    }
    }
    return SDOAbortCode_OK;
}

void Object1800::postWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, Node &node)
{
    if (remap)
    {
        node.reloadTPDO();
        remap = false;
    }
}

uint8_t Object1800::getCount() { return *(uint8_t *)entries[X1800_INDEX_COUNT].dataSrc; }

uint32_t Object1800::getCobId() { return *(uint32_t *)entries[X1800_INDEX_COBID].dataSrc; }

uint8_t Object1800::getTransmissionType() { return *(uint8_t *)entries[X1800_INDEX_TRANSMISSION].dataSrc; }

uint16_t Object1800::getInhibitTime() { return *(uint16_t *)entries[X1800_INDEX_INHIBIT].dataSrc; }

uint32_t Object1800::getInhibitTime_us() { return (uint32_t)getInhibitTime() * 100; }

uint16_t Object1800::getEventTimer() { return *(uint16_t *)entries[X1800_INDEX_EVENT].dataSrc; }

uint8_t Object1800::getSyncStart() { return *(uint8_t *)entries[X1800_INDEX_SYNC].dataSrc; }

bool Object1800::isEnabled() { return ~getCobId() & 0x80000000; }

bool Object1800::isInhibitSupported() { return getCount() >= X1800_INDEX_INHIBIT; }

bool Object1800::isTimerSupported() { return getCount() >= X1800_INDEX_EVENT; }
