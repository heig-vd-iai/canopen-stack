#pragma once
#include "frame.hpp"

namespace CANopen
{
    class EMCY
    {
    private:
        class Node &node;

    public:
        EMCY(class Node &node);
        void receiveFrame(Frame frame);
        void publishError(uint16_t errorCode, uint8_t errorRegister);
    };
}
