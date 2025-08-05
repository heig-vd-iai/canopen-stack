/**
 * Contains the definitions of the SDO class.
 */
#include "sdo.hpp"

#include <cstring>

#include "enums.hpp"
#include "frame.hpp"
#include "iobject-dictionnary.hpp"
#include "node.hpp"
#include "unions.hpp"
using namespace CANopen;

ObjectDictionnary &SDO::od() { return node.od(); }

void SDO::sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode) {
    SDOCommandByte sendCommand = {0};
    sendCommand.initiate.ccs = SDO_CCS_ABORT_TRANSFER;
    SDOFrame response(node.nodeId, sendCommand.value);
    response.setIndex(index);
    response.setSubindex(subindex);
    response.setAbortCode(abortCode);
    sendFrame(response);
    serverState = SDOServerState_Ready;
}

void SDO::sendAbort(uint32_t abortCode) { sendAbort(0, 0, abortCode); }

void SDO::sendFrame(const Frame &frame) { node.hardware().sendFrame(frame); }

void SDO::handleUploadInitiateRequest(SDOFrame &request,
                                      uint32_t timestamp_us) {
    // Get object information
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    int32_t id = od().findObject(index, subindex);
    if (id == -1) {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (id == -2) {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }

    // Fetch metadata
    Metadata metadata = *od().getMetadata(id);
    if (!metadata.access.bits.readable) {
        sendAbort(index, subindex, SDOAbortCode_AttemptReadOnWriteOnly);
        return;
    }
    if (UNLIKELY(metadata.dataType == DataType::DOMAIN)) {
        transferData.data.domain = (uint16_t *)domainBuffer;
        transferData.isDomain = true;
    } else {
        transferData.isDomain = false;
    }

    // Prepare transfer data
    uint16_t size = od().getSize(id);
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.odID = id;
    transferData.remainingBytes = size;
    transferData.size = size;
    transferData.toggle = false;

    SDOAbortCodes abortCode;
    int8_t ret = od().readData(transferData.data, transferData.odID, abortCode);
    if (abortCode != SDOAbortCode_OK) {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        return;
    }
    if (ret == 1) {
        // Data not received yet, wait for next update
        serverState = SDOServerState_UploadPending;
        transferData.timestamp_us = timestamp_us;
    } else {
        // Data collected, ready to send
        uploadInitiateSend(timestamp_us);
    }
}

void SDO::uploadInitiateSend(uint32_t timestamp_us) {
    int16_t size = transferData.size;

    const int maxFrameSize = SDO_INITIATE_DATA_LENGTH;
    bool expedited = size <= maxFrameSize;
    bool remSize = size > maxFrameSize ? 0 : maxFrameSize - size;

    SDOCommandInitiate cmd(true, expedited, remSize,
                           SDO_CCS_INITIATE_UPLOAD_REQUEST);

    SDOFrame response(node.nodeId);
    if (expedited) {
        memcpy(response.data + SDO_INITIATE_DATA_OFFSET, &transferData.data.u8,
               size);
    } else {
        response.setInitiateData(size);
    }
    response.setCommandByte(cmd.encode());
    response.setIndex(transferData.index);
    response.setSubindex(transferData.subindex);
    sendFrame(response);

    serverState = expedited ? SDOServerState_Ready : SDOServerState_Uploading;
    transferData.timestamp_us = timestamp_us;
}

void SDO::uploadSegment(SDOFrame &request, uint32_t timestamp_us) {
    // Gather commands

    recvCommand = {request.getCommandByte()};

    // Toggle bit management
    if (transferData.toggle != recvCommand.segment.t) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.segment.t;
    uint32_t payloadSize = transferData.remainingBytes > SDO_SEGMENT_DATA_LENGTH
                               ? SDO_SEGMENT_DATA_LENGTH
                               : transferData.remainingBytes;
    uint32_t bytesSent =
        od().getSize(transferData.odID) - transferData.remainingBytes;

    SDOCommandByte sendCommand = {0};
    sendCommand.segment.ccs = SDO_CCS_DOWNLOAD_SEGMENT_REQUEST;
    sendCommand.segment.t = recvCommand.segment.t;
    sendCommand.segment.n = SDO_SEGMENT_DATA_LENGTH - payloadSize;
    sendCommand.segment.c = !transferData.remainingBytes;

    SDOFrame response(node.nodeId, sendCommand.value);
    uint8_t *srcPtr = transferData.isDomain
                           ? domainBuffer + bytesSent
                           : &transferData.data.u8 + bytesSent;

    memcpy(response.data + SDO_SEGMENT_DATA_OFFSET, srcPtr, payloadSize);

    transferData.remainingBytes -= payloadSize;
    sendFrame(response);
    if (sendCommand.segment.c) serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::handleDownloadInitiateRequest(SDOFrame &request,
                                        uint32_t timestamp_us) {
    recvCommand = {request.getCommandByte()};
    uint16_t index = request.getIndex();
    uint8_t subindex = request.getSubindex();
    int32_t id = od().findObject(index, subindex);
    if (id == -1) {
        sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
        return;
    }
    if (id == -2) {
        sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
        return;
    }
    if (!od().getMetadata(id)->access.bits.writeable) {
        sendAbort(index, subindex, SDOAbortCode_AttemptWriteOnReadOnly);
        return;
    }
    transferData.index = index;
    transferData.subindex = subindex;
    transferData.odID = id;
    uint16_t size = od().getSize(id);
    transferData.toggle = false;
    if (recvCommand.initiate.e) {  // Expedited transfer
        transferData.remainingBytes =
            recvCommand.initiate.s
                ? SDO_INITIATE_DATA_LENGTH - recvCommand.initiate.n
                : size;
        if (transferData.remainingBytes > SDO_INITIATE_DATA_LENGTH) {
            sendAbort(index, subindex,
                      SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
            return;
        }
        SDOAbortCodes abortCode;
        Data tmp;
        memcpy(&tmp.u8, request.data + SDO_INITIATE_DATA_OFFSET, size);
        int8_t ret = od().writeData(tmp, index, subindex, abortCode);
        if (abortCode != SDOAbortCode_OK) {
            sendAbort(index, subindex, abortCode);
            return;
        }
        if (ret == 1) {
            serverState = SDOServerState_DownloadPending;
            transferData.timestamp_us = timestamp_us;
        } else {
            downloadInitiateSend(timestamp_us);
        }
    } else {  // Segment transfer
        if (!recvCommand.initiate.s) {
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
        transferBuffer.reset();
        downloadInitiateSend(timestamp_us);
    }
}

void SDO::downloadInitiateSend(uint32_t timestamp_us) {
    SDOCommandByte sc = {0};
    sc.initiate.ccs = SDO_CCS_UPLOAD_SEGMENT_REQUEST;

    SDOFrame response(node.nodeId, sc.value);
    response.setIndex(transferData.index);
    response.setSubindex(transferData.subindex);
    sendFrame(response);

    serverState = recvCommand.initiate.e ? SDOServerState_Ready
                                         : SDOServerState_Downloading;

    transferData.timestamp_us = timestamp_us;
}

void SDO::downloadSegment(SDOFrame &request, uint32_t timestamp_us) {
    recvCommand = {request.getCommandByte()};
    uint32_t size = od().getSize(transferData.odID);
    uint32_t payloadSize = SDO_SEGMENT_DATA_LENGTH - recvCommand.segment.n;
    uint32_t bytesReceived = size - transferData.remainingBytes;
    if (transferData.toggle != recvCommand.segment.t) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_ToggleBitNotAlternated);
        return;
    }
    transferData.toggle = !recvCommand.segment.t;
    if (bytesReceived + payloadSize > size) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh);
        return;
    }
    transferBuffer.append(request.data + SDO_SEGMENT_DATA_OFFSET,
                          payloadSize);  // Append received data to the buffer

    SDOAbortCodes abortCode;
    if (recvCommand.segment.c) {
        memcpy(&transferData.data.u8, transferBuffer.data, size);
        int8_t ret = od().writeData(transferData.data, transferData.index,
                                    transferData.subindex, abortCode);
        if (abortCode != SDOAbortCode_OK) {
            sendAbort(transferData.index, transferData.subindex, abortCode);
            return;
        }
        if (ret == 1) {
            serverState = SDOServerState_DownloadSegmentPending;
            transferData.timestamp_us = timestamp_us;
        } else {
            downloadInitiateSend(timestamp_us);
        }
    } else {
        downloadSegmentSend(timestamp_us);
    }
}

/**
 * In non expedited mode, the segments has to be sent in separate frames.
 */
void SDO::downloadSegmentSend(uint32_t timestamp_us) {
    transferData.remainingBytes -=
        SDO_SEGMENT_DATA_LENGTH - recvCommand.segment.n;

    SDOCommandByte sendCommand = {0};
    sendCommand.segment.ccs = SDO_CCS_INITIATE_DOWNLOAD_REQUEST;
    sendCommand.segment.t = recvCommand.segment.t;
    SDOFrame response(node.nodeId, sendCommand.value);
    sendFrame(response);
    transferData.timestamp_us = timestamp_us;
}

void SDO::handleBlockUploadInitiateRequest(SDOBlockFrame &request,
                                           uint32_t timestamp_us) {
    SDOBlockCommandByte recvCommand = {request.getCommandByte()};
    switch ((SDOSubCommands)recvCommand.upClient.cs) {
        case SDOSubCommand_ClientUploadInitiate: {
            uint16_t index = request.getIndex();
            uint8_t subindex = request.getSubindex();
            int32_t id = od().findObject(index, subindex);
            if (id == -1) {
                sendAbort(index, subindex, SDOAbortCode_ObjectNonExistent);
                return;
            }
            if (id == -2) {
                sendAbort(index, subindex, SDOAbortCode_SubindexNonExistent);
                return;
            }
            Metadata metadata = *od().getMetadata(id);
            if (!metadata.access.bits.readable) {
                sendAbort(index, subindex, SDOAbortCode_AttemptReadOnWriteOnly);
                return;
            }
            if (UNLIKELY(metadata.dataType == DataType::DOMAIN)) {
                transferData.data.domain = (uint16_t *)domainBuffer;
                transferData.isDomain = true;
            } else {
                transferData.isDomain = false;
            }
            transferData.index = index;
            transferData.subindex = subindex;
            transferData.odID = id;
            transferData.remainingBytes = transferData.lastBlockRemainingBytes =
                od().getSize(id);
            transferData.blksize = request.getInitiateBlockSize();
            transferData.seqno = SDO_BLOCK_SEQNO_MIN;
            SDOAbortCodes abortCode;
            int8_t ret =
                od().readData(transferData.data, transferData.odID, abortCode);
            if (abortCode != SDOAbortCode_OK) {
                sendAbort(transferData.index, transferData.subindex, abortCode);
                return;
            }
            if (ret == 1) {
                transferData.timestamp_us = timestamp_us;
                serverState = SDOServerState_BlockPending;
            } else
                blockUploadInitiateSend(timestamp_us);
            break;
        }
        case SDOSubCommand_ClientUploadStart: {
            serverState = SDOServerState_BlockUploading;
            transferData.timestamp_us = timestamp_us;
            break;
        }
        default:
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            break;
    }
}

void SDO::blockUploadInitiateSend(uint32_t timestamp_us) {
    // Prepare command byte
    SDOBlockCommandByte sendCommand = {0};
    sendCommand.upServerInitiate.scs = SDO_SCS_BLOCK_DOWNLOAD_RESPONSE;
    sendCommand.upServerInitiate.ss = SDOSubCommand_ServerUploadInitiate;
    sendCommand.upServerInitiate.sc = false;
    sendCommand.upServerInitiate.s = true;

    // Send the response
    SDOBlockFrame frame(node.nodeId, sendCommand.value);
    frame.setIndex(transferData.index);
    frame.setSubindex(transferData.subindex);
    frame.setSize(transferData.size);
    sendFrame(frame);

    // Done
    serverState = SDOServerState_Ready;
    transferData.timestamp_us = timestamp_us;
}

void SDO::blockUploadReceive(class SDOBlockFrame &request,
                             uint32_t timestamp_us) {
    SDOBlockCommandByte recvCommand = {request.getCommandByte()};
    switch ((SDOSubCommands)recvCommand.upClient.cs) {
        case SDOSubCommand_ClientUploadResponse: {
            uint32_t ackseq = request.getAckseq();
            if (ackseq < transferData.seqno - 1U) {
                // Lost data, begin next sub-block at failed offset
                transferData.seqno = SDO_BLOCK_SEQNO_MIN;
                transferData.remainingBytes =
                    transferData.lastBlockRemainingBytes -
                    ackseq * SDO_BLOCK_DATA_LENGTH;
            } else {
                // All clear
                if (transferData.remainingBytes > 0) {
                    // Begin sending next block
                    transferData.lastBlockRemainingBytes =
                        transferData.remainingBytes;
                    transferData.seqno = SDO_BLOCK_SEQNO_MIN;
                    transferData.blksize = request.getSubBlockSize();
                } else {
                    // All data was sent, initiate block end
                    SDOBlockCommandByte sendCommand = {0};
                    sendCommand.upServerEnd.scs =
                        SDO_SCS_BLOCK_DOWNLOAD_RESPONSE;
                    sendCommand.upServerEnd.n =
                        SDO_BLOCK_DATA_LENGTH -
                        transferData.lastBlockRemainingBytes %
                            SDO_BLOCK_DATA_LENGTH;
                    sendCommand.upServerEnd.ss = SDOSubCommand_ServerUploadEnd;
                    SDOBlockFrame response(node.nodeId, sendCommand.value);
                    response.setCRC(0);
                    sendFrame(response);
                }
            }
            transferData.timestamp_us = timestamp_us;
            break;
        }
        case SDOSubCommand_ServerUploadEnd: {
            serverState = SDOServerState_Ready;
            break;
        }
        default:
            sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            break;
    }
}

void SDO::blockUploadSubBlock(uint32_t timestamp_us) {
    if (transferData.seqno > transferData.blksize ||
        transferData.remainingBytes == 0)
        return;
    SDOBlockCommandByte cmd = {0};
    uint32_t payloadSize;
    cmd.upServerSub.seqno = transferData.seqno++;
    if (transferData.remainingBytes > SDO_BLOCK_DATA_LENGTH) {
        payloadSize = SDO_BLOCK_DATA_LENGTH;
        cmd.upServerSub.c = false;
    } else {
        payloadSize = transferData.remainingBytes;
        cmd.upServerSub.c = true;
    }
    uint32_t bytesSent = transferData.size - transferData.remainingBytes;
    SDOBlockFrame frame(node.nodeId, cmd.value);
    if (transferData.isDomain) {
        memcpy(frame.data + SDO_BLOCK_DATA_OFFSET, domainBuffer + bytesSent,
               payloadSize);
    } else {
        memcpy(frame.data + SDO_BLOCK_DATA_OFFSET,
               &transferData.data.u8 + bytesSent, payloadSize);
    }
    transferData.remainingBytes -= payloadSize;
    sendFrame(frame);
    transferData.timestamp_us = timestamp_us;
}

void SDO::handleBlockDownloadInitateRequest(SDOBlockFrame &request,
                                            uint32_t timestamp_us) {}

void SDO::blockDownloadReceive(SDOBlockFrame &request, uint32_t timestamp_us) {}

void SDO::blockDownloadEnd(class SDOBlockFrame &request,
                           uint32_t timestamp_us) {}

void SDO::blockDownloadEndSub(uint32_t timestamp_us) {}

void SDO::receiveFrame(SDOFrame &frame, uint32_t timestamp_us) {
    if (!enabled || frame.nodeId != node.nodeId) return;

    SDOCommandByte cmd = {frame.getCommandByte()};
    SDOClientCommandSpecifiers ccs = cmd.initiate.ccs;

    switch (serverState) {
        case SDOServerState_Ready:
            switch (ccs) {
                case SDO_CCS_INITIATE_UPLOAD_REQUEST:
                    handleUploadInitiateRequest(frame, timestamp_us);
                    break;
                case SDO_CCS_INITIATE_DOWNLOAD_REQUEST:
                    handleDownloadInitiateRequest(frame, timestamp_us);
                    break;
                case SDO_CCS_BLOCK_UPLOAD:
                    handleBlockUploadInitiateRequest((SDOBlockFrame &)frame,
                                                     timestamp_us);
                    break;
                case SDO_CCS_BLOCK_DOWNLOAD:
                    handleBlockDownloadInitateRequest((SDOBlockFrame &)frame,
                                                      timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
                    break;
            }
            break;
        case SDOServerState_Uploading:
            switch (ccs) {
                case SDO_CCS_UPLOAD_SEGMENT_REQUEST:
                    uploadSegment(frame, timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
                    break;
            }
            break;
        case SDOServerState_Downloading:
            switch (ccs) {
                case SDO_CCS_DOWNLOAD_SEGMENT_REQUEST:
                    downloadSegment(frame, timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            }
            break;
        case SDOServerState_BlockUploading:
            switch (ccs) {
                case SDO_CCS_BLOCK_UPLOAD:
                    blockUploadReceive((SDOBlockFrame &)frame, timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            }
            break;
        case SDOServerState_BlockDownloading:
            switch (ccs) {
                case SDO_CCS_BLOCK_DOWNLOAD:
                    blockDownloadReceive((SDOBlockFrame &)frame, timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            }
            break;
        case SDOServerState_BlockDownloadingEnding:
            switch (ccs) {
                case SDO_CCS_BLOCK_DOWNLOAD:
                    blockDownloadEnd((SDOBlockFrame &)frame, timestamp_us);
                    break;
                default:
                    sendAbort(SDOAbortCode_CommandSpecifierInvalid);
            }
            break;
        default:
            break;
    }
}

/**
 * Only used on server mode for block transfers.
 */
void SDO::update(uint32_t timestamp_us) {
    if (UNLIKELY(!enabled)) return;
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

            if (od().readData(transferData.data, transferData.odID,
                              abortCode) == 0) {
                remoteAccesAttempt = 0;
                uploadInitiateSend(timestamp_us);
            } else {
                remoteAccesAttempt++;
            }
            break;
        case SDOServerState_DownloadPending:
            if (od().writeData(transferData.data, transferData.index,
                               transferData.subindex, abortCode) == 0) {
                remoteAccesAttempt = 0;
                downloadInitiateSend(timestamp_us);
                if (recvCommand.segment.c) serverState = SDOServerState_Ready;
            } else {
                remoteAccesAttempt++;
            }

            break;
        case SDOServerState_DownloadSegmentPending:
            if (od().writeData(transferData.data, transferData.index,
                               transferData.subindex, abortCode) == 0) {
                remoteAccesAttempt = 0;
                downloadInitiateSend(timestamp_us);
            } else {
                remoteAccesAttempt++;
            }
            break;
        case SDOServerState_BlockPending:
            if (od().readData(transferData.data, transferData.odID,
                              abortCode) == 0) {
                remoteAccesAttempt = 0;
                blockUploadInitiateSend(timestamp_us);
            } else {
                remoteAccesAttempt++;
            }
            break;
        default:
            break;
    }
    if (remoteAccesAttempt > SDO_REMOTE_ACCESS_MAX_ATTEMPTS) {
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_AccessFailedHardwareError);
        serverState = SDOServerState_Ready;
        remoteAccesAttempt = 0;
    }
    if (UNLIKELY(abortCode != SDOAbortCode_OK)) {
        sendAbort(transferData.index, transferData.subindex, abortCode);
        abortCode = SDOAbortCode_OK;
    }
    if (UNLIKELY(serverState != SDOServerState_Ready &&
                 isTimeout(timestamp_us, SDO_TIMEOUT_US)))
        sendAbort(transferData.index, transferData.subindex,
                  SDOAbortCode_TimedOut);
}
