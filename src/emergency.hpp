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
        class Object1001 *errorRegisterObject;

        void sendError(uint16_t errorCode, uint32_t manufacturerCode);

    public:
        EMCY(class Node &node);
        void enable();
        void disable();
        void receiveFrame(class Frame frame);
        void transmitError(uint16_t errorCode, uint16_t manufacturerCode = 0);
        void clearErrorBit(unsigned bit);
        void clearHistory();
    };
}
