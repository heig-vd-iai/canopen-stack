#pragma once
#include "../object.hpp"
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
#define X1800_COBID_VALID_MASK 0x80000000

namespace CANopen
{
    class Object1800 : public Object
    {
    private:
        bool remap = false;

        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;
        void postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        Object1800(uint16_t index, uint8_t subNumber, ObjectEntry *entries) : Object(index, subNumber, entries) {}
        uint32_t getCobId();
        uint16_t getActualCobId();
        uint8_t getTransmissionType();
        uint32_t getInhibitTime_us();
        uint16_t getEventTimer_ms();
        uint32_t getEventTimer_us();
        uint8_t getSyncStart();
        bool isEnabled();
        bool isInhibitSupported();
        bool isTimerSupported();
        bool isSynchronous();
    };
}
