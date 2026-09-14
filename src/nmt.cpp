/**
 * Contains the definitions of the NMT class.
 */
#include "nmt.hpp"

#include "frame.hpp"
#include "od/parameterGroup.hpp"

using namespace CANopen;

NMT::NMT(ObjectDictionnary &od, RemoteObjects &remote, ServiceBus &bus,
         uint8_t nodeId)
    : od(od), remote(remote), bus(bus), nodeId(nodeId) {}

void NMT::initSM() { updateSM(); }

void NMT::runInitialisation() {
    ParameterGroups pg = resetState == NMTResetState_ResetCommunication
                             ? ParameterGroup_Communication
                             : ParameterGroup_All;
    if (!od.loadData(pg)) od.restoreData(pg);
    if (pg == ParameterGroup_All) {
        remote.resetRemote();
        if (onReset != nullptr) onReset();
    }
}

void NMT::updateSM(NMTServiceCommands command) {
    NMTStates nextState = currentState;
    switch (currentState) {
        case NMTState_Initialisation:
            runInitialisation();
            nextState = NMTState_PreOperational;
            break;
        case NMTState_PreOperational:
            switch (command) {
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
            switch (command) {
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
            switch (command) {
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
    if (nextState == currentState) return;
    currentState = nextState;
    bus.publishNmtState(currentState);
}

void NMT::receiveFrame(NMTFrame &frame) {
    uint8_t targetId = frame.getTargetId();
    if (frame.nodeId != 0 || (targetId != nodeId && targetId != 0)) return;
    setTransition(static_cast<NMTServiceCommands>(frame.getCommand()));
}

void NMT::setTransition(NMTServiceCommands command) {
    updateSM(command);
    updateSM();
}

NMTStates NMT::getState() { return currentState; }
