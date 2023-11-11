/**
 * Contains the definitions of the NMT class.
 */
#include "nmt.hpp"
#include "frame.hpp"
#include "node.hpp"
using namespace CANopen;

NMT::NMT(Node &node) : node(node) {}

void NMT::initSM()
{
    updateSM();
}

void CANopen::NMT::updateSM(NMTServiceCommands command)
{
    NMTStates nextState = currentState;
    switch (currentState)
    {
    case NMTState_Initialisation:
        node._pdo.disable();
        node._sdo.disable();
        node._sync.disable();
        node._emcy.disable();
        switch (resetState)
        {
        case NMTResetState_Initialising:
            break;
        case NMTResetState_ResetApplication:
            node._od.restoreData(ParameterGroup_All);
            break;
        case NMTResetState_ResetCommunication:
            node._od.restoreData(ParameterGroup_Communication);
            node._hb.resetToggleBit();
            break;
        }
        node._pdo.reloadTPDO();
        node._pdo.reloadRPDO();
        nextState = NMTState_PreOperational;
    case NMTState_PreOperational:
        node._pdo.disable();
        node._sdo.enable();
        node._sync.enable();
        node._emcy.enable();
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
        node._pdo.enable();
        node._sdo.enable();
        node._sync.enable();
        node._emcy.enable();
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
        node._pdo.disable();
        node._sdo.disable();
        node._sync.disable();
        node._emcy.disable();
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
        node._hb.publishState(NMTState_Initialisation);
    }
    currentState = nextState;
}

void NMT::receiveFrame(NMTFrame &frame)
{
    uint8_t targetId = frame.getTargetId();
    if (frame.nodeId != 0 || (targetId != node.nodeId && targetId != 0))
        return;
    setTransition((NMTServiceCommands)frame.getCommand());
}

void NMT::setTransition(NMTServiceCommands command)
{
    updateSM(command);
    updateSM();
}

NMTStates NMT::getState() { return currentState; }
