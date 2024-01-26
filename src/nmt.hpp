/**
 * Contains the declaration of the NMT class.
 */
#pragma once
#include "enums.hpp"
#include <cstdint>
#define NMT_COMMAND_OFFSET 0
#define NMT_NODEID_OFFSET 1

namespace CANopen
{
/**
 * NMT object.
 * It handles the reception of NMT commands, as well as the management of the NMT state machine.
 * See CiA301:2011§7.2.8 (p. 68)
 */
class NMT
{
private:
    NMTStates currentState = NMTState_Initialisation;
    NMTResetStates resetState = NMTResetState_Initialising;

    /**
     * Initialize the NMT state machine.
     * Must be called in order to initially switch from Initialisation to Pre-Operational.
     */
    void initSM();

    /**
     * Update the NMT state machine.
     * The state machine updating is event-driven, not periodic.
     * As such, this internal method is only called when a transition is issued (besides initialization).
     * @param command NMT service command (default: None).
     */
    void updateSM(NMTServiceCommands command = NMTServiceCommand_None);

    /**
     * Receive and process an NMT frame.
     * @param frame NMTFrame to be processed.
     */
    void receiveFrame(class NMTFrame &frame);

public:
    friend class Node;

    /**
     * Issue a state machine transition based on the given command.
     * @param command NMT service command.
     */
    void setTransition(NMTServiceCommands command);

    /**
     * Get the current state of the state machine.
     * @return The current NMT state.
     */
    NMTStates getState();
};
}
