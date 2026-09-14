#include "nmt/nmt.hpp"

#include "nmt/frames.hpp"
#include "od/parameterGroup.hpp"

using namespace CANopen;

namespace {

struct Transition {
    NMTServiceCommands command;
    NMTStates from;
    NMTStates to;
};

/** Allowed transitions. CiA301:2011§7.3.2.1 figure 41 */
constexpr Transition TRANSITIONS[] = {
    {NMTServiceCommand_Start, NMTState_PreOperational, NMTState_Operational},
    {NMTServiceCommand_Start, NMTState_Stopped, NMTState_Operational},
    {NMTServiceCommand_Stop, NMTState_PreOperational, NMTState_Stopped},
    {NMTServiceCommand_Stop, NMTState_Operational, NMTState_Stopped},
    {NMTServiceCommand_EnterPreOperational, NMTState_Operational,
     NMTState_PreOperational},
    {NMTServiceCommand_EnterPreOperational, NMTState_Stopped,
     NMTState_PreOperational},
    {NMTServiceCommand_ResetNode, NMTState_PreOperational,
     NMTState_Initialisation},
    {NMTServiceCommand_ResetNode, NMTState_Operational,
     NMTState_Initialisation},
    {NMTServiceCommand_ResetNode, NMTState_Stopped, NMTState_Initialisation},
    {NMTServiceCommand_ResetCommunication, NMTState_PreOperational,
     NMTState_Initialisation},
    {NMTServiceCommand_ResetCommunication, NMTState_Operational,
     NMTState_Initialisation},
    {NMTServiceCommand_ResetCommunication, NMTState_Stopped,
     NMTState_Initialisation},
};

}  // namespace

NMT::NMT(ObjectDictionnary &od, RemoteObjects &remote, ServiceBus &bus,
         uint8_t nodeId)
    : od(od), remote(remote), bus(bus), nodeId(nodeId) {}

void NMT::initSM() { reset(ParameterGroup_All); }

void NMT::reset(uint8_t parameterGroup) {
    enter(NMTState_Initialisation);
    if (!od.loadData(parameterGroup)) od.restoreData(parameterGroup);
    if (parameterGroup == ParameterGroup_All) {
        remote.resetRemote();
        if (onReset != nullptr) onReset();
    }
    enter(NMTState_PreOperational);
}

void NMT::enter(NMTStates state) {
    if (state == currentState) return;
    currentState = state;
    bus.publishNmtState(state);
}

void NMT::setTransition(NMTServiceCommands command) {
    for (const Transition &transition : TRANSITIONS) {
        if (transition.command != command || transition.from != currentState)
            continue;
        if (transition.to == NMTState_Initialisation) {
            reset(command == NMTServiceCommand_ResetCommunication
                      ? ParameterGroup_Communication
                      : ParameterGroup_All);
        } else {
            enter(transition.to);
        }
        return;
    }
}

void NMT::receiveFrame(const Frame &frame) {
    const nmt::Command command(frame);
    if (!command.isWellFormed() || !command.addresses(nodeId)) return;
    setTransition(static_cast<NMTServiceCommands>(command.command()));
}
