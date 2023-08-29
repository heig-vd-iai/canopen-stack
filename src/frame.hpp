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
     * @brief This class is the generic CANopen frame that is used to send and receive messages.
     * It has to be converted to a CAN frame before it can be sent on the bus.
     */
    struct Frame
    {
        uint8_t nodeId;
        uint8_t functionCode;
        uint8_t data[CAN_DATA_LENGTH] = {0};
        uint8_t dlc = 0;
        bool rtr = false;

        /**
         * @brief Constructor for a generic CANopen frame from node ID and function code.
         * @param nodeId %Node ID.
         * @param functionCode Function code.
         */
        Frame(uint8_t nodeId, uint8_t functionCode);

        /**
         * @brief Constructor for a generic CANopen frame from COB-ID.
         * @param cobId COB-ID of the message.
         */
        Frame(uint16_t cobId);

        /**
         * @brief Get the CANopen COB-ID.
         * @return The COB-ID value.
         */
        uint16_t getCobID();

        /**
         * @brief Set the CANopen COB-ID.
         * @param cobId The COB-ID to set.
         */
        void setCobID(uint16_t cobId);
    };

    /**
     * @brief This is a specialized Frame used by the HB object.
     * See p. 76 of CIA301 for more details.
     */
    struct HeartbeatFrame : public Frame
    {
        /**
         * @brief Constructor for specialized CANopen heartbeat frame.
         * @param nodeId %Node ID.
         * @param state Current NMT state.
         */
        HeartbeatFrame(uint8_t nodeId, uint8_t state);

        /**
         * @brief Set the NMT state byte (byte 0).
         * @param state Current NMT state to set.
         */
        void setNMTState(uint8_t state);
    };

    /**
     * @brief This is a specialized Frame used by the EMCY object.
     * See p. 68 of CIA301 for more details.
     */
    struct EmergencyFrame : public Frame
    {
        /**
         * @brief Constructor for specialized CANopen emergency frame.
         * @param nodeId %Node ID.
         * @param errorCode Error code.
         * @param errorRegister Error register value.
         * @param manufacturerCode Manufacturer-specific error code.
         */
        EmergencyFrame(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister, uint32_t manufacturerCode);

        /**
         * @brief Set the error code.
         * @param errorCode Error code to set.
         */
        void setErrorCode(uint16_t errorCode);

        /**
         * @brief Set the error register value.
         * @param errorRegister Error register to set.
         */
        void setErrorRegister(uint8_t errorRegister);

        /**
         * @brief Set the manufacturer-specific error code.
         * @param manufacturerCode Manufacturer-specific error code to set.
         */
        void setManufacturerCode(uint32_t manufacturerCode);
    };

    /**
     * @brief This is a specialized Frame used by the SDO object.
     * It is the base class for SDOFrame and SDOBlockFrame.
     * It contains common accessors for both its subclasses.
     * See p. 48 of CIA301 for more details.
     */
    struct SDOFrameBase : public Frame
    {
        /**
         * @brief Constructor for specialized CANopen SDO base frame.
         * @param nodeId %Node ID.
         */
        SDOFrameBase(uint8_t nodeId);

        /**
         * @brief Constructor for specialized CANopen SDO base frame from node ID and SDO command byte.
         * @param nodeId %Node ID.
         * @param commandByte SDO command byte.
         */
        SDOFrameBase(uint8_t nodeId, uint8_t commandByte);

        /**
         * @brief Set the command byte.
         * @param commandByte Command byte to set.
         */
        void setCommandByte(uint8_t commandByte);

        /**
         * @brief Get the command byte.
         * @return The command byte.
         */
        uint8_t getCommandByte();

        /**
         * @brief Set the index of the target dictionnary Object.
         * @param index Index to set.
         */
        void setIndex(uint16_t index);

        /**
         * @brief Get the index of the target dictionnary Object.
         * @return The index.
         */
        uint16_t getIndex();

        /**
         * @brief Set the subindex of the target dictionnary Object.
         * @param subindex Subindex to set.
         */
        void setSubindex(uint8_t subindex);

        /**
         * @brief Get the subindex of the target dictionnary Object.
         * @return The subindex.
         */
        uint8_t getSubindex();

        /**
         * @brief Set the abort code.
         * @param abortCode Abort code to set.
         */
        void setAbortCode(uint32_t abortCode);

        /**
         * @brief Get the abort code.
         * @return The abort code.
         */
        uint32_t getAbortCode();
    };

    /**
     * @brief This is a specialized Frame used by the SDO object when dealing with SDO transactions.
     * See p. 48 of CIA301 for more details.
     */
    struct SDOFrame : public SDOFrameBase
    {
        /**
         * @brief Constructor for specialized CANopen SDO frame.
         * @param nodeId %Node ID.
         */
        SDOFrame(uint8_t nodeId);

        /**
         * @brief Constructor for specialized CANopen SDO frame from node ID and command byte.
         * @param nodeId %Node ID.
         * @param commandByte Command byte.
         */
        SDOFrame(uint8_t nodeId, uint8_t commandByte);

        /**
         * @brief Set the data (d) field (bytes 4 to 7) to an initiate frame.
         * @param initiateData Initiate data to set.
         */
        void setInitiateData(uint32_t initiateData);

        /**
         * @brief Get the data (d) field (bytes 4 to 7) from an initiate frame.
         * @return The initiate data.
         */
        uint32_t getInitiateData();
    };

    /**
     * @brief This is a specialized Frame used by the SDO object when dealing with SDO block transactions.
     * See p. 53 of CIA301 for more details.
     */
    struct SDOBlockFrame : public SDOFrameBase
    {
        /**
         * @brief Constructor for specialized CANopen SDO block frame.
         * @param nodeId %Node ID.
         */
        SDOBlockFrame(uint8_t nodeId);

        /**
         * @brief Constructor for specialized CANopen SDO block frame from node ID and command byte.
         * @param nodeId %Node ID.
         * @param commandByte Command byte.
         */
        SDOBlockFrame(uint8_t nodeId, uint8_t commandByte);

        /**
         * @brief Set the size field (bytes 4 to 7) to a block initiate frame.
         * @param size Size to set.
         */
        void setSize(uint32_t size);

        /**
         * @brief Get the size field (bytes 4 to 7) from a block initiate frame.
         * @return The size.
         */
        uint32_t getSize();

        /**
         * @brief Set the block size (blksize) field (byte 4) to a block initiate frame.
         * @param blockSize Block size to set.
         */
        void setInitiateBlockSize(uint8_t blockSize);

        /**
         * @brief Get the block size (blksize) field (byte 4) from a block initiate frame.
         * @return The block size.
         */
        uint8_t getInitiateBlockSize();

        /**
         * @brief Set the block size (blksize) field (byte 2) to a block download/upload sub-block frame.
         * @param blockSize Block size to set.
         */
        void setSubBlockSize(uint8_t blockSize);

        /**
         * @brief Get the block size (blksize) field (byte 2) from a block download/upload sub-block frame.
         * @return The block size.
         */
        uint8_t getSubBlockSize();

        /**
         * @brief Set the CRC field (bytes 1 to 2) to a block end frame.
         * @param crc CRC value to set.
         */
        void setCRC(uint16_t crc);

        /**
         * @brief Get the CRC field (bytes 1 to 2) from a block end frame.
         * @return The CRC value.
         */
        uint16_t getCRC();

        /**
         * @brief Set the last sequence number (ackseq) field (byte 1) to a block download/upload sub-block frame.
         * @param ackseq Ackseq value to set.
         */
        void setAckseq(uint8_t ackseq);

        /**
         * @brief Get the last sequence number (ackseq) field (byte 1) from a block download/upload sub-block frame.
         * @return The ackseq value.
         */
        uint8_t getAckseq();

        /**
         * @brief Get the protocol switch threshold (pst) field (byte 5) from a block upload initiate frame.
         * @return The pst value.
         */
        uint8_t getPST();
    };

    /**
     * @brief This is a specialized Frame used by the NMT object.
     * See p. 72 of CIA301 for more details.
     */
    struct NMTFrame : public Frame
    {
        /**
         * @brief Constructor for specialized CANopen NMT frame.
         * @param nodeId %Node ID.
         */
        NMTFrame(uint8_t nodeId);

        /**
         * @brief Get the NMT command.
         * @return The NMT command.
         */
        uint8_t getCommand();

        /**
         * @brief Get the target node ID for the command (0 if broadcasting).
         * @return The target node ID.
         */
        uint8_t getTargetId();
    };

    /**
     * @brief This is a specialized Frame used by the SYNC object.
     * See p. 63 of CIA301 for more details.
     */
    struct SYNCFrame : public Frame
    {
        /**
         * @brief Constructor for specialized CANopen SYNC frame.
         * @param nodeId %Node ID.
         */
        SYNCFrame(uint8_t nodeId);

        /**
         * @brief Check if the optionnal counter value is present in the message.
         * @return True if the counter is set, false otherwise.
         */
        bool isCounter();

        /**
         * @brief Get the counter value.
         * @return The counter value.
         */
        uint8_t getCounter();
    };
}
