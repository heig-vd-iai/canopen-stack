#pragma once
#include <cstdint>

namespace CANopen
{
    class EMCY
    {
    private:
        class Node &node;

    public:
        EMCY(class Node &node);
        void receiveFrame(class Frame frame);
        void publishError(uint16_t errorCode, uint8_t errorRegister);
    };
}
