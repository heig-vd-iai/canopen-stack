#pragma once
#include "object_1800.hpp"
#define X1400_INDEX_COBID X1800_INDEX_COBID
#define X1400_INDEX_TRANSMISSION X1800_INDEX_TRANSMISSION
#define X1400_INDEX_INHIBIT X1800_INDEX_INHIBIT
#define X1400_INDEX_RESERVED X1800_INDEX_RESERVED
#define X1400_INDEX_EVENT X1800_INDEX_EVENT
#define X1400_INDEX_SYNC X1800_INDEX_SYNC
#define X1400_SYNC_MAX X1800_SYNC_MAX
#define X1400_EVENT1 X1800_EVENT1
#define X1400_EVENT2 X1800_EVENT2
#define X1400_COBID_VALID_MASK X1800_COBID_VALID_MASK

namespace CANopen
{
    class Object1400 : public Object
    {
    private:
        bool remap = false;

        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;
        void postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1400(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        uint32_t getCobId();
        uint16_t getActualCobId();
        uint8_t getTransmissionType();
        uint32_t getInhibitTime_us();
        uint16_t getEventTimer_ms();
        uint32_t getEventTimer_us();
        bool isEnabled();
        bool isInhibitSupported();
        bool isTimerSupported();
        bool isSynchronous();
    };
}
