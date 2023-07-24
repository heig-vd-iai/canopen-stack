#include "sdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstdio>
#include <cstring>

CANopen_SDO::CANopen_SDO(CANopen_Node &node) : node(node)
{
}

void CANopen_SDO::update(uint32_t timestamp_us)
{
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

void CANopen_SDO::receiveFrame(CANopen_Frame frame, uint32_t timestamp_us)
{
    if (frame.nodeId != node.nodeId)
        return;
    SDO_CommandByte recvCommand = {frame.data[0]};
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], SDOAbortCode_CommandSpecifierInvalid);
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], SDOAbortCode_CommandSpecifierInvalid);
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], SDOAbortCode_CommandSpecifierInvalid);
            break;
        }
        break;
    }
}

void CANopen_SDO::sendAbort(uint16_t index, uint8_t subindex, uint32_t errorCode)
{
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0};
    // Fill command byte
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_AbortTransfer;
    // Fill response frame fields
    response.functionCode = FunctionCode_TSDO;
    response.nodeId = node.nodeId;
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

void CANopen_SDO::uploadInitiate(CANopen_Frame request, uint32_t timestamp_us)
{
    const unsigned maxSize = 4; // TODO: be more precise
    CANopen_Frame response;
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
    if (subindex >= object->subNumber)
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->entries[subindex].accessType.bits.r)
    {
        sendAbort(index, subindex, SDOAbortCode_AttemptReadOnWriteOnly);
        return;
    }
    transferData.subindex = subindex;
    transferData.object = object;
    uint32_t size = object->entries[subindex].size;
    transferData.remainingBytes = size;
    // Fill command byte and frame data
    if (size > maxSize) // Segment transfer
    {
        sendCommand.bits_initiate.e = 0;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = 0;
        memcpy(response.data + 8 - maxSize, &size, sizeof(size));
    }
    else // Expedited transfer
    {
        sendCommand.bits_initiate.e = 1;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = maxSize - size;
        if (!object->readBytes(subindex, response.data + maxSize, size, 0, &errorCode))
        {
            sendAbort(index, subindex, errorCode);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseUploadInitiate;
    // Fill response frame fields
    response.functionCode = FunctionCode_TSDO;
    response.nodeId = node.nodeId;
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

void CANopen_SDO::uploadSegment(CANopen_Frame request, uint32_t timestamp_us)
{
    const unsigned maxSize = 7;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint32_t errorCode = 0;
    unsigned payloadSize = transferData.remainingBytes > maxSize ? maxSize : transferData.remainingBytes;
    unsigned bytesSent = transferData.object->entries[transferData.subindex].size - transferData.remainingBytes;
    // Fill command byte
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseUploadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    sendCommand.bits_segment.n = maxSize - payloadSize;
    sendCommand.bits_segment.c = !transferData.remainingBytes;
    // Fill response frame fields
    response.functionCode = FunctionCode_TSDO;
    response.nodeId = node.nodeId;
    response.dlc = 8;
    // Upload response data
    response.data[0] = sendCommand.value;
    if (!transferData.object->readBytes(transferData.subindex, response.data + 8 - maxSize, payloadSize, bytesSent, &errorCode))
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

void CANopen_SDO::downloadInitiate(CANopen_Frame request, uint32_t timestamp_us)
{
    const unsigned maxSize = 4; // TODO: be more precise
    CANopen_Frame response;
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
    if (subindex >= object->subNumber)
    {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!object->entries[subindex].accessType.bits.w)
    {
        sendAbort(index, subindex, SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }
    transferData.subindex = subindex;
    transferData.object = object;
    uint32_t size = object->entries[subindex].size;
    // Fill command byte and write data
    if (recvCommand.bits_initiate.e) // Expedited transfer
    {
        transferData.remainingBytes = recvCommand.bits_initiate.s ? maxSize - recvCommand.bits_initiate.n : size;
        if (transferData.remainingBytes > maxSize)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
            return;
        }
        if (!object->writeBytes(subindex, request.data + 8 - maxSize, transferData.remainingBytes, &errorCode, node))
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
        transferData.remainingBytes = *(uint32_t *)(request.data + 4); // TODO
        if (transferData.remainingBytes != size)
        {
            sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterMismatch);
            return;
        }
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ResponseDownloadInitiate;
    // Fill response frame fields
    response.functionCode = FunctionCode_TSDO;
    response.nodeId = node.nodeId;
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

void CANopen_SDO::downloadSegment(CANopen_Frame request, uint32_t timestamp_us)
{
    const unsigned maxSize = 7;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint32_t errorCode = 0;
    unsigned size = transferData.object->entries[transferData.subindex].size;
    unsigned payloadSize = maxSize - recvCommand.bits_segment.n;
    unsigned bytesReceived = size - transferData.remainingBytes;
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
    memcpy(transferData.buffer + bytesReceived, request.data + 8 - maxSize, payloadSize);
    if (recvCommand.bits_segment.c && !transferData.object->writeBytes(transferData.subindex, transferData.buffer, size, &errorCode, node))
    {
        sendAbort(transferData.object->index, transferData.subindex, errorCode);
        return;
    }
    transferData.remainingBytes -= payloadSize;
    // Fill command byte
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ResponseDownloadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    // Fill response frame fields
    response.functionCode = FunctionCode_TSDO;
    response.nodeId = node.nodeId;
    response.dlc = 8;
    // Fill response frame data
    response.data[0] = sendCommand.value;
    // Send response
    node.sendFrame(response);
    if (recvCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp = timestamp_us;
}
