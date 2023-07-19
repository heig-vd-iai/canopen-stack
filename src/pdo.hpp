#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"
#include "od.hpp"

class CANopen_PDO
{
    struct CANopen_TPDO
    {
        uint32_t *cobId;
        uint8_t *transmissionType;
        uint16_t *inhibitTime;
        uint16_t *eventTimer;
        uint8_t *syncStartValue;
        uint8_t mappedObjectsCount;
        OD_Object *mappedObjectsSources[8];
        CANopen_TPDO();
    };

private:
    class CANopen_Node &node;
    CANopen_TPDO tpdos[OD_TPDO_COUNT];

    void initTPDO(unsigned num);
    void remapTPDO(unsigned num);
    void bufferizeTPDO(unsigned num, uint8_t buffer[0]);

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void update(uint32_t timestamp_us);
};
