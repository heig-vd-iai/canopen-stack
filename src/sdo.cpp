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

bool SDO::isTimeout(uint32_t timestamp_us, uint32_t timeout_us)
{
    return timestamp_us - transferData.timestamp_us >= timeout_us;
}

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

void SDO::sendAbort(uint32_t abortCode)
{
    sendAbort(0, 0, abortCode);
}

void SDO::uploadInitiate(SDOFrame &request, uint32_t timestamp_us)
{
    SDOCommandByte sendCommand = {0};
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    Object *object = node._od.findObject(index);
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
    SDOFrame response(node.nodeId);
    if (size > SDO_INITIATE_DATA_LENGTH)
    { // Segment transfer
        sendCommand.bits_initiate.e = 0;
        sendCommand.bits_initiate.s = 1;
        sendCommand.bits_initiate.n = 0;
        response.setInitiateData(size);
    }
    else
    { // Expedited transfer
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
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ServerUploadInitiate;
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
    uint32_t payloadSize = transferData.remainingBytes > SDO_SEGMENT_DATA_LENGTH ? SDO_SEGMENT_DATA_LENGTH : transferData.remainingBytes;
    uint32_t bytesSent = transferData.object->getSize(transferData.subindex) - transferData.remainingBytes;
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ServerUploadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    sendCommand.bits_segment.n = SDO_SEGMENT_DATA_LENGTH - payloadSize;
    sendCommand.bits_segment.c = !transferData.remainingBytes;
    SDOFrame response(node.nodeId, sendCommand.value);
    uint32_t abortCode;
    if ((abortCode = transferData.object->readBytes(transferData.subindex, response.data + SDO_SEGMENT_DATA_OFFSET, payloadSize, bytesSent)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.index, transferData.subindex, abortCode);
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
    Object *object = node._od.findObject(index);
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
    if (recvCommand.bits_initiate.e)
    { // Expedited transfer
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
    else
    { // Segment transfer
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
        bufferReset();
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ServerDownloadInitiate;
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
    uint32_t size = transferData.object->getSize(transferData.subindex);
    uint32_t payloadSize = SDO_SEGMENT_DATA_LENGTH - recvCommand.bits_segment.n;
    uint32_t bytesReceived = size - transferData.remainingBytes;
    if (transferData.toggle != recvCommand.bits_segment.t)
    {
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.bits_segment.t;
    if (bytesReceived + payloadSize > size)
    {
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
        return;
    }
    bufferAppend(request.data + SDO_SEGMENT_DATA_OFFSET, payloadSize);
    uint32_t abortCode;
    if (recvCommand.bits_segment.c && (abortCode = transferData.object->writeBytes(transferData.subindex, buffer.data, size, node)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        return;
    }
    transferData.remainingBytes -= payloadSize;
    sendCommand.bits_segment.ccs = SDOCommandSpecifier_ServerDownloadSegment;
    sendCommand.bits_segment.t = recvCommand.bits_segment.t;
    SDOFrame response(node.nodeId, sendCommand.value);
    node.sendFrame(response);
    if (recvCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockUploadInitiate(SDOBlockFrame &request, uint32_t timestamp_us)
{
    SDOBlockCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    switch ((SDOSubCommands)recvCommand.bits_upClient.cs)
    {
    case SDOSubCommand_ClientUploadInitiate:
    {
        uint16_t index = request.getIndex();
        uint8_t subindex = request.getSubindex();
        Object *object = node._od.findObject(index);
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
        uint32_t size = object->getSize(subindex);
        transferData.index = index;
        transferData.subindex = subindex;
        transferData.object = object;
        transferData.remainingBytes = transferData.lastBlockRemainingBytes = size;
        transferData.blksize = request.getInitiateBlockSize();
        transferData.seqno = SDO_BLOCK_SEQNO_MIN;
        sendCommand.bits_upServerInitiate.scs = SDOCommandSpecifier_ServerBlockUpload;
        sendCommand.bits_upServerInitiate.sc = 0;
        sendCommand.bits_upServerInitiate.s = 1;
        sendCommand.bits_upServerInitiate.ss = SDOSubCommand_ServerUploadInitiate;
        SDOBlockFrame response(node.nodeId, sendCommand.value);
        response.setIndex(index);
        response.setSubindex(subindex);
        response.setSize(size);
        node.sendFrame(response);
        transferData.timestamp_us = timestamp_us;
        break;
    }
    case SDOSubCommand_ClientUploadStart:
    {
        serverState = SDOServerState_BlockUploading;
        transferData.timestamp_us = timestamp_us;
        break;
    }
    default:
        sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    }
}

void SDO::blockUploadReceive(class SDOBlockFrame &request, uint32_t timestamp_us)
{
    SDOBlockCommandByte recvCommand = {request.getCommandByte()};
    switch ((SDOSubCommands)recvCommand.bits_upClient.cs)
    {
    case SDOSubCommand_ClientUploadResponse:
    {
        uint32_t ackseq = request.getAckseq();
        if (ackseq < transferData.seqno - 1U)
        { // Lost data, begin next sub-block at failed offset
            transferData.seqno = SDO_BLOCK_SEQNO_MIN;
            transferData.remainingBytes = transferData.lastBlockRemainingBytes - ackseq * SDO_BLOCK_DATA_LENGTH;
        }
        else
        { // All clear
            if (transferData.remainingBytes > 0)
            { // Begin sending next block
                transferData.lastBlockRemainingBytes = transferData.remainingBytes;
                transferData.seqno = SDO_BLOCK_SEQNO_MIN;
                transferData.blksize = request.getSubBlockSize();
            }
            else
            { // All data was sent, initiate block end
                SDOBlockCommandByte sendCommand = {0};
                sendCommand.bits_upServerEnd.scs = SDOCommandSpecifier_ServerBlockUpload;
                sendCommand.bits_upServerEnd.n = SDO_BLOCK_DATA_LENGTH - transferData.lastBlockRemainingBytes % SDO_BLOCK_DATA_LENGTH;
                sendCommand.bits_upServerEnd.ss = SDOSubCommand_ServerUploadEnd;
                SDOBlockFrame response(node.nodeId, sendCommand.value);
                response.setCRC(0);
                node.sendFrame(response);
            }
        }
        transferData.timestamp_us = timestamp_us;
        break;
    }
    case SDOSubCommand_ServerUploadEnd:
    {
        serverState = SDOServerState_Ready;
        break;
    }
    default:
        sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    }
}

void SDO::blockUploadSubBlock(uint32_t timestamp_us)
{
    if (transferData.seqno > transferData.blksize || transferData.remainingBytes == 0)
        return;
    SDOBlockCommandByte cmd = {0};
    uint32_t payloadSize;
    cmd.bits_upServerSub.seqno = transferData.seqno++;
    if (transferData.remainingBytes > SDO_BLOCK_DATA_LENGTH)
    {
        payloadSize = SDO_BLOCK_DATA_LENGTH;
        cmd.bits_upServerSub.c = 0;
    }
    else
    {
        payloadSize = transferData.remainingBytes;
        cmd.bits_upServerSub.c = 1;
    }
    uint32_t bytesSent = transferData.object->getSize(transferData.subindex) - transferData.remainingBytes;
    SDOBlockFrame frame(node.nodeId, cmd.value);
    uint32_t abortCode;
    if ((abortCode = transferData.object->readBytes(transferData.subindex, frame.data + SDO_BLOCK_DATA_OFFSET, payloadSize, bytesSent)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        return;
    }
    transferData.remainingBytes -= payloadSize;
    node.sendFrame(frame);
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockDownloadInitiate(SDOBlockFrame &request, uint32_t timestamp_us)
{
    SDOBlockCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    if (recvCommand.bits_downClientInitiate.cs != SDOSubCommand_ClientDownloadInitiate)
    {
        sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        return;
    }
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    Object *object = node._od.findObject(index);
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
    uint32_t size = object->getSize(subindex);
    if (recvCommand.bits_downClientInitiate.s && size != request.getSize())
    {
        sendAbort(index, subindex, SDOAbortCode_DataTypeMismatch_LengthParameterMismatch);
        return;
    }
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.object = object;
    transferData.remainingBytes = transferData.lastBlockRemainingBytes = size;
    transferData.blksize = SDO_BLOCK_SEQNO_MAX;
    transferData.seqno = transferData.ackseq = 0;
    transferData.retries = 0;
    sendCommand.bits_downServer.scs = SDOCommandSpecifier_ServerBlockDownload;
    sendCommand.bits_downServer.sc = 0;
    sendCommand.bits_downServer.ss = SDOSubCommand_ServerDownloadInitiate;
    SDOBlockFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    response.setInitiateBlockSize(transferData.blksize);
    node.sendFrame(response);
    bufferReset();
    serverState = SDOServerState_BlockDownloading;
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockDownloadReceive(SDOBlockFrame &request, uint32_t timestamp_us)
{
    SDOBlockCommandByte recvCommand = {request.getCommandByte()};
    if (recvCommand.bits_downClientSub.seqno == transferData.ackseq + 1U)
    { // Valid sub-block received
        transferData.ackseq++;
        uint32_t payloadSize = transferData.remainingBytes > SDO_BLOCK_DATA_LENGTH ? SDO_BLOCK_DATA_LENGTH : transferData.remainingBytes;
        bufferAppend(request.data + SDO_BLOCK_DATA_OFFSET, payloadSize);
        transferData.remainingBytes -= payloadSize;
    }
    if (++transferData.seqno >= transferData.blksize || recvCommand.bits_downClientSub.c)
    { // Last sub-block, send confirmation to client
        blockDownloadEndSub(timestamp_us);
        if (recvCommand.bits_downClientSub.c)
            serverState = SDOServerState_BlockDownloadingEnding;
    }
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockDownloadEnd(class SDOBlockFrame &request, uint32_t timestamp_us)
{
    SDOBlockCommandByte sendCommand = {0}, recvCommand = {request.getCommandByte()};
    if (recvCommand.bits_downClientEnd.cs != SDOSubCommand_ClientDownloadEnd)
    {
        sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        return;
    }
    uint32_t abortCode;
    if ((abortCode = transferData.object->writeBytes(transferData.subindex, buffer.data, transferData.object->getSize(transferData.subindex), node)) != SDOAbortCode_OK)
    {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        return;
    }
    sendCommand.bits_downServer.scs = SDOCommandSpecifier_ServerBlockDownload;
    sendCommand.bits_downServer.ss = SDOSubCommand_ServerDownloadEnd;
    SDOBlockFrame frame(node.nodeId, sendCommand.value);
    node.sendFrame(frame);
    serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockDownloadEndSub(uint32_t timestamp_us)
{
    SDOBlockCommandByte cmd = {0};
    cmd.bits_downServer.scs = SDOCommandSpecifier_ServerBlockDownload;
    cmd.bits_downServer.ss = SDOSubCommand_ServerDownloadResponse;
    SDOBlockFrame frame(node.nodeId, cmd.value);
    frame.setAckseq(transferData.ackseq);
    frame.setSubBlockSize(transferData.blksize);
    node.sendFrame(frame);
    transferData.ackseq = transferData.seqno = 0;
    transferData.timestamp_us = timestamp_us;
}

void SDO::receiveFrame(SDOFrame &frame, uint32_t timestamp_us)
{
    if (!enabled || frame.nodeId != node.nodeId)
        return;
    SDOCommandByte cmd = {frame.getCommandByte()};
    switch (serverState)
    {
    case SDOServerState_Ready:
        switch ((SDOCommandSpecifiers)cmd.bits_initiate.ccs)
        {
        case SDOCommandSpecifier_ClientUploadInitiate:
            uploadInitiate(frame, timestamp_us);
            break;
        case SDOCommandSpecifier_ClientDownloadInitiate:
            downloadInitiate(frame, timestamp_us);
            break;
        case SDOCommandSpecifier_ClientBlockUpload:
            blockUploadInitiate((SDOBlockFrame &)frame, timestamp_us);
            break;
        case SDOCommandSpecifier_ClientBlockDownload:
            blockDownloadInitiate((SDOBlockFrame &)frame, timestamp_us);
            break;
        default:
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            break;
        }
        break;
    case SDOServerState_Uploading:
        if (cmd.bits_segment.ccs == SDOCommandSpecifier_ClientUploadSegment)
            uploadSegment(frame, timestamp_us);
        else
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    case SDOServerState_Downloading:
        if (cmd.bits_segment.ccs == SDOCommandSpecifier_ClientDownloadSegment)
            downloadSegment(frame, timestamp_us);
        else
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    case SDOServerState_BlockUploading:
        if (cmd.bits_segment.ccs == SDOCommandSpecifier_ClientBlockUpload)
            blockUploadReceive((SDOBlockFrame &)frame, timestamp_us);
        else
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    case SDOServerState_BlockDownloading:
        blockDownloadReceive((SDOBlockFrame &)frame, timestamp_us);
        break;
    case SDOServerState_BlockDownloadingEnding:
        if (cmd.bits_segment.ccs == SDOCommandSpecifier_ClientBlockDownload)
            blockDownloadEnd((SDOBlockFrame &)frame, timestamp_us);
        else
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
        break;
    default:
        break;
    }
}

void SDO::update(uint32_t timestamp_us)
{
    if (!enabled)
        return;
    switch (serverState)
    {
    case SDOServerState_BlockUploading:
        blockUploadSubBlock(timestamp_us);
        break;
    case SDOServerState_BlockDownloading:
        if (isTimeout(timestamp_us, SDO_BLOCK_DOWNLOAD_TIMEOUT_US) && transferData.retries < SDO_BLOCK_MAX_RETRIES)
        {
            blockDownloadEndSub(timestamp_us);
            transferData.retries++;
        }
        break;
    default:
        break;
    }
    if (serverState != SDOServerState_Ready && isTimeout(timestamp_us, SDO_TIMEOUT_US))
        sendAbort(transferData.index, transferData.subindex, SDOAbortCode_TimedOut);
}

void SDO::bufferReset()
{
    memset(buffer.data, 0, sizeof(buffer.data));
    buffer.offset = 0;
}

void SDO::bufferAppend(uint8_t *data, uint32_t size)
{
    uint32_t availableSize = sizeof(buffer.data) - buffer.offset;
    uint32_t transferSize = size < availableSize ? size : availableSize;
    if (!transferSize)
        return;
    memcpy(buffer.data + buffer.offset, data, transferSize);
    buffer.offset += transferSize;
}
