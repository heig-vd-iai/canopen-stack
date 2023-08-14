#pragma once
#include <cstdint>
#define SYNC_COUNTER_OFFSET 0

namespace CANopen
{
    class SYNC
    {
    private:
        bool enabled = false;
        class Node &node;
        uint8_t internalCounter = 1;
        uint8_t maxCounter;

        void enable();
        void disable();
        void receiveFrame(class SYNCFrame &frame, uint32_t timestamp_us);

    public:
        friend class NMT;
        friend class Node;
        SYNC(class Node &node);
    };
}
