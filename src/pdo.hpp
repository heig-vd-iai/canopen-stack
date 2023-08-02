#pragma once
#include "od.hpp"
#include <cstdint>
#define TPDO_COMMUNICATION_INDEX 0x1800
#define TPDO_MAPPING_INDEX 0x1A00
#define PDO_DATA_LENGTH 8

namespace CANopen
{
    class PDO
    {
        struct TPDOPair
        {
            class Object *object;
            uint8_t subindex;
        };
        struct TPDO
        {
            class TPDOCommunicationObject *commObject;
            class TPDOMappingObject *mapObject;
            TPDOPair *mappedEntries;
            uint8_t count = 0;
            uint8_t size = 0;
            uint32_t timestamp_us = 0;
            bool syncFlag = false;
            TPDO();
        };

    private:
        class Node &node;
        TPDO tpdos[OD_TPDO_COUNT];

        void initTPDO(unsigned index);
        void remapTPDO(unsigned index);
        void bufferizeTPDO(unsigned index, uint8_t *buffer);
        void sendTPDO(unsigned index, uint32_t timestamp_us);

    public:
        PDO(class Node &node);
        void receiveFrame(class Frame frame, uint32_t timestamp_us);
        void update(uint32_t timestamp_us);
        void onSync(uint8_t counter, uint32_t timestamp_us);
        void transmitTPDO(unsigned index);
        void reload();
    };
}
