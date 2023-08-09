#pragma once
#include <cstdint>
#define X1029_SUB_COMMUNICATION 1
#define X1029_BEHAVIOUR_PREOP 0x00
#define X1029_BEHAVIOUR_NONE 0x01
#define X1029_BEHAVIOUR_STOPPED 0x02

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
        void raiseError(uint16_t errorCode, uint16_t manufacturerCode = 0);
        void clearErrorBit(unsigned bit);
        void clearHistory();
        void reset();
    };
}
