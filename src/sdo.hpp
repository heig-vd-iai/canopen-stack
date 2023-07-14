#pragma once
#include "frame.hpp"

union SDO_CommandByte
{
    uint8_t value;
    struct
    {
        uint8_t s : 1;
        uint8_t e : 1;
        uint8_t n : 2;
        uint8_t reserved : 1;
        uint8_t ccs : 3;
    } bits;
};

class CANopen_SDO
{
private:
    class CANopen_Node &node;

    void receiveUpload(CANopen_Frame request);
    void receiveDownload(CANopen_Frame request);

public:
    CANopen_SDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
};
