#pragma once
#include <cstdint>
#include "enums.hpp"
#define HEARTBEAT_DLC 1
#define HEARTBEAT_STATE_OFFSET 0

namespace CANopen
{
    class HB
    {
    private:
        class Node &node;
        uint32_t lastPublish = 0;

    public:
        HB(class Node &node);
        void publishState(NMTStates state);
        void update(uint32_t timestamp_us);
    };
}
