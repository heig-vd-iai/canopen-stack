#include "sdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstdio>
#include <cstring>

CANopen_SDO::CANopen_SDO(CANopen_Node &node) : node(node)
{
}

void CANopen_SDO::receiveFrame(CANopen_Frame frame)
{
    if (frame.nodeId != node.nodeId)
        return;
    SDO_CommandByte recvCommand = {frame.data[0]};
    switch (serverState)
    {
    case SDOServerStates_Ready:
        switch (recvCommand.bits_initiate.ccs)
        {
        case SDOCommandSpecifiers_RequestUploadInitiate:
            uploadInitiate(frame);
            break;
        case SDOCommandSpecifiers_RequestDownloadInitiate:
            downloadInitiate(frame);
            break;
        default:
            sendAbort(0, 0, 0x05040001);
            break;
        }
        break;
    case SDOServerStates_Uploading:
        switch (recvCommand.bits_segment.ccs)
        {
        case SDOCommandSpecifiers_RequestUploadSegment:
            uploadSegment(frame);
            break;
        case SDOCommandSpecifiers_AbortTransfer:
            serverState = SDOServerStates_Ready;
            break;
        default:
            sendAbort(0, 0, 0x05040001);
            break;
        }
        break;
    case SDOServerStates_Downloading:
        switch (recvCommand.bits_segment.ccs)
        {
        case SDOCommandSpecifiers_RequestDownloadSegment:
            downloadSegment(frame);
            break;
        case SDOCommandSpecifiers_AbortTransfer:
            serverState = SDOServerStates_Ready;
            break;
        default:
            sendAbort(0, 0, 0x05040001);
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
    sendCommand.bits_initiate.ccs = SDOCommandSpecifiers_AbortTransfer;
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
    serverState = SDOServerStates_Ready;
}

void CANopen_SDO::uploadInitiate(CANopen_Frame request)
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
    sendCommand.bits_initiate.ccs = SDOCommandSpecifiers_ResponseUploadInitiate;
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
        serverState = SDOServerStates_Uploading;
}

void CANopen_SDO::uploadSegment(CANopen_Frame request)
{
    const unsigned maxSize = 7;
    CANopen_Frame response;
    SDO_CommandByte sendCommand = {0}, recvCommand = {request.data[0]};
    unsigned payloadSize = transferData.remainingBytes > maxSize ? maxSize : transferData.remainingBytes;
    unsigned bytesSent = transferData.objectSize - transferData.remainingBytes;
    // Fill command byte
    sendCommand.bits_segment.ccs = SDOCommandSpecifiers_ResponseUploadSegment;
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
        serverState = SDOServerStates_Ready;
}

void CANopen_SDO::downloadInitiate(CANopen_Frame request)
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
    sendCommand.bits_initiate.ccs = SDOCommandSpecifiers_ResponseDownloadInitiate;
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
        serverState = SDOServerStates_Downloading;
}

void CANopen_SDO::downloadSegment(CANopen_Frame request)
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
    sendCommand.bits_segment.ccs = SDOCommandSpecifiers_ResponseDownloadSegment;
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
        serverState = SDOServerStates_Ready;
}
