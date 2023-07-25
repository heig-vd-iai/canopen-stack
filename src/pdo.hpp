#pragma once
#include <cstdint>
#include <cstdlib>
#include "enums.hpp"
#include "frame.hpp"
#include "od.hpp"
#define TPDO_COMMUNICATION_INDEX 0x1800
#define TPDO_MAPPING_INDEX 0x1A00
#define PDO_DATA_LENGTH 8

class CANopen_PDO
{
    struct TPDO
    {
        TPDOCommunicationObject *commObject = NULL;
        TPDOMappingObject *mapObject = NULL;
        ObjectEntry **mappedEntries = NULL;
        uint8_t count = 0;
        uint32_t timestamp_us = 0;
        bool syncFlag = false;
    };

private:
    class CANopen_Node &node;
    TPDO tpdos[OD_TPDO_COUNT];

    void initTPDO(unsigned index);
    void remapTPDO(unsigned index);
    void bufferizeTPDO(unsigned index, uint8_t *buffer);
    void sendTPDO(unsigned index, uint32_t timestamp_us);

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame, uint32_t timestamp_us);
    void update(uint32_t timestamp_us);
    void onSync(uint8_t counter, uint32_t timestamp_us);
    void transmitTPDO(unsigned index);
};
