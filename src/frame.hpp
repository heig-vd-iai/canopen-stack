/**
 * Contains the declaration of the basic, generic CANopen frame, as well as all specialized subclasses.
 */
#pragma once
#include <cstdint>
#define CAN_DATA_LENGTH 8
#define FUNCTION_OFFSET 7
#define COBID_MASK 0x7FF
#define FUNCTION_MASK 0xF
#define NODEID_MASK 0x7F

namespace CANopen
{
/**
 * CANopen generic frame that is used to send and receive messages.
 * It has to be converted to a CAN frame before it can be sent on the bus.
 */
struct Frame
{
    uint8_t nodeId = 0;
    uint8_t functionCode = 0;
    uint8_t data[CAN_DATA_LENGTH] = {0};
    uint8_t dlc = 0;  // Data length
    bool rtr = false; // Remote transmission request

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
struct HeartbeatFrame : public Frame
{
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
struct EmergencyFrame : public Frame
{
    /**
     * Constructor for specialized CANopen emergency frame.
     * @param nodeId Node ID.
     * @param errorCode Error code, should be a value from EMCYErrorCodes enum.
     * @param errorRegister Error register value, should come from object 0x1001.
     * @param manufacturerCode Manufacturer-specific error code.
     */
    EmergencyFrame(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister, uint32_t manufacturerCode);

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
 * This is a specialized Frame used by the SDO class.
 * It is the base class for SDOFrame and SDOBlockFrame.
 * It contains common accessors for both its subclasses.
 * CiA301:2011§7.2.4.3 (p. 48)
 */
struct SDOFrameBase : public Frame
{
    /**
     * Constructor for specialized CANopen SDO base frame.
     * @param nodeId Node ID.
     */
    SDOFrameBase(uint8_t nodeId);

    /**
     * Constructor for specialized CANopen SDO base frame from node ID and SDO command byte.
     * @param nodeId Node ID.
     * @param commandByte SDO command byte.
     */
    SDOFrameBase(uint8_t nodeId, uint8_t commandByte);

    /**
     * Set the command byte.
     * @param commandByte Command byte to set.
     */
    void setCommandByte(uint8_t commandByte);

    /**
     * Get the command byte.
     * @return The command byte.
     */
    uint8_t getCommandByte() const;

    /**
     * Set the index of the target dictionnary Object.
     * @param index Index to set.
     */
    void setIndex(uint16_t index);

    /**
     * Get the index of the target dictionnary Object.
     * @return The index.
     */
    uint16_t getIndex() const;

    /**
     * Set the subindex of the target dictionnary Object.
     * @param subindex Subindex to set.
     */
    void setSubindex(uint8_t subindex);

    /**
     * Get the subindex of the target dictionnary Object.
     * @return The subindex.
     */
    uint8_t getSubindex() const;

    /**
     * Set the abort code.
     * @param abortCode Abort code to set.
     */
    void setAbortCode(uint32_t abortCode);

    /**
     * Get the abort code.
     * @return The abort code.
     */
    uint32_t getAbortCode() const;
};

/**
 * This is a specialized Frame used by the SDO class when dealing with SDO transactions.
 *
 * CiA301:2011§7.2.4.3 (p. 48)
 */
struct SDOFrame : public SDOFrameBase
{
    /**
     * Constructor for specialized CANopen SDO frame.
     * @param nodeId Node ID.
     */
    SDOFrame(uint8_t nodeId);

    /**
     * Constructor for specialized CANopen SDO frame from node ID and command byte.
     * @param nodeId Node ID.
     * @param commandByte Command byte.
     */
    SDOFrame(uint8_t nodeId, uint8_t commandByte);

    /**
     * Set the data (d) field (bytes 4 to 7) to an initiate frame.
     * @param initiateData Initiate data to set.
     */
    void setInitiateData(uint32_t initiateData);

    /**
     * Get the data (d) field (bytes 4 to 7) from an initiate frame.
     * @return The initiate data.
     */
    uint32_t getInitiateData() const;
};

/**
 * This is a specialized Frame used by the SDO class when dealing with SDO block transactions.
 * CiA301:2011§7.2.4.3 (p. 53)
 */
struct SDOBlockFrame : public SDOFrameBase
{
    /**
     * Constructor for specialized CANopen SDO block frame.
     * @param nodeId Node ID.
     */
    SDOBlockFrame(uint8_t nodeId);

    /**
     * Constructor for specialized CANopen SDO block frame from node ID and command byte.
     * @param nodeId Node ID.
     * @param commandByte Command byte.
     */
    SDOBlockFrame(uint8_t nodeId, uint8_t commandByte);

    /**
     * Set the size field (bytes 4 to 7) to a block initiate frame.
     * @param sizeBytes Size to set.
     */
    void setSize(uint32_t sizeBytes);

    /**
     * Get the size field (bytes 4 to 7) from a block initiate frame.
     * @return The size.
     */
    uint32_t getSize() const;

    /**
     * Set the block size (blksize) field (byte 4) to a block initiate frame.
     * @param blockSizeBytes Block size to set.
     */
    void setInitiateBlockSize(uint8_t blockSizeBytes);

    /**
     * Get the block size (blksize) field (byte 4) from a block initiate frame.
     * @return The block size.
     */
    uint8_t getInitiateBlockSize() const;

    /**
     * Set the block size (blksize) field (byte 2) to a block download/upload sub-block frame.
     * @param blockSizeBytes Block size to set.
     */
    void setSubBlockSize(uint8_t blockSizeBytes);

    /**
     * Get the block size (blksize) field (byte 2) from a block download/upload sub-block frame.
     * @return The block size in bytes.
     */
    uint8_t getSubBlockSize() const;

    /**
     * Set the CRC field (bytes 1 to 2) to a block end frame.
     * @param crc CRC value to set.
     */
    void setCRC(uint16_t crc);

    /**
     * Get the CRC field (bytes 1 to 2) from a block end frame.
     * @return The CRC value.
     */
    uint16_t getCRC() const;

    /**
     * Set the last sequence number (ackseq) field (byte 1) to a block download/upload sub-block frame.
     * @param ackseq Ackseq value to set.
     */
    void setAckseq(uint8_t ackseq);

    /**
     * Get the last sequence number (ackseq) field (byte 1) from a block download/upload sub-block frame.
     * @return The ackseq value.
     */
    uint8_t getAckseq() const;

    /**
     * Get the protocol switch threshold (pst) field (byte 5) from a block upload initiate frame.
     * @return The pst value.
     */
    uint8_t getPST() const;
};

/**
 * This is a specialized Frame used by the NMT class.
 * CiA301:2011§7.2.8.3.1 (p. 72)
 */
struct NMTFrame : public Frame
{
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
struct SYNCFrame : public Frame
{
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
}
