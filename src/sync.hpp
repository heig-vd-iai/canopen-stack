#pragma once
#include <cstdint>

namespace CANopen
{
    class SYNC
    {
    private:
        bool enabled = false;
        class Node &node;
        uint8_t internalCounter = 1;
        uint8_t maxCounter;

    public:
        SYNC(class Node &node);
        void enable();
        void disable();
        void receiveFrame(class Frame frame, uint32_t timestamp_us);
    };
}
