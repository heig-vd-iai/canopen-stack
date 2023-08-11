#pragma once
#include <cstdint>
#define SDO_TIMEOUT_US 5000
#define SDO_INITIATE_DATA_LENGTH 4
#define SDO_INITIATE_DATA_OFFSET 4
#define SDO_SEGMENT_DATA_LENGTH 7
#define SDO_SEGMENT_DATA_OFFSET 1
#define SDO_BUFFSIZE 256

namespace CANopen
{
    class SDO
    {
        enum SDOServerStates
        {
            SDOServerState_Ready,
            SDOServerState_Uploading,
            SDOServerState_Downloading
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
            uint32_t timestamp;
            uint8_t toggle;
        } transferData;

        void sendAbort(uint16_t index, uint8_t subindex, uint32_t errorCode);
        void uploadInitiate(class Frame &request, uint32_t timestamp_us);
        void uploadSegment(class Frame &request, uint32_t timestamp_us);
        void downloadInitiate(class Frame &request, uint32_t timestamp_us);
        void downloadSegment(class Frame &request, uint32_t timestamp_us);

    public:
        SDO(class Node &node);
        void enable();
        void disable();
        void receiveFrame(class Frame &frame, uint32_t timestamp_us);
        void update(uint32_t timestamp_us);
    };
}
