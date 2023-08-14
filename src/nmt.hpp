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

        void initSM();
        void updateSM(NMTServiceCommands command = NMTServiceCommand_None);
        void receiveFrame(class NMTFrame &frame);

    public:
        friend class Node;
        NMT(class Node &node);
        void setTransition(NMTServiceCommands command);
        NMTStates getState();
    };
}
