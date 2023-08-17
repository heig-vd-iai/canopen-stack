#pragma once
#include <cstdint>
#define CAN_DATA_LENGTH 8
#define FUNCTION_OFFSET 7
#define COBID_MASK 0x7FF
#define FUNCTION_MASK 0xF
#define NODEID_MASK 0x7F

namespace CANopen
{
    struct Frame
    {
        uint8_t nodeId;
        uint8_t functionCode;
        uint8_t data[CAN_DATA_LENGTH] = {0};
        uint8_t dlc = 0;
        bool rtr = false;
        Frame(uint8_t nodeId, uint8_t functionCode);
        Frame(uint16_t cobId);
        uint16_t getCobID();
        void setCobID(uint16_t cobId);
    };

    struct HeartbeatFrame : public Frame
    {
        HeartbeatFrame(uint8_t nodeId, uint8_t state);
        void setNMTState(uint8_t state);
    };

    struct EmergencyFrame : public Frame
    {
        EmergencyFrame(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister, uint32_t manufacturerCode);
        void setErrorCode(uint16_t errorCode);
        void setErrorRegister(uint8_t errorRegister);
        void setManufacturerCode(uint32_t manufacturerCode);
    };

    struct SDOFrameBase : public Frame
    {
        SDOFrameBase(uint8_t nodeId);
        SDOFrameBase(uint8_t nodeId, uint8_t commandByte);
        void setCommandByte(uint8_t commandByte);
        uint8_t getCommandByte();
        void setIndex(uint16_t index);
        uint16_t getIndex();
        void setSubindex(uint8_t subindex);
        uint8_t getSubindex();
        void setAbortCode(uint32_t abortCode);
        uint32_t getAbortCode();
    };

    struct SDOFrame : public SDOFrameBase
    {
        SDOFrame(uint8_t nodeId);
        SDOFrame(uint8_t nodeId, uint8_t commandByte);
        void setInitiateData(uint32_t initiateData);
        uint32_t getInitiateData();
    };

    struct SDOBlockFrame : public SDOFrameBase
    { // TODO: remove unused methods
        SDOBlockFrame(uint8_t nodeId);
        SDOBlockFrame(uint8_t nodeId, uint8_t commandByte);
        void setSize(uint32_t size);
        uint32_t getSize();
        void setInitiateBlockSize(uint8_t blockSize);
        uint8_t getInitiateBlockSize();
        void setSubBlockSize(uint8_t blockSize);
        uint8_t getSubBlockSize();
        void setCRC(uint16_t crc);
        uint16_t getCRC();
        void setAckseq(uint8_t ackseq);
        uint8_t getAckseq();
        uint8_t getPST();
    };

    struct NMTFrame : public Frame
    {
        NMTFrame(uint8_t nodeId);
        uint8_t getCommand();
        uint8_t getTargetId();
    };

    struct SYNCFrame : public Frame
    {
        SYNCFrame(uint8_t nodeId);
        bool isCounter();
        uint8_t getCounter();
    };
}
