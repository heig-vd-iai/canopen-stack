#pragma once
#include "od_classes.hpp"
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
private:
    bool enabledFlag = false;

public:
    TPDOCommunicationObject(uint16_t index, uint8_t subNumber, uint16_t objectType, ObjectEntry *entries) : Object(index, subNumber, objectType, entries) {}

    uint8_t getCount();
    uint32_t getCobId();
    uint8_t getTransmissionType();
    uint16_t getInhibitTime();
    uint16_t getEventTimer();
    uint8_t getSyncStart();
    bool isEnabled();
    bool isInhibitSupported();
    bool isTimerSupported();
    bool getEnableFlag();
    void clearEnableFlag();
    bool writeBytes(uint8_t subindex, uint8_t *bytes, unsigned size, uint32_t *errorCode, class CANopen_Node &node);
};
