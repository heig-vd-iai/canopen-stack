#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"

namespace CANopen
{
    class NMT
    {
    private:
        NMTStates currentState;
        class Node &node;

    public:
        NMT(class Node &node);
        void receiveFrame(Frame frame);
        void setTransition(NMTServiceCommands command);
        NMTStates getState();
        void update();
    };
}
