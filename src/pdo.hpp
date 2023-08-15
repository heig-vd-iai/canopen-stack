#pragma once
#include "od.hpp"
#include <cstdint>
#include <functional>
#define TPDO_COMMUNICATION_INDEX 0x1800
#define TPDO_MAPPING_INDEX 0x1A00
#define RPDO_COMMUNICATION_INDEX 0x1400
#define RPDO_MAPPING_INDEX 0x1600
#define PDO_DLC 8

namespace CANopen
{
    class PDO
    {
        struct PDOPair
        {
            class Object *object;
            uint8_t subindex;
        };
        struct TPDO
        {
            class Object1800 *commObject;
            class Object1A00 *mapObject;
            PDOPair mappedEntries[OD_PDO_MAPPING_MAX];
            uint8_t count = 0;
            uint8_t size = 0;
            uint32_t timestamp_us = 0;
            bool syncFlag = false;
        };
        struct RPDO
        {
            class Object1400 *commObject;
            class Object1600 *mapObject;
            PDOPair mappedEntries[OD_PDO_MAPPING_MAX];
            uint8_t buffer[PDO_DLC] = {0};
            uint8_t count = 0;
            uint8_t size = 0;
            uint32_t timestamp_us = 0;
            bool syncFlag = false;
            bool watchTimeoutFlag = false;
        };

    private:
        bool enabled = false;
        class Node &node;
        TPDO tpdos[OD_TPDO_COUNT];
        RPDO rpdos[OD_RPDO_COUNT];
        std::function<void(unsigned)> onReceiveFunc;
        std::function<void(unsigned)> onTimeoutFunc;

        void enable();
        void disable();
        void initTPDO(unsigned index);
        void initRPDO(unsigned index);
        void remapTPDO(unsigned index);
        void remapRPDO(unsigned index);
        void bufferizeTPDO(unsigned index, uint8_t *buffer);
        void unpackRPDO(unsigned index, uint8_t *buffer, uint32_t timestamp_us);
        void sendTPDO(unsigned index, uint32_t timestamp_us);
        void receiveTPDO(class Frame &frame, uint32_t timestamp_us);
        void receiveRPDO(class Frame &frame, uint32_t timestamp_us);
        void update(uint32_t timestamp_us);
        void onSync(uint8_t counter, uint32_t timestamp_us);
        uint32_t getSyncWindow_us();

    public:
        friend class SYNC;
        friend class NMT;
        friend class Node;
        PDO(class Node &node);
        void transmitTPDO(unsigned index);
        void reloadTPDO();
        void reloadRPDO();
        void onReceive(std::function<void(unsigned)> callback);
        void onTimeout(std::function<void(unsigned)> callback);
    };
}
