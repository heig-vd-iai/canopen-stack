#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"

namespace CANopen
{
    class SYNC
    {
    private:
        class Node &node;

    public:
        SYNC(class Node &node);
        void receiveFrame(Frame frame, uint32_t timestamp_us);
    };
}
