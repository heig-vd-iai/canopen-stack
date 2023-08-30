/******************************************************************************
 * [Filename]:      sdo.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the declaration of the SDO class.
 *****************************************************************************/
#pragma once
#include "od.hpp"
#include <cstdint>
#define SDO_TIMEOUT_US 30000
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

namespace CANopen
{
    /**
     * @brief This class represents the SDO object.
     * It handles data upload and download, in regular and block mode transfer.
     * See p. 37 of CIA301 for more details.
     */
    class SDO
    {
        enum SDOServerStates
        {
            SDOServerState_Ready,
            SDOServerState_Uploading,
            SDOServerState_Downloading,
            SDOServerState_BlockUploading,
            SDOServerState_BlockDownloading,
            SDOServerState_BlockDownloadingEnding
        };

    private:
        bool enabled = false;
        class Node &node;
        SDOServerStates serverState = SDOServerState_Ready;
        struct
        {
            class Object *object;
            uint16_t index;
            uint8_t subindex;
            uint32_t remainingBytes;
            uint32_t timestamp_us;
            uint8_t toggle;
            // Block transfer specific
            uint16_t blksize;
            uint16_t seqno;
            uint16_t ackseq;
            uint32_t lastBlockRemainingBytes;
            unsigned retries;
        } transferData;
        struct
        {
            uint8_t data[SDO_BUFFSIZE] = {0};
            uint32_t offset = 0;
        } buffer;

        /**
         * @brief Enable %SDO functionality, should be used by NMT only.
         */
        void enable();

        /**
         * @brief Disable %SDO functionality, should be used by NMT only.
         */
        void disable();

        /**
         * @brief Check if a timeout has occurred.
         * @param timestamp_us Current timestamp in microseconds.
         * @param timeout_us Timeout duration in microseconds.
         * @return True if a timeout has occurred, otherwise false.
         */
        bool isTimeout(uint32_t timestamp_us, uint32_t timeout_us);

        /**
         * @brief Send an abort frame with specified abort code and object index and subindex.
         * @param index Index of the current object.
         * @param subindex Subindex of the current object.
         * @param abortCode %SDO abort code.
         */
        void sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode);

        /**
         * @brief Send an abort frame with specified abort code.
         * @param abortCode %SDO abort code.
         */
        void sendAbort(uint32_t abortCode);

        /**
         * @brief This method handles upload initiate frames.
         * @param request SDOFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void uploadInitiate(class SDOFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles upload segment frames.
         * @param request SDOFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void uploadSegment(class SDOFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles download initiate frames.
         * @param request SDOFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void downloadInitiate(class SDOFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles download segment frames.
         * @param request SDOFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void downloadSegment(class SDOFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles block upload initiate frames.
         * @param request SDOBlockFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void blockUploadInitiate(class SDOBlockFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles the reception of client block upload response and client block upload end block.
         * @param request SDOBlockFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void blockUploadReceive(class SDOBlockFrame &request, uint32_t timestamp_us);

        /**
         * @brief Periodically upload sub-blocks in succession during block upload stage.
         * @param timestamp_us Current timestamp in microseconds.
         */
        void blockUploadSubBlock(uint32_t timestamp_us);

        /**
         * @brief This method handles block download initiate frames.
         * @param request SDOBlockFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void blockDownloadInitiate(class SDOBlockFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles successions of download sub-block frames.
         * @param request SDOBlockFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void blockDownloadReceive(class SDOBlockFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method handles block download end frames.
         * @param request SDOBlockFrame to be processed.
         * @param timestamp_us %Frame reception timestamp in microseconds.
         */
        void blockDownloadEnd(class SDOBlockFrame &request, uint32_t timestamp_us);

        /**
         * @brief This method is called when the last sub-block of a block was received, or when a download timeout occurs (missing sub-blocks).
         * @param timestamp_us Current timestamp in microseconds.
         */
        void blockDownloadEndSub(uint32_t timestamp_us);

        /**
         * @brief Receive and process an SDO frame.
         * @param frame SDOFrame to be processed.
         * @param timestamp_us Timestamp in microseconds of the frame reception.
         */
        void receiveFrame(class SDOFrame &frame, uint32_t timestamp_us);

        /**
         * @brief Check for general %SDO or block download timeouts and send block upload sub-blocks.
         * @param timestamp_us Current timestamp in microseconds.
         */
        void update(uint32_t timestamp_us);

        /**
         * @brief Empty and reset the download buffer.
         */
        void bufferReset();

        /**
         * @brief Append data to the download buffer.
         * @param data Pointer to the source data.
         * @param size Size in bytes of the data.
         */
        void bufferAppend(uint8_t *data, uint32_t size);

    public:
        friend class NMT;
        friend class Node;

        /**
         * @brief Constructor for the SDO class.
         * @param node The parent Node reference.
         */
        SDO(class Node &node);
    };
}
