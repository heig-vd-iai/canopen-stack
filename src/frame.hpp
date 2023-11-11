/**
 * Contains the declaration of all of the frame classes.
 * TODO: Title isn't very descriptive. Below I see a class Frame. So
 * it looks like I don't need this header. Insteal, describe what a Frame is.
 */
#pragma once
#include <cstdint>
#define CAN_DATA_LENGTH 8
#define FUNCTION_OFFSET 7
#define COBID_MASK 0x7FF
#define FUNCTION_MASK 0xF
#define NODEID_MASK 0x7F

// TODO: I do not like indenting namespaces. It makes the code harder to read.
// https://github.com/sass/libsass/issues/2917 to see why. Modify .clang-format accordingly
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
        uint8_t dlc = 0;
        bool rtr = false;

        /**
         * Default constructor for a generic CANopen frame.
         */
        // TODO: No implementation in header files. You could use a default NodeId value in the nodeId constructor
        Frame(){};

        /**
         * Constructor for a generic CANopen frame from node ID.
         * @param nodeId Node ID.
         */
        Frame(uint8_t nodeId);

        /**
         * Constructor for a generic CANopen frame from node ID and function code.
         * @param nodeId Node ID.
         * @param functionCode Function code.
         */
        // TODO: functionCode could be optional, then one constructor instead of three.
        Frame(uint8_t nodeId, uint8_t functionCode);

        /**
         * Constructor for a generic CANopen frame from COB-ID.
         * @param cobId COB-ID of the message.
         */
        // TODO: Very confusing. I would instead use a factory method to create a Frame from a COB-ID.
        // auto frame = Frame::fromCobId(cobId);
        Frame(uint16_t cobId);

        /**
         * Get the CANopen COB-ID.
         * @return The COB-ID value.
         */
        // TODO This one doesn't modify the instance : const
        uint16_t getCobID() const;

        /**
         * Set the CANopen COB-ID.
         * @param cobId The COB-ID to set.
         */
        void setCobID(uint16_t cobId);
    };

    /**
     * This is a specialized Frame used by the HB class.
     * See p. 76 of CIA301 for more details.
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
     * See p. 68 of CIA301 for more details.
     */
    struct EmergencyFrame : public Frame
    {
        /**
         * Constructor for specialized CANopen emergency frame.
         * TODO: Very useless documentation. I do not have more information here :(
         * @param nodeId Node ID.
         * @param errorCode Error code.
         * @param errorRegister Error register value.
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
     * See p. 48 of CIA301 for more details.
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
        // TODO: Const
        uint8_t getCommandByte();

        /**
         * Set the index of the target dictionnary Object.
         * @param index Index to set.
         */
        void setIndex(uint16_t index);

        /**
         * Get the index of the target dictionnary Object.
         * @return The index.
         */
        // TODO: Const
        uint16_t getIndex();

        /**
         * Set the subindex of the target dictionnary Object.
         * @param subindex Subindex to set.
         */
        void setSubindex(uint8_t subindex);

        /**
         * Get the subindex of the target dictionnary Object.
         * @return The subindex.
         */
        uint8_t getSubindex();

        /**
         * Set the abort code.
         * @param abortCode Abort code to set.
         */
        void setAbortCode(uint32_t abortCode);

        /**
         * Get the abort code.
         * @return The abort code.
         */
        uint32_t getAbortCode();
    };

    /**
     * This is a specialized Frame used by the SDO class when dealing with SDO transactions.
     * See p. 48 of CIA301 for more details.
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
        uint32_t getInitiateData();
    };

    /**
     * This is a specialized Frame used by the SDO class when dealing with SDO block transactions.
     * See p. 53 of CIA301 for more details.
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
         * @param size Size to set.
         */
        void setSize(uint32_t size);

        /**
         * Get the size field (bytes 4 to 7) from a block initiate frame.
         * @return The size.
         */
        uint32_t getSize();

        /**
         * Set the block size (blksize) field (byte 4) to a block initiate frame.
         * @param blockSize Block size to set.
         */
        void setInitiateBlockSize(uint8_t blockSize);

        /**
         * Get the block size (blksize) field (byte 4) from a block initiate frame.
         * @return The block size.
         */
        uint8_t getInitiateBlockSize();

        /**
         * Set the block size (blksize) field (byte 2) to a block download/upload sub-block frame.
         * @param blockSize Block size to set.
         */
        void setSubBlockSize(uint8_t blockSize);

        /**
         * Get the block size (blksize) field (byte 2) from a block download/upload sub-block frame.
         * @return The block size.
         */
        uint8_t getSubBlockSize();

        /**
         * Set the CRC field (bytes 1 to 2) to a block end frame.
         * @param crc CRC value to set.
         */
        void setCRC(uint16_t crc);

        /**
         * Get the CRC field (bytes 1 to 2) from a block end frame.
         * @return The CRC value.
         */
        uint16_t getCRC();

        /**
         * Set the last sequence number (ackseq) field (byte 1) to a block download/upload sub-block frame.
         * @param ackseq Ackseq value to set.
         */
        void setAckseq(uint8_t ackseq);

        /**
         * Get the last sequence number (ackseq) field (byte 1) from a block download/upload sub-block frame.
         * @return The ackseq value.
         */
        uint8_t getAckseq();

        /**
         * Get the protocol switch threshold (pst) field (byte 5) from a block upload initiate frame.
         * @return The pst value.
         */
        uint8_t getPST();
    };

    /**
     * This is a specialized Frame used by the NMT class.
     * See p. 72 of CIA301 for more details.
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
        uint8_t getCommand();

        /**
         * Get the target node ID for the command (0 if broadcasting).
         * @return The target node ID.
         */
        uint8_t getTargetId();
    };

    /**
     * This is a specialized Frame used by the SYNC class.
     * See p. 63 of CIA301 for more details.
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
        bool isCounter();

        /**
         * Get the counter value.
         * @return The counter value.
         */
        uint8_t getCounter();
    };
}
