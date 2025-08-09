#pragma once

#include "enums.hpp"
#include "mixins.hpp"

#include "endianUtils.hpp"

/**
 * SDO frame abort transfer.
 * CiA 301:2011 §7.2.4.3.17 p. 61
 *
 * +---------------+---------------+---------------+---------------+----
 * |       0       |       1       |       2       |      3        | 4..7
 * | cs  |         |           index               |   subindex    | d
 * +---------------+-------------------------------+---------------+----
 *
 * cs: Command Specifier (CCS or SCS)
 * m: Multiplexer (index + subindex)
 * d: Data field containing the SDO abort code.
 */
struct SDOFrameAbort : public SDOTFrame,
                       public IndexSubindexMixin<SDOFrameAbort>,
                       public CCSMixin<SDOFrameAbort> {
    SDOFrameAbort(int8_t nodeId, uint16_t index = 0, uint8_t subindex = 0,
                  SDOAbortCodes abortCode = SDOAbortCodes::SDOAbortCode_OK)
        : SDOTFrame(nodeId) {
        set_ccs(CCS::AbortTransfer);
        setIndexSubIndex(index, subindex);
        setAbortCode(abortCode);
    }

   private:
    SDOAbortCodes abortCode() const {
        return static_cast<SDOAbortCodes>(endian::read_le32(bytes() + 4));
    }

    void setAbortCode(SDOAbortCodes code) {
        endian::write_le32(bytes() + 4, static_cast<uint32_t>(code));
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
    SDOFrameDownloadSegmentResponse(uint8_t nodeId) : SDORFrame(nodeId) {
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
    SDOFrameUploadSegmentRequest(uint8_t nodeId) : SDOTFrame(nodeId) {
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
 * |0|1|1| | | | | |
 * +-----+-+-----+-+---------------------------------------------...
 *
 */
struct SDOFrameUploadSegmentResponse
    : public SDORFrame,
      public SCSMixin<SDOFrameDownloadSegmentRequest>,
      public FrameSegmentMixin<SDOFrameUploadSegmentResponse> {
    SDOFrameUploadSegmentResponse(uint8_t nodeId, bool toggle = false)
        : SDORFrame(nodeId) {
        set_scs(SCS::UploadSegmentResponse);
        setToggleBit(toggle);
    }
};

/**
 * SDO Block download initiate request.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       | 4..7
 * | ccs | | |R|s|C|            Index              | Subindex      | Size
 * |1|1|0|x|x| | |0|                               |               |
 * +---------------+---------------+---------------+---------------+---
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * s: 0: size not specified, 1: size specified
 * C (Client Subcommand): 0: initiate download request
 */
struct SDOFrameBlockDownloadInitiateRequest
    : public SDORFrame,
      public CCS<SDOFrameBlockDownloadInitiateRequest>,
      public CRCBitMixin<SDOFrameBlockDownloadInitiateRequest>,
      public BlockSizeMixin<SDOFrameBlockDownloadInitiateRequest>,
      public IndexSubindexMixin<SDOFrameBlockDownloadInitiateRequest> {
    SDOFrameBlockDownloadInitiateRequest(uint8_t nodeId, uint16_t index = 0,
                                         uint8_t subindex = 0) {
        set_ccs(CCS::BlockDownload);
    }
};

/**
 *  SDO Block upload initiate response.
 *
 * +---------------+---------------+---------------+---------------+---
 * |       0       |       1       |       2       |       3       | 4..7
 * | scs | | |R|s|C|            Index              | Subindex      | Size
 * |1|1|0|x|x| | |0|                               |               |
 * +---------------+---------------+---------------+---------------+---
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * s: 0: size not specified, 1: size specified
 * C (Client Subcommand): 0: initiate download request
 */
struct SDOFrameBlockUploadInitiateResponse
    : public SDOTFrame,
      public SCS<SDOFrameBlockUploadInitiateResponse>,
      public CRCBitMixin<SDOFrameBlockUploadInitiateResponse>,
      public BlockSizeMixin<SDOFrameBlockDownloadInitiateRequest>,
      public IndexSubindexMixin<SDOFrameBlockUploadInitiateResponse> {
    SDOFrameBlockUploadInitiateResponse(uint8_t nodeId, uint16_t index = 0,
                                        uint8_t subindex = 0) {
        set_scs(SCS::BlockUploadReponse);
    }
};

/**
 *  SDO Block download initiate response
 *
 * +---------------+---------------+---------------+----------+---------+
 * |       0       |       1       |       2       |       3  |      4  |
 * | scs | | |R| S |            Index              | Subindex | Blksize |
 * |1|0|1|x|x| |0|0|                               |          |         |
 * +---------------+---------------+---------------+----------+---------+
 * R (Server CRC): 0: no CRC support, 1: CRC support
 * S (Server Subcommand): 0: initiate download request
 * Blksize: Size of the block in bytes
 */
struct SDOFrameBlockUploadInitiateResponse
    : public SDORFrame,
      public SCS<SDOFrameBlockUploadInitiateResponse>,
      public BlockSizeMixin<SDOFrameBlockDownloadInitiateRequest>,
      public IndexSubindexMixin<SDOFrameBlockUploadInitiateResponse> {
    SDOFrameBlockUploadInitiateResponse(uint8_t nodeId, uint16_t index = 0,
                                        uint8_t subindex = 0) {
        set_scs(SCS::BlockDownloadResponse);
    }
};

/**
 *  SDO Block upload initial request
 *
 * +---------------+---------------+---------------+----------+---------+-----+
 * |       0       |       1       |       2       |       3  |      4  |  5  |
 * | ccs | | |R| S |            Index              | Subindex | Blksize | Pst |
 * |1|1|0|x|x| |0|0|                               |          |         |     |
 * +---------------+---------------+---------------+----------+---------+-----+
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * S (Client Subcommand): 0: initiate download response
 * Blksize: Size of the block in bytes
 * Pst:
 *   0: No change of transfer protocol,
 *   pst > 0: switch to SDO upload, if data less than.
 */
struct SDOFrameBlockUploadInitiateResponse
    : public SDORFrame,
      public CCS<SDOFrameBlockUploadInitiateResponse>,
      public BlockSizeMixin<SDOFrameBlockDownloadInitiateRequest>,
      public PstMixin<SDOFrameBlockUploadInitiateResponse>,
      public IndexSubindexMixin<SDOFrameBlockUploadInitiateResponse> {
    SDOFrameBlockUploadInitiateResponse(uint8_t nodeId, uint16_t index = 0,
                                        uint8_t subindex = 0) {
        set_ccs(CCS::BlockUpload);
    }
};

/**
 * SDO Block upload/download subblock request.
 *
 * +---------------+-------------...
 * |       0       |       1..7
 * |c| seqno       | Data...
 * | | | | | | | | |
 * +---------------+-------------...
 * Seqno: Sequence number of the subblock, 0 for the first subblock.
 * c: Final transfer bit, 1 if this is the last subblock.
 */
struct SDOFrameUploadSubBlockRequest
    : public SDOTFrame,
      public FinalTransferMixin<SDOFrameUploadSubBlockRequest, 7> {
    SDOFrameUploadSubBlockRequest(uint8_t nodeId, uint8_t seqno)
        : SDOTFrame(nodeId) {
        setFinalTransfer(seqno == 0);
    }

    void setData(const uint8_t* data, uint8_t size) {
        size = size > 7 ? 7 : size; // Limit size to 7 bytes
        memcpy(bytes() + 1, data, size);
    }

    const uint8_t* getDataPtr() const {
        return bytes() + 1;
    }
};

struct SDOFrameDownloadSubBlockRequest
    : public SDORFrame,
      public FinalTransferMixin<SDOFrameDownloadSubBlockRequest, 7> {
    SDOFrameDownloadSubBlockRequest(uint8_t nodeId, uint8_t seqno)
        : SDORFrame(nodeId) {
        setFinalTransfer(seqno == 0);
    }

    void setData(const uint8_t* data, uint8_t size) {
        size = size > 7 ? 7 : size; // Limit size to 7 bytes
        memcpy(bytes() + 1, data, size);
    }

    const uint8_t* getDataPtr() const {
        return bytes() + 1;
    }
};