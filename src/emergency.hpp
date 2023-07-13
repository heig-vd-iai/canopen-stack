#pragma once
#include "frame.hpp"

class CANopen_EMCY
{
private:
    class CANopen_Node &node;

public:
    CANopen_EMCY(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void publishError(uint16_t errorCode, uint8_t errorRegister);
};
