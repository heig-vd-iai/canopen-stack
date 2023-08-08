#include "sdo.hpp"
#include "node.hpp"
#include "frame.hpp"
#include "enums.hpp"
#include "object.hpp"
#include <cstdio>
#include <cstring>
using namespace CANopen;

SDO::SDO(Node &node) : node(node) {}

void SDO::sendAbort(uint16_t index, uint8_t subindex, uint32_t errorCode)
{
    Frame response;
    SDO_CommandByte sendCommand = {0};
    // Fill command byte
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_AbortTransfer;
    // Fill response frame fields
    response.cobId.bits.functionCode = FunctionCode_TSDO;
    response.cobId.bits.nodeId = node.nodeId;
    response.dlc = 8;
    // Fill response frame data
    response.data[0] = sendCommand.value;
    *(uint16_t *)(response.data + 1) = index;
    response.data[3] = subindex;
    *(uint32_t *)(response.data + 4) = errorCode;
    // Send response
    node.sendFrame(response);
    serverState = SDOServerState_Ready;
}

void SDO::uploadInitiate(Frame request, uint32_t timestamp_us)
{
    Frame response;
    SDO_CommandByte sendCommand = {0};
    uint32_t errorCode = 0;
    uint16_t index = *(uint16_t *)(request.data + 1);
    uint8_t subindex = request.data[3];
    Object *object = node.od.findObject(index);

    if (object == NULL)
    {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (!object->isSubValid(subindex))
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->getAccessType(subindex).bits.r)
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
    // Fill command byte and frame data
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
        if ((errorCode = object->readBytes(subindex, response.data + SDO_INITIATE_DATA_OFFSET, size, 0)) != SDOAbortCode_OK)
        {
            sendAbort(index, subindex, errorCode);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseUploadInitiate;
    // Fill response frame fields
    response.cobId.bits.functionCode = FunctionCode_TSDO;
    response.cobId.bits.nodeId = node.nodeId;
    response.dlc = 8;
    // Fill response frame data
    response.data[0] = sendCommand.value;
    response.data[1] = request.data[1];
    response.data[2] = request.data[2];
    response.data[3] = request.data[3];
    // Send response
    node.sendFrame(response);
    if (!sendCommand.bits_initiate.e)
        serverState = SDOServerState_Uploading;
    transferData.timestamp = timestamp_us;
}

void SDO::uploadSegment(Frame request, uint32_t timestamp_us)
{
    Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint32_t errorCode = 0;
    if (transferData.toggle != recvCommand.bits_segment.t)
    {
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.bits_segment.t;
    unsigned payloadSize = transferData.remainingBytes > SDO_SEGMENT_DATA_LENGTH ? SDO_SEGMENT_DATA_LENGTH : transferData.remainingBytes;
    unsigned bytesSent = transferData.object->getSize(transferData.subindex) - transferData.remainingBytes;
    // Fill command byte
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseUploadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    sendCommand.bits_segment.n = SDO_SEGMENT_DATA_LENGTH - payloadSize;
    sendCommand.bits_segment.c = !transferData.remainingBytes;
    // Fill response frame fields
    response.cobId.bits.functionCode = FunctionCode_TSDO;
    response.cobId.bits.nodeId = node.nodeId;
    response.dlc = 8;
    // Upload response data
    response.data[0] = sendCommand.value;
    if ((errorCode = transferData.object->readBytes(transferData.subindex, response.data + SDO_SEGMENT_DATA_OFFSET, payloadSize, bytesSent)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.object->index, transferData.subindex, errorCode);
        return;
    }
    transferData.remainingBytes -= payloadSize;
    // Send response
    node.sendFrame(response);
    if (sendCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp = timestamp_us;
}

void SDO::downloadInitiate(Frame request, uint32_t timestamp_us)
{
    Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint32_t errorCode = 0;
    uint16_t index = *(uint16_t *)(request.data + 1);
    uint8_t subindex = request.data[3];
    Object *object = node.od.findObject(index);

    if (object == NULL)
    {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (!object->isSubValid(subindex))
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->getAccessType(subindex).bits.w)
    {
        sendAbort(index, subindex, SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }

    transferData.index = index;
    transferData.subindex = subindex;
    transferData.object = object;
    uint32_t size = object->getSize(subindex);
    transferData.toggle = 0;
    // Fill command byte and write data
    if (recvCommand.bits_initiate.e) // Expedited transfer
    {
        transferData.remainingBytes = recvCommand.bits_initiate.s ? SDO_INITIATE_DATA_LENGTH - recvCommand.bits_initiate.n : size;
        if (transferData.remainingBytes > SDO_INITIATE_DATA_LENGTH)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
            return;
        }
        if ((errorCode = object->writeBytes(subindex, request.data + SDO_INITIATE_DATA_OFFSET, transferData.remainingBytes, node)) != SDOAbortCode_OK)
        {
            sendAbort(index, subindex, errorCode);
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
        transferData.remainingBytes = *(uint32_t *)(request.data + SDO_INITIATE_DATA_OFFSET);
        if (transferData.remainingBytes != size)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterMismatch);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseDownloadInitiate;
    // Fill response frame fields
    response.cobId.bits.functionCode = FunctionCode_TSDO;
    response.cobId.bits.nodeId = node.nodeId;
    response.dlc = 8;
    // Fill response frame data
    response.data[0] = sendCommand.value;
    response.data[1] = request.data[1];
    response.data[2] = request.data[2];
    response.data[3] = request.data[3];
    // Send response
    node.sendFrame(response);
    if (!recvCommand.bits_initiate.e)
        serverState = SDOServerState_Downloading;
    transferData.timestamp = timestamp_us;
}

void SDO::downloadSegment(Frame request, uint32_t timestamp_us)
{
    Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint32_t errorCode = 0;
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
    if (recvCommand.bits_segment.c && (errorCode = transferData.object->writeBytes(transferData.subindex, transferData.buffer, size, node)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.object->index, transferData.subindex, errorCode);
        return;
    }

    transferData.remainingBytes -= payloadSize;
    // Fill command byte
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseDownloadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    // Fill response frame fields
    response.cobId.bits.functionCode = FunctionCode_TSDO;
    response.cobId.bits.nodeId = node.nodeId;
    response.dlc = 8;
    // Fill response frame data
    response.data[0] = sendCommand.value;
    // Send response
    node.sendFrame(response);
    if (recvCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp = timestamp_us;
}

void SDO::enable() { enabled = true; }

void SDO::disable() { enabled = false; }

void SDO::receiveFrame(Frame frame, uint32_t timestamp_us)
{
    if (!enabled || frame.cobId.bits.nodeId != node.nodeId)
        return;
    SDO_CommandByte recvCommand = {frame.data[0]};
    uint16_t index = *(uint16_t *)(frame.data + 1);
    uint8_t subindex = frame.data[3];
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
        if (timestamp_us - transferData.timestamp >= SDO_TIMEOUT_US)
        {
            sendAbort(transferData.object->index, transferData.subindex, SDOAbortCode_TimedOut);
        }
        break;
    default:
        break;
    }
}
