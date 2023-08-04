#pragma once
#include "../od_classes.hpp"
#define X1800_INDEX_COUNT 0
#define X1800_INDEX_COBID 1
#define X1800_INDEX_TRANSMISSION 2
#define X1800_INDEX_INHIBIT 3
#define X1800_INDEX_RESERVED 4
#define X1800_INDEX_EVENT 5
#define X1800_INDEX_SYNC 6
#define X1800_ACYCLIC 0x0
#define X1800_SYNC_MAX 0xF0
#define X1800_RTR_SYNC 0xFC
#define X1800_RTR_EVENT 0xFD
#define X1800_EVENT1 0xFE
#define X1800_EVENT2 0xFF

namespace CANopen
{
    class Object1800 : public Object
    {
    private:
        bool remap = false;

        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;
        void postWriteBytes(uint8_t subindex, uint8_t *bytes, unsigned size, class Node &node) override;

    public:
        Object1800(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}
        uint8_t getCount();
        uint32_t getCobId();
        uint8_t getTransmissionType();
        uint16_t getInhibitTime();
        uint32_t getInhibitTime_us();
        uint16_t getEventTimer();
        uint8_t getSyncStart();
        bool isEnabled();
        bool isInhibitSupported();
        bool isTimerSupported();
    };
}
