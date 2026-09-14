/**
 * Contains the declaration of the basic, generic CANopen frame, as well as all
 * specialized subclasses.
 */
#pragma once
#include <cstdint>

#define CAN_DATA_LENGTH 8
#define FUNCTION_OFFSET 7
#define COBID_MASK 0x7FF
#define FUNCTION_MASK 0xF
#define NODEID_MASK 0x7F

namespace CANopen {
/**
 * CANopen generic frame that is used to send and receive messages.
 * It has to be converted to a CAN frame before it can be sent on the bus.
 */
struct Frame {
    uint8_t nodeId = 0;
    uint8_t functionCode = 0;
    uint8_t data[CAN_DATA_LENGTH] = {0};  // TODO: change to Data type ?
    uint8_t dlc = 0;                      // Data length
    bool rtr = false;                     // Remote transmission request

    /**
     * Constructor for a generic CANopen frame from node ID and function code.
     * @param nodeId Node ID.
     * @param functionCode Function code.
     */
    Frame(uint8_t nodeId = 0, uint8_t functionCode = 0);

    /**
     * Factory method for creating a generic CANopen frame from COB-ID.
     * @param cobId COB-ID of the message.
     */
    static Frame fromCobId(uint16_t cobId);

    /**
     * Get the CANopen COB-ID.
     * @return The COB-ID value.
     */
    uint16_t getCobID() const;
};

}  // namespace CANopen
