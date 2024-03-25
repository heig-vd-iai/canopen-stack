/**
 * Contains the declaration of the SDO class.
 */
#pragma once
#include <cstdint>

#include "od.hpp"
#define SDO_TIMEOUT_US 300000000
#define SDO_BLOCK_DOWNLOAD_TIMEOUT_US 10000
#define SDO_DLC 8
#define SDO_BUFFSIZE OD_ENTRY_SIZE_MAX
#define SDO_INITIATE_DATA_LENGTH 4
#define SDO_INITIATE_DATA_OFFSET 4
#define SDO_SEGMENT_DATA_LENGTH 7
#define SDO_SEGMENT_DATA_OFFSET 1
#define SDO_COMMANDBYTE_OFFSET 0
#define SDO_INDEX_OFFSET 1
#define SDO_SUBINDEX_OFFSET 3
#define SDO_ABORTCODE_OFFSET 4
#define SDO_BLOCK_SIZE_OFFSET 4
#define SDO_BLOCK_INIT_BLKSIZE_OFFSET 4
#define SDO_BLOCK_SUB_BLKSIZE_OFFSET 2
#define SDO_BLOCK_ACKSEQ_OFFSET 1
#define SDO_BLOCK_CRC_OFFSET 1
#define SDO_BLOCK_PST_OFFSET 5
#define SDO_BLOCK_DATA_LENGTH 7
#define SDO_BLOCK_DATA_OFFSET 1
#define SDO_BLOCK_SEQNO_MIN 1
#define SDO_BLOCK_SEQNO_MAX 127
#define SDO_BLOCK_MAX_RETRIES 1

#define SDO_REMOTE_ACCESS_MAX_ATTEMPTS 20

namespace CANopen {
/**
 * SDO object.
 * It handles data upload and download, in regular and block mode transfer.
 * See CiA301:2011§7.2.4 (p. 37)
 */
class SDO {
    enum SDOServerStates {
        SDOServerState_Ready,
        SDOServerState_UploadPending,
        SDOServerState_UploadSegmentPending,
        SDOServerState_DownloadPending,
        SDOServerState_DownloadSegmentPending,
        SDOServerState_Uploading,
        SDOServerState_Downloading,
        SDOServerState_BlockPending,
        SDOServerState_BlockUploading,
        SDOServerState_BlockDownloading,
        SDOServerState_BlockDownloadingEnding
    };

   private:
    bool enabled = false;
    uint16_t remoteAccesAttempt = 0;
    SDOServerStates serverState = SDOServerState_Ready;
    struct {
        int32_t odID;
        uint16_t index;
        uint8_t subindex;
        uint16_t size;
        uint32_t remainingBytes;
        uint32_t timestamp_us;
        uint8_t toggle;
        // Block transfer specific
        uint16_t blksize;
        uint16_t seqno;
        uint16_t ackseq;
        uint32_t lastBlockRemainingBytes;
        unsigned retries;
        Data data;
        SDOCommandByte sendCommand, recvCommand;
    } transferData;
    struct {
        uint8_t data[SDO_BUFFSIZE] = {0};
        uint32_t offset = 0;
    } buffer;

    /**
     * Enable SDO functionality, should only be called internally by NMT class.
     */
    void enable();

    /**
     * Disable SDO functionality, should only be called internally by NMT class.
     */
    void disable();

    /**
     * Check if a timeout has occurred.
     * @param timestamp_us Current timestamp in microseconds.
     * @param timeout_us Timeout duration in microseconds.
     * @return True if a timeout has occurred, otherwise false.
     */
    bool isTimeout(uint32_t timestamp_us, uint32_t timeout_us);

    /**
     * Send an abort frame with specified abort code and object index and
     * subindex.
     * @param index Index of the current object.
     * @param subindex Subindex of the current object.
     * @param abortCode SDO abort code.
     */
    void sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode);

    /**
     * Send an abort frame with specified abort code.
     * @param abortCode SDO abort code.
     */
    void sendAbort(uint32_t abortCode);

    /**
     * This method handles upload initiate frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void uploadInitiate(class SDOFrame &request, uint32_t timestamp_us);

    /**
     * This method handles the sending of upload initiate frames.
     * It should only be called by uploadInitiate() or update() depending on the
     * object being remotely updated or not.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void uploadInitiateSend(uint32_t timestamp_us);

    /**
     * This method handles upload segment frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void uploadSegment(class SDOFrame &request, uint32_t timestamp_us);

    void uploadSegmentSend(uint32_t timestamp_us);

    /**
     * This method handles download initiate frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void downloadInitiate(class SDOFrame &request, uint32_t timestamp_us);

    void downloadInitiateSend(uint32_t timestamp_us);

    /**
     * This method handles download segment frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void downloadSegment(class SDOFrame &request, uint32_t timestamp_us);

    void downloadSegmentSend(uint32_t timestamp_us);

    /**
     * This method handles block upload initiate frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockUploadInitiate(class SDOBlockFrame &request,
                             uint32_t timestamp_us);

    /**
     * This method handles the sending of block upload initiate frames.
     * It should only be called by blockUploadInitiate() or update() depending
     * on the object being remotely updated or not.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void blockUploadInitiateSend(uint32_t timestamp_us);

    /**
     * This method handles the reception of client block upload response and
     * client block upload end block.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockUploadReceive(class SDOBlockFrame &request,
                            uint32_t timestamp_us);

    /**
     * Periodically upload sub-blocks in succession during block upload stage.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void blockUploadSubBlock(uint32_t timestamp_us);

    /**
     * This method handles block download initiate frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockDownloadInitiate(class SDOBlockFrame &request,
                               uint32_t timestamp_us);

    /**
     * This method handles successions of download sub-block frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockDownloadReceive(class SDOBlockFrame &request,
                              uint32_t timestamp_us);

    /**
     * This method handles block download end frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockDownloadEnd(class SDOBlockFrame &request, uint32_t timestamp_us);

    /**
     * This method is called when the last sub-block of a block was received, or
     * when a download timeout occurs (missing sub-blocks).
     * @param timestamp_us Current timestamp in microseconds.
     */
    void blockDownloadEndSub(uint32_t timestamp_us);

    /**
     * Receive and process an SDO frame.
     * @param frame SDOFrame to be processed.
     * @param timestamp_us Timestamp in microseconds of the frame reception.
     */
    void receiveFrame(class SDOFrame &frame, uint32_t timestamp_us);

    /**
     * Check for general SDO or block download timeouts and send block upload
     * sub-blocks.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void update(uint32_t timestamp_us);

    /**
     * Empty and reset the download buffer.
     */
    void bufferReset();

    /**
     * Append data to the download buffer.
     * @param data Pointer to the source data.
     * @param sizeBytes Size in bytes of the data.
     */
    void bufferAppend(uint8_t *data, uint32_t sizeBytes);

   public:
    friend class NMT;
    friend class Node;

    /**
     * Constructor for the SDO class.
     * @param node The parent Node reference.
     */
};
}  // namespace CANopen
