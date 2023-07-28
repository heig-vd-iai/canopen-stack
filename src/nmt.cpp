#include "nmt.hpp"
#include "node.hpp"
using namespace CANopen;

NMT::NMT(Node &node) : currentState(NMTState_Initialisation), node(node) {}

void NMT::receiveFrame(Frame frame)
{
    if (frame.cobId.bits.functionCode != FunctionCode_NMT || (frame.data[1] != node.nodeId && frame.data[1] != 0))
        return;
    setTransition((NMTServiceCommands)frame.data[0]);
}

void NMT::setTransition(NMTServiceCommands command)
{
    NMTStates nextState = currentState;

    switch (currentState)
    {
    case NMTState_Initialisation:
        nextState = NMTState_PreOperational;
        break;
    case NMTState_PreOperational:
        switch (command)
        {
        case NMTServiceCommand_Start:
            nextState = NMTState_Operational;
            break;
        case NMTServiceCommand_Stop:
            nextState = NMTState_Stopped;
            break;
        case NMTServiceCommand_ResetNode:
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            break;
        default:
            break;
        }
        break;
    case NMTState_Operational:
        switch (command)
        {
        case NMTServiceCommand_EnterPreOperational:
            nextState = NMTState_PreOperational;
            break;
        case NMTServiceCommand_Stop:
            nextState = NMTState_Stopped;
            break;
        case NMTServiceCommand_ResetNode:
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            break;
        default:
            break;
        }
        break;
    case NMTState_Stopped:
        switch (command)
        {
        case NMTServiceCommand_EnterPreOperational:
            nextState = NMTState_PreOperational;
            break;
        case NMTServiceCommand_Start:
            nextState = NMTState_Operational;
            break;
        case NMTServiceCommand_ResetNode:
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            break;
        default:
            break;
        }
        break;
    }
    if (currentState == NMTState_Initialisation && nextState == NMTState_PreOperational)
    {
        node.hb.publishState(NMTState_Initialisation);
    }
    // if (currentState != nextState)
    // {
    //     printf("[NMT] Node %d entered state 0x%02X\n\n", node.nodeId, nextState);
    // }
    currentState = nextState;
}

NMTStates NMT::getState()
{
    return currentState;
}

void NMT::update()
{
    setTransition((NMTServiceCommands)0);
}
