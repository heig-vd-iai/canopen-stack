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
    } bits_initiate;
    struct
    {
        uint8_t c : 1;
        uint8_t n : 3;
        uint8_t t : 1;
        uint8_t ccs : 3;
    } bits_segment;
};

enum SDOServerStates
{
    SDOServerStates_Ready,
    SDOServerStates_Uploading,
    SDOServerStates_Downloading
};

// create SDOClient and SDOServer sub-classes?
class CANopen_SDO
{
private:
    class CANopen_Node &node;
    SDOServerStates serverState;
    struct
    {
        uint32_t objectSize;
        uint32_t remainingBytes;
        uint8_t *dataSrc;
    } transferData;

    void receiveUpload(CANopen_Frame request);
    void receiveDownload(CANopen_Frame request);

public:
    CANopen_SDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
};
