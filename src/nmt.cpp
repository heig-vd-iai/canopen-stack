#include "nmt.hpp"
#include "node.hpp"
#include <cstdio>

CANopen_NMT::CANopen_NMT(CANopen_Node &node) : currentState(NMTState_Initialisation), node(node)
{
}

void CANopen_NMT::receiveFrame(CANopen_Frame frame, uint64_t timestamp_us)
{
    if (frame.functionCode != FunctionCode_NMT || (frame.data[1] != node.nodeId && frame.data[1] != 0))
        return;
    setTransition((NMTServiceCommands)frame.data[0]);
}

void CANopen_NMT::setTransition(NMTServiceCommands command)
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

NMTStates CANopen_NMT::getState()
{
    return currentState;
}

void CANopen_NMT::update(uint64_t timestamp_us)
{
    setTransition((NMTServiceCommands)0);
}
