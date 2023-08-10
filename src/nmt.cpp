#include "nmt.hpp"
#include "node.hpp"
#include "frame.hpp"
using namespace CANopen;

NMT::NMT(Node &node) : node(node)
{
}

void CANopen::NMT::updateSM(NMTServiceCommands command)
{
    NMTStates nextState = currentState;
    switch (currentState)
    {
    case NMTState_Initialisation:
        node.pdo.disable();
        node.sdo.disable();
        node.sync.disable();
        node.emcy.disable();
        switch (resetState)
        {
        case NMTResetState_Initialising:
            break;
        case NMTResetState_ResetApplication:
            node.od.restoreData(ParameterGroup_All);
            break;
        case NMTResetState_ResetCommunication:
            node.od.restoreData(ParameterGroup_Communication);
            break;
        }
        nextState = NMTState_PreOperational;
    case NMTState_PreOperational:
        node.pdo.disable();
        node.sdo.enable();
        node.sync.enable();
        node.emcy.enable();
        switch (command)
        {
        case NMTServiceCommand_Start:
            nextState = NMTState_Operational;
            break;
        case NMTServiceCommand_Stop:
            nextState = NMTState_Stopped;
            break;
        case NMTServiceCommand_ResetNode:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetApplication;
            break;
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetCommunication;
            break;
        default:
            break;
        }
        break;
    case NMTState_Operational:
        node.pdo.enable();
        node.sdo.enable();
        node.sync.enable();
        node.emcy.enable();
        switch (command)
        {
        case NMTServiceCommand_EnterPreOperational:
            nextState = NMTState_PreOperational;
            break;
        case NMTServiceCommand_Stop:
            nextState = NMTState_Stopped;
            break;
        case NMTServiceCommand_ResetNode:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetApplication;
            break;
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetCommunication;
            break;
        default:
            break;
        }
        break;
    case NMTState_Stopped:
        node.pdo.disable();
        node.sdo.disable();
        node.sync.disable();
        node.emcy.disable();
        switch (command)
        {
        case NMTServiceCommand_EnterPreOperational:
            nextState = NMTState_PreOperational;
            break;
        case NMTServiceCommand_Start:
            nextState = NMTState_Operational;
            break;
        case NMTServiceCommand_ResetNode:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetApplication;
            break;
        case NMTServiceCommand_ResetCommunication:
            nextState = NMTState_Initialisation;
            resetState = NMTResetState_ResetCommunication;
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
    currentState = nextState;
}

void NMT::receiveFrame(Frame frame)
{
    if (frame.cobId.bits.nodeId != 0 || (frame.data[1] != node.nodeId && frame.data[1] != 0))
        return;
    setTransition((NMTServiceCommands)frame.data[0]);
}

void NMT::setTransition(NMTServiceCommands command)
{
    updateSM(command);
    updateSM();
}

void NMT::initSM()
{
    updateSM();
}

NMTStates NMT::getState() { return currentState; }
