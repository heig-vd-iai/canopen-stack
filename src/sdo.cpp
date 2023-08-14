#include "sdo.hpp"
#include "node.hpp"
#include "frame.hpp"
#include "enums.hpp"
#include "unions.hpp"
#include "object.hpp"
#include <cstring>
using namespace CANopen;

SDO::SDO(Node &node) : node(node) {}

void SDO::enable() { enabled = true; }

void SDO::disable() { enabled = false; }

void SDO::sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode)
{
    SDOCommandByte sendCommand = {0};
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_AbortTransfer;
    SDOFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    response.setAbortCode(abortCode);
    node.sendFrame(response);
    serverState = SDOServerState_Ready;
}

void SDO::uploadInitiate(SDOFrame &request, uint32_t timestamp_us)
{
    SDOCommandByte sendCommand = {0};
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    Object *object = node.od.findObject(index);
    if (!object)
    {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (!object->isSubValid(subindex))
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->getAccessType(subindex).bits.readable)
    {
        sendAbort(index, subindex, SDOAbortCode_AttemptReadOnWriteOnly);
        return;
    }
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.object = object;
    uint32_t size = object->getSize(subindex);
    transferData.remainingBytes = size;
    transferData.toggle = 0;
    SDOFrame response(node.nodeId, FunctionCode_TSDO);
    if (size > SDO_INITIATE_DATA_LENGTH) // Segment transfer
    {
        sendCommand.bits_initiate.e = 0;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = 0;
        memcpy(response.data + SDO_INITIATE_DATA_OFFSET, &size, sizeof(size));
    }
    else // Expedited transfer
    {
        sendCommand.bits_initiate.e = 1;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = SDO_INITIATE_DATA_LENGTH - size;
        uint32_t abortCode;
        if ((abortCode = object->readBytes(subindex, response.data + SDO_INITIATE_DATA_OFFSET, size, 0)) != SDOAbortCode_OK)
        {
            sendAbort(index, subindex, abortCode);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseUploadInitiate;
    response.setCommandByte(sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    node.sendFrame(response);
    if (!sendCommand.bits_initiate.e)
        serverState = SDOServerState_Uploading;
    transferData.timestamp_us = timestamp_us;
}

void SDO::uploadSegment(SDOFrame &request, uint32_t timestamp_us)
{
    SDOCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    if (transferData.toggle != recvCommand.bits_segment.t)
    {
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.bits_segment.t;
    unsigned payloadSize = transferData.remainingBytes > SDO_SEGMENT_DATA_LENGTH ? SDO_SEGMENT_DATA_LENGTH : transferData.remainingBytes;
    unsigned bytesSent = transferData.object->getSize(transferData.subindex) - transferData.remainingBytes;
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseUploadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    sendCommand.bits_segment.n = SDO_SEGMENT_DATA_LENGTH - payloadSize;
    sendCommand.bits_segment.c = !transferData.remainingBytes;
    SDOFrame response(node.nodeId, sendCommand.value);
    uint32_t abortCode;
    if ((abortCode = transferData.object->readBytes(transferData.subindex, response.data + SDO_SEGMENT_DATA_OFFSET, payloadSize, bytesSent)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.object->index, transferData.subindex, abortCode);
        return;
    }
    transferData.remainingBytes -= payloadSize;
    node.sendFrame(response);
    if (sendCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::downloadInitiate(SDOFrame &request, uint32_t timestamp_us)
{
    SDOCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    Object *object = node.od.findObject(index);
    if (!object)
    {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (!object->isSubValid(subindex))
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->getAccessType(subindex).bits.writeable)
    {
        sendAbort(index, subindex, SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.object = object;
    uint32_t size = object->getSize(subindex);
    transferData.toggle = 0;
    if (recvCommand.bits_initiate.e) // Expedited transfer
    {
        transferData.remainingBytes = recvCommand.bits_initiate.s ? SDO_INITIATE_DATA_LENGTH - recvCommand.bits_initiate.n : size;
        if (transferData.remainingBytes > SDO_INITIATE_DATA_LENGTH)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
            return;
        }
        uint32_t abortCode;
        if ((abortCode = object->writeBytes(subindex, request.data + SDO_INITIATE_DATA_OFFSET, transferData.remainingBytes, node)) != SDOAbortCode_OK)
        {
            sendAbort(index, subindex, abortCode);
            return;
        }
    }
    else // Segment transfer
    {
        if (!recvCommand.bits_initiate.s)
        {
            sendAbort(index, subindex, SDOAbortCode_GeneralParameterIncompatibility);
            return;
        }
        transferData.remainingBytes = request.getInitiateData();
        if (transferData.remainingBytes != size)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterMismatch);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseDownloadInitiate;
    SDOFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    node.sendFrame(response);
    if (!recvCommand.bits_initiate.e)
        serverState = SDOServerState_Downloading;
    transferData.timestamp_us = timestamp_us;
}

void SDO::downloadSegment(SDOFrame &request, uint32_t timestamp_us)
{
    SDOCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    unsigned size = transferData.object->getSize(transferData.subindex);
    unsigned payloadSize = SDO_SEGMENT_DATA_LENGTH - recvCommand.bits_segment.n;
    unsigned bytesReceived = size - transferData.remainingBytes;
    if (transferData.toggle != recvCommand.bits_segment.t)
    {
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.bits_segment.t;
    if (bytesReceived + payloadSize > size)
    {
        sendAbort(transferData.object->index, transferData.subindex, SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
        return;
    }
    // Download request data
    if (size > sizeof(transferData.buffer)) // Not enough space in buffer, abort
    {
        sendAbort(transferData.object->index, transferData.subindex, SDOAbortCode_OutOfMemory);
        return;
    }
    memcpy(transferData.buffer + bytesReceived, request.data + SDO_SEGMENT_DATA_OFFSET, payloadSize);
    uint32_t abortCode;
    if (recvCommand.bits_segment.c && (abortCode = transferData.object->writeBytes(transferData.subindex, transferData.buffer, size, node)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.object->index, transferData.subindex, abortCode);
        return;
    }
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseDownloadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    SDOFrame response(node.nodeId, sendCommand.value);
    transferData.remainingBytes -= payloadSize;
    node.sendFrame(response);
    if (recvCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::receiveFrame(SDOFrame &frame, uint32_t timestamp_us)
{
    if (!enabled || frame.nodeId != node.nodeId)
        return;
    SDOCommandByte recvCommand = {frame.getCommandByte()};
    uint16_t index = frame.getIndex();
    uint8_t subindex = frame.getSubindex();
    switch (serverState)
    {
    case SDOServerState_Ready:
        switch (recvCommand.bits_initiate.ccs)
        {
        case SDOCommandSpecifier_RequestUploadInitiate:
            uploadInitiate(frame, timestamp_us);
            break;
        case SDOCommandSpecifier_RequestDownloadInitiate:
            downloadInitiate(frame, timestamp_us);
            break;
        default:
            sendAbort(index, subindex, SDOAbortCode_CommandSpecifierInvalid);
            break;
        }
        break;
    case SDOServerState_Uploading:
        switch (recvCommand.bits_segment.ccs)
        {
        case SDOCommandSpecifier_RequestUploadSegment:
            uploadSegment(frame, timestamp_us);
            break;
        case SDOCommandSpecifier_AbortTransfer:
            serverState = SDOServerState_Ready;
            break;
        default:
            sendAbort(index, subindex, SDOAbortCode_CommandSpecifierInvalid);
            break;
        }
        break;
    case SDOServerState_Downloading:
        switch (recvCommand.bits_segment.ccs)
        {
        case SDOCommandSpecifier_RequestDownloadSegment:
            downloadSegment(frame, timestamp_us);
            break;
        case SDOCommandSpecifier_AbortTransfer:
            serverState = SDOServerState_Ready;
            break;
        default:
            sendAbort(index, subindex, SDOAbortCode_CommandSpecifierInvalid);
            break;
        }
        break;
    }
}

void SDO::update(uint32_t timestamp_us)
{
    if (!enabled)
        return;
    switch (serverState)
    {
    case SDOServerState_Uploading:
    case SDOServerState_Downloading:
        if (timestamp_us - transferData.timestamp_us >= SDO_TIMEOUT_US)
        {
            sendAbort(transferData.object->index, transferData.subindex, SDOAbortCode_TimedOut);
        }
        break;
    default:
        break;
    }
}
