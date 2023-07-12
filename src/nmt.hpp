#pragma once
#include "frame.hpp"

enum NMTStates
{
    NMTState_Initialisation = 0,
    NMTState_PreOperational = 127,
    NMTState_Operational = 5,
    NMTState_Stopped = 4
};

enum NMTServiceCommands
{
    NMTServiceCommands_Start = 1,
    NMTServiceCommands_Stop = 2,
    NMTServiceCommands_EnterPreOperational = 128,
    NMTServiceCommands_ResetNode = 129,
    NMTServiceCommands_ResetCommunication = 130
};

class CANopen_NMT
{
private:
    NMTStates currentState;

public:
    CANopen_NMT() {}
    void receiveFrame(CANopen_Frame frame) {}
    void setTransition(NMTServiceCommands command)
    {
        if (currentState == NMTState_Initialisation)
        {
            currentState = NMTState_PreOperational;
        }
        else if (currentState == NMTState_PreOperational)
        {
            if (command == NMTServiceCommands_Start)
            {
                currentState = NMTState_Operational;
            }
            else if (command == NMTServiceCommands_Stop)
            {
                currentState = NMTState_Stopped;
            }
            else if (command == NMTServiceCommands_ResetNode || command == NMTServiceCommands_ResetCommunication)
            {
                currentState = NMTState_Initialisation;
            }
        }
        else if (currentState == NMTState_Operational)
        {
            if (command == NMTServiceCommands_EnterPreOperational)
            {
                currentState = NMTState_PreOperational;
            }
            else if (command == NMTServiceCommands_Stop)
            {
                currentState = NMTState_Stopped;
            }
            else if (command == NMTServiceCommands_ResetNode || command == NMTServiceCommands_ResetCommunication)
            {
                currentState = NMTState_Initialisation;
            }
        }
        else if (currentState == NMTState_Stopped)
        {
            if (command == NMTServiceCommands_EnterPreOperational)
            {
                currentState = NMTState_PreOperational;
            }
            else if (command == NMTServiceCommands_Start)
            {
                currentState = NMTState_Operational;
            }
            else if (command == NMTServiceCommands_ResetNode || command == NMTServiceCommands_ResetCommunication)
            {
                currentState = NMTState_Initialisation;
            }
        }
    }
};
