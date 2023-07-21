#pragma once
#include "od_classes.hpp"
#include <cstring>
#define X1800_INDEX_COUNT 0
#define X1800_INDEX_COBID 1
#define X1800_INDEX_TRANSMISSION 2
#define X1800_INDEX_INHIBIT 3
#define X1800_INDEX_RESERVED 4
#define X1800_INDEX_EVENT 5
#define X1800_INDEX_SYNC 6
#define X1800_SYNC_MAX 0xF0
#define X1800_RTR_SYNC 0xFC
#define X1800_RTR_EVENT 0xFD
#define X1800_EVENT1 0xFE
#define X1800_EVENT2 0xFF

class TPDOCommunicationObject : public Object
{
public:
    TPDOCommunicationObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}

    uint8_t getCount() { return *(uint8_t *)entries[X1800_INDEX_COUNT].dataSrc; }
    uint32_t getCobId() { return *(uint32_t *)entries[X1800_INDEX_COBID].dataSrc; }
    uint8_t getTransmissionType() { return *(uint8_t *)entries[X1800_INDEX_TRANSMISSION].dataSrc; }
    uint16_t getInhibitTime() { return *(uint16_t *)entries[X1800_INDEX_INHIBIT].dataSrc; }
    uint16_t getEventTimer() { return *(uint16_t *)entries[X1800_INDEX_EVENT].dataSrc; }
    uint8_t getSyncStart() { return *(uint8_t *)entries[X1800_INDEX_SYNC].dataSrc; }
    bool isEnabled() { return ~getCobId() & 0x80000000; }
    bool isInhibitSupported() { return getCount() >= X1800_INDEX_INHIBIT; }
    bool isTimerSupported() { return getCount() >= X1800_INDEX_EVENT; }

    bool writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode)
    {
        if (subindex > subNumber)
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
            uint32_t value = *(uint32_t *)bytes;
            if (enabled && (getCobId() ^ value) & ~0x80000000)
            {
                *errorCode = SDOAbortCode_InvalidDownloadParameterValue;
                return false;
            }
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
};
