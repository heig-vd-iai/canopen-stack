#pragma once
#include <cstdint>
#include <cstdlib>
#include "enums.hpp"
#include "frame.hpp"
#define HB_OBJECT_1017 0x1017

namespace CANopen
{
    class HB
    {
    private:
        class Node &node;
        class Object *heartbeatTimeObject = NULL;
        uint32_t lastPublish = 0;

    public:
        HB(class Node &node);
        void receiveFrame(Frame frame);
        void publishState(NMTStates state);
        void update(uint32_t timestamp_us);
    };
}
