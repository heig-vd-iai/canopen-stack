/******************************************************************************
 * [Filename]:      object_1400.cpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the definitions of the "RPDO communication parameter" Object1400 class.
 *****************************************************************************/
#include "object_1400.hpp"
#include "../frame.hpp"
#include "../node.hpp"
#include <cstring>
using namespace CANopen;

SDOAbortCodes Object1400::preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset)
{
    if (subindex == X1400_INDEX_RESERVED)
        return SDOAbortCode_SubindexNonExistent;
    return SDOAbortCode_OK;
}

SDOAbortCodes Object1400::preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, Node &node)
{
    bool enabled = isEnabled();
    switch (subindex)
    {
    case X1400_INDEX_COBID:
    {
        PDOCobidEntry current = {getCobId()};
        PDOCobidEntry recent = {*(uint32_t *)bytes};
        // Check if bits 0 to 30 are modified
        if (enabled && (current.value ^ recent.value) & ~X1400_COBID_VALID_MASK)
            return SDOAbortCode_InvalidDownloadParameterValue;
        // If a PDO was enabled
        if (current.bits.valid && !recent.bits.valid)
            remap = true;
        break;
    }
    case X1400_INDEX_TRANSMISSION:
    {
        uint8_t value = bytes[0];
        if (X1400_SYNC_MAX < value && value < X1400_EVENT1)
            return SDOAbortCode_InvalidDownloadParameterValue;
        break;
    }
    case X1400_INDEX_INHIBIT:
    {
        if (enabled)
            return SDOAbortCode_UnsupportedObjectAccess;
        break;
    }
    case X1400_INDEX_RESERVED:
    case X1400_INDEX_SYNC:
        return SDOAbortCode_SubindexNonExistent;
    case X1400_INDEX_EVENT:
        break;
    }
    return SDOAbortCode_OK;
}

void Object1400::postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, Node &node)
{
    if (remap)
    {
        node.pdo().reloadRPDO();
        remap = false;
    }
}

uint32_t Object1400::getCobId()
{
    uint32_t value;
    getValue(X1400_INDEX_COBID, &value);
    return value;
}

uint16_t Object1400::getActualCobId() { return getCobId() & COBID_MASK; }

uint8_t Object1400::getTransmissionType()
{
    uint8_t value;
    getValue(X1400_INDEX_TRANSMISSION, &value);
    return value;
}

uint32_t Object1400::getInhibitTime_us()
{
    uint16_t value;
    getValue(X1400_INDEX_INHIBIT, &value);
    return (uint32_t)value * 100;
}

uint16_t Object1400::getEventTimer_ms()
{
    uint16_t value;
    getValue(X1400_INDEX_EVENT, &value);
    return value;
}

uint32_t Object1400::getEventTimer_us() { return (uint32_t)getEventTimer_ms() * 1000; }

bool Object1400::isEnabled() { return ~getCobId() & X1400_COBID_VALID_MASK; }

bool Object1400::isInhibitSupported() { return getCount() >= X1400_INDEX_INHIBIT; }

bool Object1400::isTimerSupported() { return getCount() >= X1400_INDEX_EVENT; }

bool Object1400::isSynchronous() { return getTransmissionType() <= X1400_SYNC_MAX; }
