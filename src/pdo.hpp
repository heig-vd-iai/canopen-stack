#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"
#include "od.hpp"

struct PDOMap
{
    uint8_t count;
    OD_Object *sources[8];
};

class CANopen_PDO
{
private:
    class CANopen_Node &node;

    PDOMap tpdos[OD_TPDO_COUNT];

    void build(PDOMap tpdo, uint8_t *buffer);

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void update(uint32_t timestamp_us);
};
