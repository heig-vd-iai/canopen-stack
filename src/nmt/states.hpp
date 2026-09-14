#pragma once

namespace CANopen {

/** NMT states as coded in the heartbeat and boot-up messages.
 * CiA301:2011§7.3.2.2 */
enum NMTStates {
    NMTState_Initialisation = 0x00,
    NMTState_PreOperational = 0x7F,
    NMTState_Operational = 0x05,
    NMTState_Stopped = 0x04
};

/** NMT command specifiers. CiA301:2011§7.2.8.3.1 */
enum NMTServiceCommands {
    NMTServiceCommand_None = 0x00,
    NMTServiceCommand_Start = 0x01,
    NMTServiceCommand_Stop = 0x02,
    NMTServiceCommand_EnterPreOperational = 0x80,
    NMTServiceCommand_ResetNode = 0x81,
    NMTServiceCommand_ResetCommunication = 0x82
};
}  // namespace CANopen
