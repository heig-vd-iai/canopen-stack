#pragma once
#include <cstdint>

namespace CANopen
{
    class EMCY
    {
    private:
        bool enabled = false;
        bool errorFree = true;
        class Node &node;

    public:
        EMCY(class Node &node);
        void enable();
        void disable();
        void receiveFrame(class Frame frame);
        void publishError(uint16_t errorCode, uint8_t errorRegister);
        void publishResetError();
    };
}
