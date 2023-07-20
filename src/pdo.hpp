#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"
#include "od.hpp"

class CANopen_PDO
{
    // union CobId
    // {
    //     uint32_t value;
    //     struct
    //     {
    //         uint32_t canId : 29;
    //         uint32_t frame : 1;
    //         uint32_t rtr : 1;
    //         uint32_t valid : 1;
    //     } bits;
    // };
    // struct TPDO
    // {
    //     uint32_t *cobId;
    //     uint8_t *transmissionType;
    //     uint16_t *inhibitTime;
    //     uint16_t *eventTimer;
    //     uint8_t *syncStartValue;
    //     uint8_t mappedObjectsCount;
    //     OD_Object **mappedObjects;
    //     uint32_t timestamp;
    //     TPDO();
    // };

private:
    class CANopen_Node &node;
    // TPDO tpdos[OD_TPDO_COUNT];

    // void initTPDO(unsigned num);
    // void remapTPDO(unsigned num);
    // void bufferizeTPDO(unsigned num, uint8_t buffer[0]);

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void update(uint32_t timestamp_us);
};
