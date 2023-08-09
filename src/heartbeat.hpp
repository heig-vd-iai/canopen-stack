#pragma once
#include <cstdint>
#include "enums.hpp"

namespace CANopen
{
    class HB
    {
    private:
        class Node &node;
        uint32_t lastPublish = 0;

    public:
        HB(class Node &node);
        void receiveFrame(class Frame frame);
        void publishState(NMTStates state);
        void update(uint32_t timestamp_us);
    };
}
