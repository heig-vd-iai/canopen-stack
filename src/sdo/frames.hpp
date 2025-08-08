#pragma once

#include "mixins.hpp"

/**
 * SDO frame abort transfer.
 *
 * +-----+---------+-------------------------------+---------------+----
 * | ccs |         |           index               |   subindex    | code
 * +-----+---------+-------------------------------+---------------+----
 */
struct SDOFrameAbort : public SDOTFrame,
                       public IndexSubindexMixin<SDOFrameAbort>,
                       public CCSMixin<SDOFrameAbort> {
    SDOFrameAbort(int8_t nodeId, uint16_t index = 0, uint8_t subindex = 0,
                  SDOAbortCodes abortCode = SDOAbortCodes::SDOAbortCode_OK)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::AbortTransfer);
        setIndexSubIndex(index, subindex);
        set_abortCode(abortCode);
    }

   private:
    SDOAbortCodes abortCode() const {
        const uint8_t* d = bytes();
        return static_cast<SDOAbortCodes>(read_le32(d + 4));
    }

    void set_abortCode(SDOAbortCodes code) {
        uint8_t* d = bytes();
        write_le32(d + 4, static_cast<uint32_t>(code));
    }
};

/**
 * SDO frame for download request.
 *
 * +-----+-+---+-+-+-------------------------------+---------------+-----
 * | ccs | | n |e|s|           index               |   subindex    | data 4..7
 * +-----+-+---+-+-+-------------------------------+---------------+-----
 */
struct SDOFrameInitiateDownloadRequest
    : public SDOTFrame,
      public IndexSubindexMixin<SDOFrameInitiateDownloadRequest>,
      public CCSMixin<SDOFrameInitiateDownloadRequest>,
      public ExpeditedTransferMixin<SDOFrameInitiateDownloadRequest>,
      public SizeSpecifiedMixin<SDOFrameInitiateDownloadRequest>,
      public UnusedBytes2Mixin<SDOFrameInitiateDownloadRequest> {
    SDOFrameInitiateDownloadRequest(int8_t nodeId, uint16_t index = 0,
                                    uint8_t subindex = 0)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::DownloadSegmentRequest);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for upload response.
 *
 * +-----+-+---+-+-+-------------------------------+---------------+-----
 * | scs | | n |e|s|           index               |   subindex    | data 4..7
 * +-----+-+---+-+-+-------------------------------+---------------+-----
 */
struct SDOFrameInitiateUploadResponse
    : public SDORFrame,
      public IndexSubindexMixin<SDOFrameInitiateUploadResponse>,
      public SCSMixin<SDOFrameInitiateUploadResponse>,
      public ExpeditedTransferMixin<SDOFrameInitiateUploadResponse>,
      public UnusedBytes2Mixin<SDOFrameInitiateUploadResponse>,
      public FinalTransferMixin<SDOFrameInitiateUploadResponse>,
      public SizeSpecifiedMixin<SDOFrameInitiateUploadResponse> {
    SDOFrameInitiateUploadResponse(uint8_t nodeId, uint16_t index = 0,
                                   uint8_t subindex = 0)
        : SDORFrame(nodeId) {
        set_scs(SCS::InitiateUploadResponse);
        setIndexSubIndex(index, subindex);
    }

    unsigned setSize(uint32_t size) {
        auto available = 4;
        bool overflow = (size > available);
        auto payloadSize = overflow ? 0 : size;
        set_unused_bytes(available - payloadSize);
        set_final_transfer(!overflow);
        return payloadSize;
    }

    unsigned getSize() const {
        auto available = 4;
        return available - unused_bytes();
    }

    unsigned setData(uint8_t* dataPtr, uint8_t size) {
        auto payloadSize = setSize(size);
        memcpy(&bytes()[1], dataPtr, payloadSize);
        return payloadSize;
    }

    void getData(uint8_t* dataPtr) { memcpy(dataPtr, &bytes()[1], getSize()); }
};

/**
 * SDO frame for download response.
 *
 * +-----+---------+-------------------------------+---------------+----
 * | ccs |         |           index               |   subindex    | reserved
 * +-----+---------+-------------------------------+---------------+----
 *
 */
struct SDOFrameInitiateDownloadResponse
    : public SDORFrame,
      public IndexSubindexMixin<SDOFrameInitiateDownloadResponse>,
      public SCSMixin<SDOFrameInitiateDownloadResponse> {
    SDOFrameInitiateDownloadResponse(uint8_t nodeId, uint16_t index = 0,
                                     uint8_t subindex = 0)
        : SDORFrame(nodeId) {
        set_scs(SCS::InitiateDownloadResponse);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for upload request.
 *
 * +-----+---------+-------------------------------+---------------+----
 * | scs |         |           index               |   subindex    | reserved
 * +-----+---------+-------------------------------+---------------+----
 *
 */
struct SDOFrameInitiateUploadRequest
    : public SDOTFrame,
      public IndexSubindexMixin<SDOFrameInitiateUploadRequest>,
      public CCSMixin<SDOFrameInitiateUploadRequest> {
    SDOFrameInitiateUploadRequest(uint8_t nodeId, uint16_t index = 0,
                                  uint8_t subindex = 0)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::InitiateUploadRequest);
        setIndexSubIndex(index, subindex);
    }
};


/**
 * SDO frame for download response.
 *
 * +-----+-+-------+---------------------------------------------...
 * | scs |t|       |           reserved 1..7
 * +-----+-+-------+---------------------------------------------...
 */
struct SDOFrameDownloadSegmentResponse
    : public SDORFrame,
      public SCSMixin<SDOFrameDownloadSegmentResponse>,
      public ToggleBitMixin<SDOFrameDownloadSegmentResponse> {
    SDOFrameDownloadSegmentResponse(uint8_t nodeId)
        : SDORFrame(nodeId) {
        set_scs(SCS::DownloadSegmentResponse);
    }
};

/**
 * SDO frame for upload request.
 *
 * +-----+-+-------+---------------------------------------------...
 * | ccs |t|       |           reserved 1..7
 * +-----+-+-------+---------------------------------------------...
 */
struct SDOFrameUploadSegmentRequest
    : public SDOTFrame,
      public CCSMixin<SDOFrameUploadSegmentRequest>,
      public ToggleBitMixin<SDOFrameUploadSegmentRequest> {
    SDOFrameUploadSegmentRequest(uint8_t nodeId)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::UploadSegmentRequest);
    }
};

/**
 * SDO frame for download segment request.
 *
 * +-----+-+-----+-+---------------------------------------------...
 * | ccs |t| n   |c|           segment-data 1..7
 * +-----+-+-----+-+---------------------------------------------...
 *
 */
struct SDOFrameDownloadSegmentRequest
    : public SDOTFrame,
      public CCSMixin<SDOFrameDownloadSegmentRequest>,
      public FrameSegmentMixin<SDOFrameDownloadSegmentRequest> {
    SDOFrameDownloadSegmentRequest(uint8_t nodeId, uint16_t index = 0,
                                   uint8_t subindex = 0)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::DownloadSegmentRequest);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for upload segment response.
 *
 * +-----+-+-----+-+---------------------------------------------...
 * | scs |t| n   |c|           segment-data 1..7
 * +-----+-+-----+-+---------------------------------------------...
 *
 */
struct SDOFrameUploadSegmentResponse
    : public SDORFrame,
      public SCSMixin<SDOFrameDownloadSegmentRequest>,
      public FrameSegmentMixin<SDOFrameUploadSegmentResponse> {
    SDOFrameUploadSegmentResponse(uint8_t nodeId, uint16_t index = 0,
                                  uint8_t subindex = 0)
        : SDORFrame(nodeId) {
        set_scs(SCS::UploadSegmentResponse);
        setIndexSubIndex(index, subindex);
    }
};
