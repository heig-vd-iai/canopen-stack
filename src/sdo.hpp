#pragma once
#include <cstdint>
// #define SDO_TIMEOUT_US 5000
#define SDO_TIMEOUT_US 1000000 // TODO
#define SDO_BUFFSIZE 256
#define SDO_DLC 8
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

namespace CANopen
{
    class SDO
    {
        enum SDOServerStates
        {
            SDOServerState_Ready,
            SDOServerState_Uploading,
            SDOServerState_Downloading,
            SDOServerState_BlockUploading,
            SDOServerState_BlockDownloading,
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
            uint8_t buffer[SDO_BUFFSIZE];
            uint32_t timestamp_us;
            uint8_t toggle;
            // Block transfer specific
            uint16_t blksize;
            uint16_t seqno;
            uint32_t lastBlockRemainingBytes;
        } transferData;

        void enable();
        void disable();
        void sendAbort(uint16_t index, uint8_t subindex, uint32_t abortCode);
        void sendAbort(uint32_t abortCode);
        void uploadInitiate(class SDOFrame &request, uint32_t timestamp_us);
        void uploadSegment(class SDOFrame &request, uint32_t timestamp_us);
        void downloadInitiate(class SDOFrame &request, uint32_t timestamp_us);
        void downloadSegment(class SDOFrame &request, uint32_t timestamp_us);
        void blockUploadInitiate(class SDOBlockFrame &request, uint32_t timestamp_us);
        void blockUploadReceive(class SDOBlockFrame &request, uint32_t timestamp_us);
        void blockUploadSubBlock(uint32_t timestamp_us);
        void receiveFrame(class SDOFrame &frame, uint32_t timestamp_us);
        void update(uint32_t timestamp_us);

    public:
        friend class NMT;
        friend class Node;
        SDO(class Node &node);
    };
}
