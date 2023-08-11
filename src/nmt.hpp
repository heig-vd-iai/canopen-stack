#pragma once
#include <cstdint>
#include "enums.hpp"

namespace CANopen
{
    class NMT
    {
    private:
        NMTStates currentState = NMTState_Initialisation;
        NMTResetStates resetState = NMTResetState_Initialising;
        class Node &node;

        void updateSM(NMTServiceCommands command = NMTServiceCommand_None);

    public:
        NMT(class Node &node);
        void receiveFrame(class Frame &frame);
        void setTransition(NMTServiceCommands command);
        void initSM();
        NMTStates getState();
    };
}
