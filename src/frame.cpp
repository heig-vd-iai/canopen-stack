/******************************************************************************
 * [Filename]:      frame.cpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the definitions of all of the frame classes.
 *****************************************************************************/
#include "frame.hpp"
#include "emergency.hpp"
#include "heartbeat.hpp"
#include "nmt.hpp"
#include "sdo.hpp"
#include "sync.hpp"
using namespace CANopen;

Frame::Frame(uint8_t nodeId, uint8_t functionCode) : nodeId(nodeId), functionCode(functionCode) {}

Frame::Frame(uint16_t cobId)
{
    setCobID(cobId);
}

uint16_t Frame::getCobID()
{
    return ((uint16_t)functionCode & FUNCTION_MASK) << FUNCTION_OFFSET | (nodeId & NODEID_MASK);
}

void Frame::setCobID(uint16_t cobId)
{
    nodeId = cobId & NODEID_MASK;
    functionCode = (cobId >> FUNCTION_OFFSET) & FUNCTION_MASK;
}

HeartbeatFrame::HeartbeatFrame(uint8_t nodeId, uint8_t state) : Frame(nodeId, FunctionCode_HEARTBEAT)
{
    dlc = HEARTBEAT_DLC;
    setNMTState(state);
}

void HeartbeatFrame::setNMTState(uint8_t state)
{
    data[HEARTBEAT_STATE_OFFSET] = state;
}

EmergencyFrame::EmergencyFrame(uint8_t nodeId, uint16_t errorCode, uint8_t errorRegister, uint32_t manufacturerCode) : Frame(nodeId, FunctionCode_EMCY)
{
    dlc = EMCY_DLC;
    setErrorCode(errorCode);
    setErrorRegister(errorRegister);
    setManufacturerCode(manufacturerCode);
}

void EmergencyFrame::setErrorCode(uint16_t errorCode)
{
    *(uint16_t *)(data + EMCY_ERRCODE_OFFSET) = errorCode;
}

void EmergencyFrame::setErrorRegister(uint8_t errorRegister)
{
    data[EMCY_ERRREG_OFFSET] = errorRegister;
}

void EmergencyFrame::setManufacturerCode(uint32_t manufacturerCode)
{
    *(uint32_t *)(data + EMCY_MANUFACTURER_OFFSET) = manufacturerCode;
}

SDOFrameBase::SDOFrameBase(uint8_t nodeId) : Frame(nodeId, FunctionCode_TSDO)
{
    dlc = SDO_DLC;
}

SDOFrameBase::SDOFrameBase(uint8_t nodeId, uint8_t commandByte) : Frame(nodeId, FunctionCode_TSDO)
{
    dlc = SDO_DLC;
    setCommandByte(commandByte);
}

void SDOFrameBase::setCommandByte(uint8_t commandByte)
{
    data[SDO_COMMANDBYTE_OFFSET] = commandByte;
}

uint8_t SDOFrameBase::getCommandByte()
{
    return data[SDO_COMMANDBYTE_OFFSET];
}

void SDOFrameBase::setIndex(uint16_t index)
{
    *(uint16_t *)(data + SDO_INDEX_OFFSET) = index;
}

uint16_t SDOFrameBase::getIndex()
{
    return *(uint16_t *)(data + SDO_INDEX_OFFSET);
}

void SDOFrameBase::setSubindex(uint8_t subindex)
{
    data[SDO_SUBINDEX_OFFSET] = subindex;
}

uint8_t SDOFrameBase::getSubindex()
{
    return data[SDO_SUBINDEX_OFFSET];
}

void SDOFrameBase::setAbortCode(uint32_t abortCode)
{
    *(uint32_t *)(data + SDO_ABORTCODE_OFFSET) = abortCode;
}

uint32_t SDOFrameBase::getAbortCode()
{
    return *(uint32_t *)(data + SDO_ABORTCODE_OFFSET);
}

SDOFrame::SDOFrame(uint8_t nodeId) : SDOFrameBase(nodeId) {}

SDOFrame::SDOFrame(uint8_t nodeId, uint8_t commandByte) : SDOFrameBase(nodeId, commandByte) {}

void SDOFrame::setInitiateData(uint32_t initiateData)
{
    *(uint32_t *)(data + SDO_INITIATE_DATA_OFFSET) = initiateData;
}

uint32_t SDOFrame::getInitiateData()
{
    return *(uint32_t *)(data + SDO_INITIATE_DATA_OFFSET);
}

SDOBlockFrame::SDOBlockFrame(uint8_t nodeId) : SDOFrameBase(nodeId) {}

SDOBlockFrame::SDOBlockFrame(uint8_t nodeId, uint8_t commandByte) : SDOFrameBase(nodeId, commandByte) {}

void SDOBlockFrame::setSize(uint32_t size)
{
    *(uint32_t *)(data + SDO_BLOCK_SIZE_OFFSET) = size;
}

uint32_t SDOBlockFrame::getSize()
{
    return *(uint32_t *)(data + SDO_BLOCK_SIZE_OFFSET);
}

void SDOBlockFrame::setInitiateBlockSize(uint8_t blockSize)
{
    data[SDO_BLOCK_INIT_BLKSIZE_OFFSET] = blockSize;
}

uint8_t SDOBlockFrame::getInitiateBlockSize()
{
    return data[SDO_BLOCK_INIT_BLKSIZE_OFFSET];
}

void SDOBlockFrame::setSubBlockSize(uint8_t blockSize)
{
    data[SDO_BLOCK_SUB_BLKSIZE_OFFSET] = blockSize;
}

uint8_t SDOBlockFrame::getSubBlockSize()
{
    return data[SDO_BLOCK_SUB_BLKSIZE_OFFSET];
}

void SDOBlockFrame::setCRC(uint16_t crc)
{
    *(uint16_t *)(data + SDO_BLOCK_CRC_OFFSET) = crc;
}

uint16_t SDOBlockFrame::getCRC()
{
    return *(uint16_t *)(data + SDO_BLOCK_CRC_OFFSET);
}

void SDOBlockFrame::setAckseq(uint8_t ackseq)
{
    data[SDO_BLOCK_ACKSEQ_OFFSET] = ackseq;
}

uint8_t SDOBlockFrame::getAckseq()
{
    return data[SDO_BLOCK_ACKSEQ_OFFSET];
}

uint8_t SDOBlockFrame::getPST()
{
    return data[SDO_BLOCK_PST_OFFSET];
}

NMTFrame::NMTFrame(uint8_t nodeId) : Frame(nodeId, FunctionCode_NMT) {}

uint8_t NMTFrame::getCommand()
{
    return data[NMT_COMMAND_OFFSET];
}

uint8_t NMTFrame::getTargetId()
{
    return data[NMT_NODEID_OFFSET];
}

SYNCFrame::SYNCFrame(uint8_t nodeId) : Frame(nodeId, FunctionCode_SYNC) {}

bool SYNCFrame::isCounter()
{
    return dlc > 0;
}

uint8_t SYNCFrame::getCounter()
{
    return data[SYNC_COUNTER_OFFSET];
}
