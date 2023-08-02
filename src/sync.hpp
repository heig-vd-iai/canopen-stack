#pragma once
#include <cstdint>
#define SYNC_OBJECT_1019 0x1019
#define SYNC_MAX_COUNTER 240

namespace CANopen
{
    class SYNC
    {
    private:
        class Node &node;
        class Object *syncCounterOverflow;
        uint8_t internalCounter = 1;

    public:
        SYNC(class Node &node);
        void receiveFrame(class Frame frame, uint32_t timestamp_us);
    };
}
