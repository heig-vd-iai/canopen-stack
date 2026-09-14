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
 * This is a specialized Frame used by the EMCY class.
 * See CiA301:2011§7.2.7.3.1 (p. 68)
 */
struct EmergencyFrame : public Frame {
    /**
     * Constructor for specialized CANopen emergency frame.
     * @param nodeId Node ID.
     * @param errorCode Error code, should be a value from EMCYErrorCodes enum.
     * @param errorRegister Error register value, should come from object
     * 0x1001.
     * @param manufacturerCode Manufacturer-specific error code.
     */
    EmergencyFrame(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister,
                   uint32_t manufacturerCode);

    /**
     * Set the error code.
     * @param errorCode Error code to set.
     */
    void setErrorCode(uint16_t errorCode);

    /**
     * Set the error register value.
     * @param errorRegister Error register to set.
     */
    void setErrorRegister(uint8_t errorRegister);

    /**
     * Set the manufacturer-specific error code.
     * @param manufacturerCode Manufacturer-specific error code to set.
     */
    void setManufacturerCode(uint32_t manufacturerCode);
};

/**
 * This is a specialized Frame used by the NMT class.
 * CiA301:2011§7.2.8.3.1 (p. 72)
 */
struct NMTFrame : public Frame {
    /**
     * Constructor for specialized CANopen NMT frame.
     * @param nodeId Node ID.
     */
    NMTFrame(uint8_t nodeId);

    /**
     * Get the NMT command.
     * @return The NMT command.
     */
    uint8_t getCommand() const;

    /**
     * Get the target node ID for the command (0 if broadcasting).
     * @return The target node ID.
     */
    uint8_t getTargetId() const;
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
