/**
 * Contains the definitions of the SDO class.
 */
#include "sdo.hpp"

#include <cstring>

#include "enums.hpp"
#include "frame.hpp"
#include "node.hpp"
#include "unions.hpp"
using namespace CANopen;

void SDO::enable() { enabled = true; }

void SDO::disable() { enabled = false; }

bool SDO::isTimeout(uint32_t timestamp_us, uint32_t timeout_us) {
    return timestamp_us - transferData.timestamp_us >= timeout_us;
}

void SDO::sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode) {
    SDOCommandByte sendCommand = {0};
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_AbortTransfer;
    SDOFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    response.setAbortCode(abortCode);
    node.hardware().sendFrame(response);
    serverState = SDOServerState_Ready;
}

void SDO::sendAbort(uint32_t abortCode) { sendAbort(0, 0, abortCode); }

void SDO::uploadInitiate(SDOFrame &request, uint32_t timestamp_us) {
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    int32_t id = node.od().findObject(index, subindex);
    if (id == -1) {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (id == -2) {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!node.od().getMetadata(id).access.bits.readable) {
        sendAbort(index, subindex, SDOAbortCode_AttemptReadOnWriteOnly);
        return;
    }
    uint16_t size = node.od().getSize(id);
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.odID = id;
    transferData.remainingBytes = size;
    transferData.size = size;
    transferData.toggle = false;

    SDOAbortCodes abortCode;
    if (node.od().readData(transferData.data, transferData.odID, abortCode) ==
        1) {
        serverState = SDOServerState_UploadPending;
        transferData.timestamp_us = timestamp_us;
    } else {
        uploadInitiateSend(timestamp_us);
    }
    if (abortCode != SDOAbortCode_OK) {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        return;
    }
}

void SDO::uploadInitiateSend(uint32_t timestamp_us) {
    SDOFrame response(node.nodeId);
    SDOCommandByte sendCommand = {0};
    int16_t size = transferData.size;
    if (size > SDO_INITIATE_DATA_LENGTH) {  // Segment transfer
        sendCommand.bits_initiate.e = false;
        sendCommand.bits_initiate.s = true;
        sendCommand.bits_initiate.n = false;
        response.setInitiateData(size);
    } else {  // Expedited transfer
        sendCommand.bits_initiate.e = true;
        sendCommand.bits_initiate.s = true;
        sendCommand.bits_initiate.n = SDO_INITIATE_DATA_LENGTH - size;
        SDOAbortCodes abortCode;
        memcpy(response.data + SDO_INITIATE_DATA_OFFSET, &transferData.data.u8,
               size);
    }
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ServerUploadInitiate;
    response.setCommandByte(sendCommand.value);
    response.setIndex(transferData.index);
    response.setSubindex(transferData.subindex);
    node.hardware().sendFrame(response);
    serverState = sendCommand.bits_initiate.e ? SDOServerState_Ready
                                              : SDOServerState_Uploading;
    transferData.timestamp_us = timestamp_us;
}

void SDO::uploadSegment(SDOFrame &request, uint32_t timestamp_us) {
    transferData.sendCommand = {0};
    transferData.recvCommand = {request.getCommandByte()};
    if (transferData.toggle != transferData.recvCommand.bits_segment.t) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !transferData.recvCommand.bits_segment.t;
    uint32_t payloadSize = transferData.remainingBytes > SDO_SEGMENT_DATA_LENGTH
                               ? SDO_SEGMENT_DATA_LENGTH
                               : transferData.remainingBytes;
    uint32_t bytesSent =
        node.od().getSize(transferData.odID) - transferData.remainingBytes;
    transferData.sendCommand.bits_segment.ccs =
        SDOCommandSpecifier_ServerUploadSegment;
    transferData.sendCommand.bits_segment.t =
        transferData.recvCommand.bits_segment.t;
    transferData.sendCommand.bits_segment.n =
        SDO_SEGMENT_DATA_LENGTH - payloadSize;
    transferData.sendCommand.bits_segment.c = !transferData.remainingBytes;
    SDOFrame response(node.nodeId, transferData.sendCommand.value);
    memcpy(response.data + SDO_SEGMENT_DATA_OFFSET,
           &transferData.data.u8 + bytesSent, payloadSize);
    transferData.remainingBytes -= payloadSize;
    node.hardware().sendFrame(response);
    if (transferData.sendCommand.bits_segment.c)
        serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::downloadInitiate(SDOFrame &request, uint32_t timestamp_us) {
    transferData.sendCommand = {0};
    transferData.recvCommand = {request.getCommandByte()};
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    int32_t id = node.od().findObject(index, subindex);
    if (id == -1) {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (id == -2) {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!node.od().getMetadata(id).access.bits.writeable) {
        sendAbort(index, subindex, SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.odID = id;
    uint16_t size = node.od().getSize(id);
    transferData.toggle = false;
    if (transferData.recvCommand.bits_initiate.e) {  // Expedited transfer
        transferData.remainingBytes =
            transferData.recvCommand.bits_initiate.s
                ? SDO_INITIATE_DATA_LENGTH -
                      transferData.recvCommand.bits_initiate.n
                : size;
        if (transferData.remainingBytes > SDO_INITIATE_DATA_LENGTH) {
            sendAbort(index, subindex,
                      SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
            return;
        }
        SDOAbortCodes abortCode;
        Data tmp;
        memcpy(&tmp.u8, request.data + SDO_INITIATE_DATA_OFFSET, size);
        if (node.od().writeData(tmp, index, subindex, abortCode) == 1) {
            serverState = SDOServerState_DownloadPending;
            transferData.timestamp_us = timestamp_us;
        } else {
            downloadInitiateSend(timestamp_us);
        }
        if (abortCode != SDOAbortCode_OK) {
            sendAbort(index, subindex, abortCode);
            return;
        }
    } else {  // Segment transfer
        if (!transferData.recvCommand.bits_initiate.s) {
            sendAbort(index, subindex,
                      SDOAbortCode_GeneralParameterIncompatibility);
            return;
        }
        transferData.remainingBytes = request.getInitiateData();
        if (transferData.remainingBytes != size) {
            sendAbort(index, subindex,
                      SDOAbortCode_DataTypeMismatch_LengthParameterMismatch);
            return;
        }
        bufferReset();
        downloadInitiateSend(timestamp_us);
    }
}

void SDO::downloadInitiateSend(uint32_t timestamp_us) {
    SDOCommandByte sendCommand = {0}, recvCommand = transferData.recvCommand;
    uint16_t index = transferData.index;
    uint8_t subindex = transferData.subindex;
    sendCommand.bits_initiate.ccs = SDOCommandSpecifier_ServerDownloadInitiate;
    SDOFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    node.hardware().sendFrame(response);
    if (!recvCommand.bits_initiate.e)
        serverState = SDOServerState_Downloading;
    else
        serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::downloadSegment(SDOFrame &request, uint32_t timestamp_us) {
    transferData.sendCommand = {0};
    transferData.recvCommand = {request.getCommandByte()};
    uint32_t size = node.od().getSize(transferData.odID);
    uint32_t payloadSize =
        SDO_SEGMENT_DATA_LENGTH - transferData.recvCommand.bits_segment.n;
    uint32_t bytesReceived = size - transferData.remainingBytes;
    if (transferData.toggle != transferData.recvCommand.bits_segment.t) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !transferData.recvCommand.bits_segment.t;
    if (bytesReceived + payloadSize > size) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
        return;
    }
    bufferAppend(request.data + SDO_SEGMENT_DATA_OFFSET, payloadSize);
    SDOAbortCodes abortCode;
    if (transferData.recvCommand.bits_segment.c) {
        memcpy(&transferData.data.u8, buffer.data, size);
        if (node.od().writeData(transferData.data, transferData.index,
                                transferData.subindex, abortCode) == 1) {
            serverState = SDOServerState_DownloadPending;
            transferData.timestamp_us = timestamp_us;
        }
        if (abortCode != SDOAbortCode_OK) {
            sendAbort(transferData.index, transferData.subindex, abortCode);
            return;
        }
    }
    transferData.remainingBytes -= payloadSize;
    transferData.sendCommand.bits_segment.ccs =
        SDOCommandSpecifier_ServerDownloadSegment;
    transferData.sendCommand.bits_segment.t =
        transferData.recvCommand.bits_segment.t;
    SDOFrame response(node.nodeId, transferData.sendCommand.value);
    node.hardware().sendFrame(response);
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockUploadInitiate(SDOBlockFrame &request, uint32_t timestamp_us) {
    // TODO: block
}

void SDO::blockUploadInitiateSend(uint32_t timestamp_us) {}

void SDO::blockUploadReceive(class SDOBlockFrame &request,
                             uint32_t timestamp_us) {}

void SDO::blockUploadSubBlock(uint32_t timestamp_us) {}

void SDO::blockDownloadInitiate(SDOBlockFrame &request, uint32_t timestamp_us) {
}

void SDO::blockDownloadReceive(SDOBlockFrame &request, uint32_t timestamp_us) {}

void SDO::blockDownloadEnd(class SDOBlockFrame &request,
                           uint32_t timestamp_us) {}

void SDO::blockDownloadEndSub(uint32_t timestamp_us) {}

void SDO::receiveFrame(SDOFrame &frame, uint32_t timestamp_us) {
    if (!enabled || frame.nodeId != node.nodeId) return;
    SDOCommandByte cmd = {frame.getCommandByte()};
    switch (serverState) {
        case SDOServerState_Ready:
            switch ((SDOCommandSpecifiers)cmd.bits_initiate.ccs) {
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
            if (cmd.bits_segment.ccs ==
                SDOCommandSpecifier_ClientDownloadSegment)
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

void SDO::update(uint32_t timestamp_us) {
    if (!enabled) return;
    SDOAbortCodes abortCode = SDOAbortCode_OK;
    switch (serverState) {
        case SDOServerState_BlockUploading:
            blockUploadSubBlock(timestamp_us);
            break;
        case SDOServerState_BlockDownloading:
            if (isTimeout(timestamp_us, SDO_BLOCK_DOWNLOAD_TIMEOUT_US) &&
                transferData.retries < SDO_BLOCK_MAX_RETRIES) {
                blockDownloadEndSub(timestamp_us);
                transferData.retries++;
            }
            break;
        case SDOServerState_UploadPending:

            if (node.od().readData(transferData.data, transferData.odID,
                                   abortCode) == 0) {
                uploadInitiateSend(timestamp_us);
            }
            break;
        case SDOServerState_DownloadPending:
            if (node.od().writeData(transferData.data, transferData.index,
                                    transferData.subindex, abortCode) == 0) {
                downloadInitiateSend(timestamp_us);
                if (transferData.recvCommand.bits_segment.c)
                    serverState = SDOServerState_Ready;
            }
            break;
        case SDOServerState_BlockPending:
            break;
        default:
            break;
    }
    if (abortCode != SDOAbortCode_OK) {
        sendAbort(transferData.index, transferData.subindex, abortCode);
    }
    if (serverState != SDOServerState_Ready &&
        isTimeout(timestamp_us, SDO_TIMEOUT_US))
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_TimedOut);
}

void SDO::bufferReset() {
    memset(buffer.data, 0, sizeof(buffer.data));
    buffer.offset = 0;
}

void SDO::bufferAppend(uint8_t *data, uint32_t size) {
    uint32_t availableSize = sizeof(buffer.data) - buffer.offset;
    uint32_t transferSize = size < availableSize ? size : availableSize;
    if (!transferSize) return;
    memcpy(buffer.data + buffer.offset, data, transferSize);
    buffer.offset += transferSize;
}
