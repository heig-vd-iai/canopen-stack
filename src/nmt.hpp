#pragma once
#include <cstdint>
#include "enums.hpp"
#define NMT_COMMAND_OFFSET 0
#define NMT_NODEID_OFFSET 1

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
        void receiveFrame(class NMTFrame &frame);
        void setTransition(NMTServiceCommands command);
        void initSM();
        NMTStates getState();
    };
}
