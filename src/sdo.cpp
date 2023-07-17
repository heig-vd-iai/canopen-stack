#include "sdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstdio>
#include <cstring>

CANopen_SDO::CANopen_SDO(CANopen_Node &node) : node(node)
{
}

void CANopen_SDO::update(uint64_t timestamp_us)
{
    switch (serverState)
    {
    case SDOServerState_Uploading:
    case SDOServerState_Downloading:
        if (timestamp_us - transferData.timestamp >= SDO_TIMEOUT_US)
        {
            sendAbort(transferData.index, transferData.subIndex, 0x05040000);
        }
        break;
    default:
        break;
    }
}

void CANopen_SDO::receiveFrame(CANopen_Frame frame, uint64_t timestamp_us)
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], 0x05040001);
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], 0x05040001);
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
            sendAbort(*(uint16_t *)(frame.data + 1), frame.data[3], 0x05040001);
            break;
        }
        break;
    }
}

void CANopen_SDO::sendAbort(uint16_t index, uint8_t subIndex, uint32_t error)
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
    response.data[3] = subIndex;
    *(uint32_t *)(response.data + 4) = error;
    // Send response
    node.sendFrame(response);
    serverState = SDOServerState_Ready;
}

void CANopen_SDO::uploadInitiate(CANopen_Frame request, uint64_t timestamp_us)
{
    const unsigned maxSize = 4;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0};
    uint16_t index = *(uint16_t *)(request.data + 1);
    uint8_t subIndex = request.data[3];
    OD_ObjectEntry *entry = node.od.findEntry(index);
    if (entry == NULL)
    {
        sendAbort(index, subIndex, 0x06020000);
        return;
    }
    if (subIndex >= entry->subNumber)
    {
        sendAbort(index, subIndex, 0x06090011);
        return;
    }
    if (!entry->objects[subIndex].accessType.bits.r)
    {
        sendAbort(index, subIndex, 0x06010001);
        return;
    }
    transferData.index = index;
    transferData.subIndex = subIndex;
    transferData.objectSize = transferData.remainingBytes = entry->getSize(subIndex);
    transferData.dataSrc = (uint8_t *)entry->objects[subIndex].valueSrc;
    // Fill command byte and frame data
    if (transferData.objectSize > maxSize) // Segment transfer
    {
        sendCommand.bits_initiate.e = 0;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = 0;
        memcpy(response.data + 8 - maxSize, &transferData.objectSize, sizeof(transferData.objectSize));
    }
    else // Expedited transfer
    {
        sendCommand.bits_initiate.e = 1;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = maxSize - transferData.objectSize;
        memcpy(response.data + maxSize, entry->objects[subIndex].valueSrc, transferData.objectSize);
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

void CANopen_SDO::uploadSegment(CANopen_Frame request, uint64_t timestamp_us)
{
    const unsigned maxSize = 7;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    unsigned payloadSize = transferData.remainingBytes > maxSize ? maxSize : transferData.remainingBytes;
    unsigned bytesSent = transferData.objectSize - transferData.remainingBytes;
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
    memcpy(response.data + 8 - maxSize, transferData.dataSrc + bytesSent, payloadSize);
    transferData.remainingBytes -= payloadSize;
    // Send response
    node.sendFrame(response);
    if (sendCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp = timestamp_us;
}

void CANopen_SDO::downloadInitiate(CANopen_Frame request, uint64_t timestamp_us)
{
    const unsigned maxSize = 4;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    uint16_t index = *(uint16_t *)(request.data + 1);
    uint8_t subIndex = request.data[3];
    OD_ObjectEntry *entry = node.od.findEntry(index);
    if (entry == NULL)
    {
        sendAbort(index, subIndex, 0x06020000);
        return;
    }
    if (subIndex >= entry->subNumber)
    {
        sendAbort(index, subIndex, 0x06090011);
        return;
    }
    if (!entry->objects[subIndex].accessType.bits.w)
    {
        sendAbort(index, subIndex, 0x06010002);
        return;
    }
    transferData.index = index;
    transferData.subIndex = subIndex;
    transferData.objectSize = entry->getSize(subIndex);
    transferData.dataSrc = (uint8_t *)entry->objects[subIndex].valueSrc;
    // Fill command byte and write data
    if (recvCommand.bits_initiate.e) // Expedited transfer
    {
        transferData.remainingBytes = recvCommand.bits_initiate.s ? maxSize - recvCommand.bits_initiate.n : transferData.objectSize;
        if (transferData.remainingBytes > maxSize)
        {
            sendAbort(index, subIndex, 0x06070012);
            return;
        }
        memcpy(transferData.dataSrc, request.data + 8 - maxSize, transferData.remainingBytes);
    }
    else // Segment transfer
    {
        if (!recvCommand.bits_initiate.s)
        {
            sendAbort(index, subIndex, 0x06040043);
            return;
        }
        transferData.remainingBytes = *(uint32_t *)(request.data + 4);
        if (transferData.remainingBytes != transferData.objectSize)
        {
            sendAbort(index, subIndex, 0x06070010);
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

void CANopen_SDO::downloadSegment(CANopen_Frame request, uint64_t timestamp_us)
{
    const unsigned maxSize = 7;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    unsigned payloadSize = maxSize - recvCommand.bits_segment.n;
    unsigned bytesReceived = transferData.objectSize - transferData.remainingBytes;
    if (bytesReceived + payloadSize > transferData.objectSize)
    {
        sendAbort(transferData.index, transferData.subIndex, 0x06070012);
        return;
    }
    // Download request data
    if (transferData.objectSize > sizeof(transferData.buffer)) // Download to OD directly
    {
        memcpy(transferData.dataSrc + bytesReceived, request.data + 8 - maxSize, payloadSize);
    }
    else // Use download buffer instead
    {
        memcpy(transferData.buffer + bytesReceived, request.data + 8 - maxSize, payloadSize);
        if (recvCommand.bits_segment.c)
            memcpy(transferData.dataSrc, transferData.buffer, transferData.objectSize);
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
