#include "object_1800.hpp"
#include "node.hpp"
#include <cstring>

uint8_t TPDOCommunicationObject::getCount() { return *(uint8_t *)entries[X1800_INDEX_COUNT].dataSrc; }

uint32_t TPDOCommunicationObject::getCobId() { return *(uint32_t *)entries[X1800_INDEX_COBID].dataSrc; }

uint8_t TPDOCommunicationObject::getTransmissionType() { return *(uint8_t *)entries[X1800_INDEX_TRANSMISSION].dataSrc; }

uint16_t TPDOCommunicationObject::getInhibitTime() { return *(uint16_t *)entries[X1800_INDEX_INHIBIT].dataSrc; }

uint16_t TPDOCommunicationObject::getEventTimer() { return *(uint16_t *)entries[X1800_INDEX_EVENT].dataSrc; }

uint8_t TPDOCommunicationObject::getSyncStart() { return *(uint8_t *)entries[X1800_INDEX_SYNC].dataSrc; }

bool TPDOCommunicationObject::isEnabled() { return ~getCobId() & 0x80000000; }

bool TPDOCommunicationObject::isInhibitSupported() { return getCount() >= X1800_INDEX_INHIBIT; }

bool TPDOCommunicationObject::isTimerSupported() { return getCount() >= X1800_INDEX_EVENT; }

bool TPDOCommunicationObject::getEnableFlag() { return enabledFlag; }

void TPDOCommunicationObject::clearEnableFlag() { enabledFlag = false; }

bool TPDOCommunicationObject::writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode, CANopen_Node &node)
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
    bool enabled = isEnabled();
    switch (subindex)
    {
    case X1800_INDEX_COBID: // TPDO COB-ID
    {
        TPDOCobidEntry current = {getCobId()};
        TPDOCobidEntry recent = {*(uint32_t *)bytes};
        // Check if bits 0 to 30 are modified
        if (enabled && (current.value ^ recent.value) & ~0x80000000)
        {
            *errorCode = SDOAbortCode_InvalidDownloadParameterValue;
            return false;
        }
        // If a PDO was enabled
        if (current.bits.valid && !recent.bits.valid)
            enabledFlag = true;
        break;
    }
    case X1800_INDEX_TRANSMISSION: // transmission type
    {
        uint8_t value = bytes[0];
        if (X1800_SYNC_MAX < value && value < X1800_RTR_SYNC)
        {
            *errorCode = SDOAbortCode_InvalidDownloadParameterValue;
            return false;
        }
        break;
    }
    case X1800_INDEX_INHIBIT: // inhibit time
    {
        if (enabled)
        {
            *errorCode = SDOAbortCode_UnsupportedObjectAccess;
            return false;
        }
        break;
    }
    case X1800_INDEX_RESERVED: // reserved
    {
        *errorCode = SDOAbortCode_SubindexNonExistent;
        return false;
    }
    case X1800_INDEX_EVENT: // event timer
        break;
    case X1800_INDEX_SYNC: // SYNC start value
    {
        if (enabled)
        {
            *errorCode = SDOAbortCode_UnsupportedObjectAccess;
            return false;
        }
        break;
    }
    }
    memcpy((void *)entry.dataSrc, bytes, size);
    return true;
}
