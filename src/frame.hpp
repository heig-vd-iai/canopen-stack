#pragma once
#include <cstdint>
#define CAN_DATA_LENGTH 8
#define COBID_MASK 0x7FF

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

    struct SDOFrame : public Frame // TODO: Really need function code in constructor ?
    {
        SDOFrame(uint8_t nodeId, uint8_t functionCode);
        SDOFrame(uint8_t nodeId, uint8_t functionCode, uint8_t commandByte);
        void setCommandByte(uint8_t commandByte);
        uint8_t getCommandByte();
        void setIndex(uint16_t index);
        uint16_t getIndex();
        void setSubindex(uint8_t subindex);
        uint8_t getSubindex();
        void setAbortCode(uint32_t abortCode);
        uint32_t getAbortCode();
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
