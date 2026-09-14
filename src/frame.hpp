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

/**
 * This is a specialized Frame used by the HB class.
 * See CiA301:2011§7.2.8.3.2.2 (p. 76)
 */
struct HeartbeatFrame : public Frame {
    /**
     * Constructor for specialized CANopen heartbeat frame.
     * @param nodeId Node ID.
     * @param state Current NMT state.
     */
    HeartbeatFrame(uint8_t nodeId, uint8_t state);

    /**
     * Set the NMT state byte (byte 0).
     * @param state Current NMT state to set.
     */
    void setNMTState(uint8_t state);
};

/**
 * This is a specialized Frame used by the SYNC class.
 * CiA301:2011§7.2.5.3.1 (p. 63)
 */
struct SYNCFrame : public Frame {
    /**
     * Constructor for specialized CANopen SYNC frame.
     * @param nodeId Node ID.
     */
    SYNCFrame(uint8_t nodeId);

    /**
     * Check if the optionnal counter value is present in the message.
     * @return True if the counter is set, false otherwise.
     */
    bool isCounter() const;

    /**
     * Get the counter value.
     * @return The counter value.
     */
    uint8_t getCounter() const;
};
}  // namespace CANopen
