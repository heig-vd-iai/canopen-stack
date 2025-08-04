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

#define SDO_REMOTE_ACCESS_MAX_ATTEMPTS 50000

namespace CANopen {

/**
 * Enumeration representing the command specifier part in the SDO command byte.
 * See CiA301:2011§7.2.4.3 (p. 49)
 *
 * It concerns the ccs (command code specifier) bits.
 */
enum SDOClientCommandSpecifiers {
    SDO_CCS_DOWNLOAD_SEGMENT_REQUEST = 0,
    SDO_CCS_INITIATE_DOWNLOAD_REQUEST = 1,
    SDO_CCS_INITIATE_UPLOAD_REQUEST = 2,
    SDO_CCS_UPLOAD_SEGMENT_REQUEST = 3,
    SDO_CCS_ABORT_TRANSFER = 4,
    SDO_CCS_BLOCK_UPLOAD = 5,
    SDO_CCS_BLOCK_DOWNLOAD = 6,
    SDO_CCS_RESERVED = 7
};

enum SDOServerCommandSpecifiers {
    SDO_SCS_INITIATE_DOWNLOAD_RESPONSE = 0,
    SDO_SCS_DOWNLOAD_SEGMENT_RESPONSE = 1,
    SDO_SCS_INITIATE_UPLOAD_RESPONSE = 2,
    SDO_SCS_UPLOAD_SEGMENT_RESPONSE = 3,
    SDO_SCS_ABORT_TRANSFER = 4,
    SDO_SCS_BLOCK_UPLOAD_RESPONSE = 5,
    SDO_SCS_BLOCK_DOWNLOAD_RESPONSE = 6,
    SDO_SCS_RESERVED = 7
};

/**
 * Union representing the SDO command byte (byte 0) in SDO transactions.
 *
 * Initiate is used for both upload and download requests.
 * Segment is used for upload and download segments in non expedited mode.
 */
union SDOCommandByte {
    uint8_t value;
    struct {
        bool s : 1;      // 0: no specified size, 1: size specified
        bool e : 1;      // 0: segmented transfer, 1: expedited transfer
        unsigned n : 2;  // unused bytes 0..3 to complete the 4 bytes
        unsigned reserved : 1;
        SDOClientCommandSpecifiers ccs : 3;
    } bits_initiate;
    struct {
        bool c : 1;  // 0: not final segment, 1: final segment
        unsigned n : 3;
        bool t : 1;  // toggle on each segment to detect errors
        SDOClientCommandSpecifiers ccs : 3;
    } bits_segment;
};

/**
 * Union representing the SDO command byte (byte 0) in SDO block transactions.
 * Multiple structures are used depending on the operation.
 */
union SDOBlockCommandByte {
    uint8_t value;
    // Used in "Initiate Block Download Request" (client → server)
    struct {
        bool cs : 1;  // 0: initiate block download, 1: reserved
        bool s : 1;   // 0: size not specified, 1: size specified
        bool cc : 1;  // 0: client does not support CRC, 1: client supports CRC
        unsigned reserved : 2;
        SDOClientCommandSpecifiers ccs : 3;
    } bits_downClientInitiate;

    // Used in "Block Download Sub-block Request" (client → server)
    struct {
        unsigned seqno : 7;  // sequence number of the block segment (1..127)
        bool c : 1;          // 0: more segments to follow, 1: last segment
    } bits_downClientSub;

    // Used in "End Block Download Request" (client → server)
    struct {
        bool cs : 1;  // 0: end block download, 1: reserved (client subcommand)
        bool reserved : 1;
        unsigned n : 3;  // number of unused bytes (0..7) in the last segment
        SDOClientCommandSpecifiers ccs : 3;  // always SDO_CCS_BLOCK_DOWNLOAD
    } bits_downClientEnd;

    // Used in "Block Download Response" (server → client)
    struct {
        unsigned
            ss : 2;  // server subcommand: 0=acknowledge sequence, 1..3 reserved
        bool sc : 1;  // 0: server does not support CRC, 1: server supports CRC
        unsigned reserved : 2;
        SDOServerCommandSpecifiers
            scs : 3;  // always SDO_SCS_BLOCK_DOWNLOAD_RESPONSE
    } bits_downServer;

    // Used in "Block Upload Request" (client → server)
    struct {
        unsigned cs : 2;  // 0=start upload, 1=end upload, 2..3 reserved
        bool cc : 1;  // 0: client does not support CRC, 1: client supports CRC
        unsigned reserved : 2;
        SDOClientCommandSpecifiers ccs : 3;  // always SDO_CCS_BLOCK_UPLOAD
    } bits_upClient;

    // Used in "Initiate Block Upload Response" (server → client)
    struct {
        bool ss : 1;  // 0: initiate block upload, 1: reserved
        bool s : 1;   // 0: size not specified, 1: size specified
        bool sc : 1;  // 0: server does not support CRC, 1: server supports CRC
        unsigned reserved : 2;
        SDOServerCommandSpecifiers
            scs : 3;  // always SDO_SCS_BLOCK_UPLOAD_RESPONSE
    } bits_upServerInitiate;

    // Used in "Block Upload Sub-block Response" (server → client)
    struct {
        unsigned seqno : 7;  // sequence number of the block segment (1..127)
        bool c : 1;          // 0: more segments to follow, 1: last segment
    } bits_upServerSub;

    // Used in "End Block Upload Response" (server → client)
    struct {
        unsigned ss : 2;  // 0=confirm end block upload, 1..3
                          // reserved
        unsigned n : 3;   // number of unused bytes (0..7) in the last segment
        SDOServerCommandSpecifiers
            scs : 3;  // always SDO_SCS_BLOCK_UPLOAD_RESPONSE
    } bits_upServerEnd;
};

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
    uint8_t domainBuffer[DOMAIN_MAX_SIZE];

    struct {
        int32_t odID;             // Index in object dictionary
        uint16_t index;           // Object index
        uint8_t subindex;         // Object subindex
        uint16_t size;            // Object size
        uint32_t remainingBytes;  // Remaining bytes to transfer in non
                                  // expedited mode
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
        bool isDomain = false;
    } transferData;
    struct {
        uint8_t data[SDO_BUFFSIZE] = {0};
        uint32_t offset = 0;
    } buffer;

    /**
     * Enables SDO functionality, should only be called internally by NMT class.
     */
    void enable();

    /**
     * Disables SDO functionality, should only be called internally by NMT
     * class.
     */
    void disable();

    /**
     * Checks if a timeout has occurred.
     * @param timestamp_us Current timestamp in microseconds.
     * @param timeout_us Timeout duration in microseconds.
     * @return True if a timeout has occurred, otherwise false.
     */
    bool isTimeout(uint32_t timestamp_us, uint32_t timeout_us);

    /**
     * Sends an abort frame with specified abort code and object index and
     * subindex.
     * @param index Index of the current object.
     * @param subindex Subindex of the current object.
     * @param abortCode SDO abort code.
     */
    void sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode);

    /**
     * Sends an abort frame with specified abort code.
     * @param abortCode SDO abort code.
     */
    void sendAbort(uint32_t abortCode);

    /**
     * Handles upload initiate frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void handleUploadInitiateRequest(class SDOFrame &request,
                                     uint32_t timestamp_us);

    /**
     * Handles the sending of upload initiate frames.
     * It should only be called by handleUploadInitiateRequest() or update()
     * depending on the object being remotely updated or not.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void uploadInitiateSend(uint32_t timestamp_us);

    /**
     * Handles upload segment frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void uploadSegment(class SDOFrame &request, uint32_t timestamp_us);

    void uploadSegmentSend(uint32_t timestamp_us);

    /**
     * THandles download initiate frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void handleDownloadInitiateRequest(class SDOFrame &request,
                                       uint32_t timestamp_us);

    void downloadInitiateSend(uint32_t timestamp_us);

    /**
     * Handles download segment frames.
     * @param request SDOFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void downloadSegment(class SDOFrame &request, uint32_t timestamp_us);

    void downloadSegmentSend(uint32_t timestamp_us);

    /**
     * Handles block upload initiate frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void handleBlockUploadInitiateRequest(class SDOBlockFrame &request,
                                          uint32_t timestamp_us);

    /**
     * Handles the sending of block upload initiate frames.
     * It should only be called by handleBlockUploadInitiateRequest() or
     * update() depending on the object being remotely updated or not.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void blockUploadInitiateSend(uint32_t timestamp_us);

    /**
     * Handles the reception of client block upload response and
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
     * Handles block download initiate frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void handleBlockDownloadInitateRequest(class SDOBlockFrame &request,
                                           uint32_t timestamp_us);

    /**
     * Handles successions of download sub-block frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockDownloadReceive(class SDOBlockFrame &request,
                              uint32_t timestamp_us);

    /**
     * Handles block download end frames.
     * @param request SDOBlockFrame to be processed.
     * @param timestamp_us Frame reception timestamp in microseconds.
     */
    void blockDownloadEnd(class SDOBlockFrame &request, uint32_t timestamp_us);

    /**
     * Called when the last sub-block of a block was received, or
     * when a download timeout occurs (missing sub-blocks).
     * @param timestamp_us Current timestamp in microseconds.
     */
    void blockDownloadEndSub(uint32_t timestamp_us);

    /**
     * Receives and process an SDO frame.
     * @param frame SDOFrame to be processed.
     * @param timestamp_us Timestamp in microseconds of the frame reception.
     */
    void receiveFrame(class SDOFrame &frame, uint32_t timestamp_us);

    /**
     * Checks for general SDO or block download timeouts and send block upload
     * sub-blocks.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void update(uint32_t timestamp_us);

    /**
     * Empty and reset the download buffer.
     */
    void bufferReset();

    /**
     * Appends data to the download buffer.
     * @param data Pointer to the source data.
     * @param sizeBytes Size in bytes of the data.
     */
    void bufferAppend(uint8_t *data, uint32_t sizeBytes);

   public:
    friend class NMT;
    friend class Node;
};
}  // namespace CANopen
