#pragma once
#include <cstdint>
#define SYNC_OBJECT_1019 0x1019
#define SYNC_MAX_COUNTER 240

namespace CANopen
{
    class SYNC
    {
    private:
        bool enabled = false;
        class Node &node;
        class Object *syncCounterOverflow;  //TODO: use #ifdef
        uint8_t internalCounter = 1;

    public:
        SYNC(class Node &node);
        void enable();
        void disable();
        void receiveFrame(class Frame frame, uint32_t timestamp_us);
    };
}
