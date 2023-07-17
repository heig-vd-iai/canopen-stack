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
    if (currentState == NMTState_Initialisation)
    {
        nextState = NMTState_PreOperational;
    }
    else if (currentState == NMTState_PreOperational)
    {
        if (command == NMTServiceCommand_Start)
        {
            nextState = NMTState_Operational;
        }
        else if (command == NMTServiceCommand_Stop)
        {
            nextState = NMTState_Stopped;
        }
        else if (command == NMTServiceCommand_ResetNode || command == NMTServiceCommand_ResetCommunication)
        {
            nextState = NMTState_Initialisation;
        }
    }
    else if (currentState == NMTState_Operational)
    {
        if (command == NMTServiceCommand_EnterPreOperational)
        {
            nextState = NMTState_PreOperational;
        }
        else if (command == NMTServiceCommand_Stop)
        {
            nextState = NMTState_Stopped;
        }
        else if (command == NMTServiceCommand_ResetNode || command == NMTServiceCommand_ResetCommunication)
        {
            currentState = NMTState_Initialisation;
        }
    }
    else if (currentState == NMTState_Stopped)
    {
        if (command == NMTServiceCommand_EnterPreOperational)
        {
            nextState = NMTState_PreOperational;
        }
        else if (command == NMTServiceCommand_Start)
        {
            nextState = NMTState_Operational;
        }
        else if (command == NMTServiceCommand_ResetNode || command == NMTServiceCommand_ResetCommunication)
        {
            nextState = NMTState_Initialisation;
        }
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
