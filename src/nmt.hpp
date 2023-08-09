#pragma once
#include <cstdint>
#include "enums.hpp"

namespace CANopen
{
    class NMT
    {
    private:
        NMTStates currentState;
        class Node &node;

    public:
        NMT(class Node &node);
        void receiveFrame(class Frame frame);
        void setTransition(NMTServiceCommands command = NMTServiceCommand_None);
        NMTStates getState();
        void update();
    };
}
