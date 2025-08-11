#pragma once

#include "endianUtils.hpp"
#include "enums.hpp"
#include "sdo/enums.hpp"
#include "sdo/mixins.hpp"

/**
 * SDO frame abort transfer.
 * CiA 301:2011 §7.2.4.3.17 p. 61
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │     4..7      │
 * ├─────┬─────────┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │         │             index             │   subindex    │      d        │
 * │1 0 0│x x x x x│            uint16_t           │   uint8_t     │   uint32_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 *
 * cs: Command Specifier (CCS or SCS with value 4)
 * m: Multiplexer (index + subindex)
 * d: Data field containing the SDO abort code.
 */
struct SDOFrameAbort : public SDOFrame,
                       public CSMixin<SDOFrameAbort>,
                       public IndexSubindexMixin<SDOFrameAbort> {
    SDOFrameAbort(uint16_t index = 0, uint8_t subindex = 0,
                  SDOAbortCodes abortCode = SDOAbortCodes::SDOAbortCode_OK) {
        set_cs(CS::AbortTransfer);
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
 * SDO frame for download initiate request.
 * CiA 301:2011 §7.2.4.3.3 p. 49
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │     4..7      │
 * ├─────┬─┬───┬─┬─┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │ │ n │e│s│             index             │   subindex    │      d        │
 * │0 0 1│x│   │ │ │            uint16_t           │   uint8_t     │   uint32_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 * e: Expedited transfer bit
 * s: Size specified bit
 * n: Size of unused bytes field
 */
struct SDOFrameInitiateDownloadRequest
    : public SDOFrame,
      public InitiateDataMixin<SDOFrameInitiateDownloadRequest> {
    SDOFrameInitiateDownloadRequest(uint16_t index = 0, uint8_t subindex = 0) {
        set_cs(CS::ClientInitiateDownloadRequest);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for upload response.
 * CiA 301:2011 §7.2.4.3.3 p. 49
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │     4..7      │
 * ├─────┬─┬───┬─┬─┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │ │ n │e│s│             index             │   subindex    │      d        │
 * │0 1 0│x│   │ │ │            uint16_t           │   uint8_t     │   uint32_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 */
struct SDOFrameInitiateUploadResponse
    : public SDOFrame,
      public InitiateDataMixin<SDOFrameInitiateDownloadRequest> {
    SDOFrameInitiateUploadResponse(uint16_t index = 0, uint8_t subindex = 0) {
        set_cs(CS::ServerInitiateUploadResponse);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for download response.
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │
 * ├─────┬─────────┼───────────────┴───────────────┼───────────────┤
 * │  cs │         │             index             │   subindex    │
 * │0 1 1│x x x x x│            uint16_t           │   uint8_t     │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┘
 */
struct SDOFrameInitiateDownloadResponse
    : public SDOFrame,
      public IndexSubindexMixin<SDOFrameInitiateDownloadResponse>,
      public CSMixin<SDOFrameInitiateDownloadResponse> {
    SDOFrameInitiateDownloadResponse(uint16_t index = 0, uint8_t subindex = 0) {
        set_cs(CS::ServerInitiateDownloadResponse);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for upload request.
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │
 * ├─────┬─────────┼───────────────┴───────────────┼───────────────┤
 * │  cs │         │             index             │   subindex    │
 * │0 1 0│x x x x x│            uint16_t           │   uint8_t     │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┘
 */
struct SDOFrameInitiateDownloadResponse
    : public SDOFrame,
      public CSMixin<SDOFrameInitiateDownloadResponse>,
      public IndexSubindexMixin<SDOFrameInitiateDownloadResponse> {
    SDOFrameInitiateDownloadResponse(uint16_t index = 0, uint8_t subindex = 0) {
        set_cs(CS::ClientInitiateUploadRequest);
        setIndexSubIndex(index, subindex);
    }
};

/**
 * SDO frame for download segment request.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬─┬─────┬─┼───────────────────────────────────────────────┤
 * │  cs │t│  n  │c│                   seg-data                    │
 * │0 0 0│ │     │ │                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameDownloadSegmentRequest
    : public SDOFrame,
      public CSMixin<SDOFrameDownloadSegmentRequest>,
      public FrameSegmentMixin<SDOFrameDownloadSegmentRequest> {
    SDOFrameDownloadSegmentRequest() {
        set_cs(CS::ClientDownloadSegmentRequest);
    }
};

/**
 * SDO frame for upload segment response.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬─┬─────┬─┼───────────────────────────────────────────────┤
 * │  cs │t│  n  │c│                   seg-data                    │
 * │0 0 0│ │     │ │                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameUploadSegmentResponse
    : public SDOFrame,
      public CSMixin<SDOFrameUploadSegmentResponse>,
      public FrameSegmentMixin<SDOFrameUploadSegmentResponse> {
    SDOFrameUploadSegmentResponse() {
        set_cs(CS::ServerUploadSegmentResponse);
    }
};


/**
 * SDO frame for download segment response.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬─┬───────┼───────────────────────────────────────────────┤
 * │  cs │t│       │                   reserved                    │
 * │0 0 1│ │x x x x│                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameDownloadSegmentResponse
    : public SDOFrame,
      public CSMixin<SDOFrameDownloadSegmentResponse>,
      public ToggleBitMixin<SDOFrameDownloadSegmentResponse> {
    SDOFrameDownloadSegmentResponse() {
        set_cs(CS::ServerDownloadSegmentResponse);
    }
};

/**
 * SDO frame for upload segment request.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬─┬───────┼───────────────────────────────────────────────┤
 * │  cs │t│       │                   reserved                    │
 * │0 1 1│ │x x x x│                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameDownloadSegmentRequest
    : public SDOFrame,
      public CSMixin<SDOFrameDownloadSegmentRequest>,
      public ToggleBitMixin<SDOFrameDownloadSegmentRequest> {
    SDOFrameDownloadSegmentRequest() {
        set_cs(CS::ClientUploadSegmentRequest);
    }
};


/**
 * SDO Block download initiate request.
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │     4..7      │
 * ├─────┬───┬─┬─┬─┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │   │R│s│C│             index             │   subindex    │     size      │
 * │1 1 0│x x│ │ │0│            uint16_t           │   uint8_t     │   uint32_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 *
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * s: 0: size not specified, 1: size specified
 * C (Client Subcommand): 0: initiate download request
 */
struct SDOFrameBlockDownloadInitiateRequest
    : public SDOFrame,
      public CS<SDOFrameBlockDownloadInitiateRequest>,
      public CRCBitMixin<SDOFrameBlockDownloadInitiateRequest>,
      public TransferSizeMixin<SDOFrameBlockDownloadInitiateRequest>,
      public IndexSubindexMixin<SDOFrameBlockDownloadInitiateRequest> {
    SDOFrameBlockDownloadInitiateRequest(uint16_t index = 0,
                                         uint8_t subindex = 0) {
        set_cs(CS::ClientBlockDownload);
    }
};

/**
 * SDO Block upload initiate response.
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │     4..7      │
 * ├─────┬───┬─┬─┬─┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │   │R│s│S│             index             │   subindex    │     size      │
 * │1 1 0│x x│ │ │0│            uint16_t           │   uint8_t     │   uint32_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 *
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * s: 0: size not specified, 1: size specified
 * C (Client Subcommand): 0: initiate download request
 */
struct SDOFrameBlockUploadnitiateResponse
    : public SDOFrame,
      public CS<SDOFrameBlockUploadnitiateResponse>,
      public CRCBitMixin<SDOFrameBlockUploadnitiateResponse>,
      public TransferSizeMixin<SDOFrameBlockUploadnitiateResponse>,
      public IndexSubindexMixin<SDOFrameBlockUploadnitiateResponse> {
    SDOFrameBlockUploadnitiateResponse(uint16_t index = 0,
                                         uint8_t subindex = 0) {
        set_cs(CS::ServerBlockUploadResponse);
    }
};

/**
 * SDO Block download initiate response.
 *
 * ┌───────────────┬───────────────┬───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │       3       │       4       │
 * ├─────┬───┬─┬───┼───────────────┴───────────────┼───────────────┼───────────────┤
 * │  cs │   │R│ S │             index             │   subindex    │    blksize    │
 * │1 0 1│x x│ │0 0│            uint16_t           │   uint8_t     │    uint8_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴───────────────┴───────────────┘
 *
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * S (Server Subcommand): 0: initiate download response
 */
struct SDOFrameBlockDownloadInitiateResponse
    : public SDOFrame,
      public CS<SDOFrameBlockDownloadInitiateResponse>,
      public BlockSizeMixin<SDOFrameBlockDownloadInitiateResponse>,
      public IndexSubindexMixin<SDOFrameBlockDownloadInitiateResponse> {
    SDOFrameBlockDownloadInitiateResponse(uint16_t index = 0,
                                        uint8_t subindex = 0) {
        set_cs(CS::ServerBlockDownloadResponse);
    }
};

/**
 * SDO Block upload initiate request.
 *
 * ┌───────────────┬───────────────┬───────────────┬──────────┬─────────┬─────┐
 * │       0       │       1       │      2        │     3    │    4    │  5  │
 * ├─────┬───┬─┬───┼───────────────┴───────────────┼──────────┼─────────┼─────┤
 * │  cs │   │R│ C │             index             │ subindex │ blksize │ pst │
 * │1 0 1│x x│ │0 0│            uint16_t           │ uint8_t  │ uint8_t │     │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┴──────────┴─────────┴─────┘
 *
 * R (Client CRC): 0: no CRC support, 1: CRC support
 * C (Client Subcommand): 0: initiate upload request
 */
struct SDOFrameBlockUploadInitiateRequest
    : public SDOFrame,
      public CS<SDOFrameBlockUploadInitiateRequest>,
      public BlockSizeMixin<SDOFrameBlockUploadInitiateRequest>,
      public PstMixin<SDOFrameBlockUploadInitiateRequest>,
      public IndexSubindexMixin<SDOFrameBlockUploadInitiateRequest> {
    SDOFrameBlockUploadInitiateRequest(uint16_t index = 0,
                                        uint8_t subindex = 0) {
        set_cs(CS::ClientBlockUpload);
    }
};

/**
 * SDO Block upload start request.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬───┬─┬───┼───────────────────────────────────────────────┤
 * │  cs │   │R│ C │                   reserved                    │
 * │1 0 1│x x│ │1 1│                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameBlockUploadStartRequest
    : public SDOFrame,
      public CS<SDOFrameBlockUploadStartRequest> {
    SDOFrameBlockUploadStartRequest() {
        set_cs(CS::ClientBlockUpload);
    }
};

/**
 * SDO Block sub-block.
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─┬─────────────┼───────────────────────────────────────────────┤
 * │c│    seqno    │                   seg-data                    │
 * │ │             │                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 * c: Final transfer bit, 1 if this is the last subblock.
 */
struct SDOFrameBlockSubBlockData
    : public SDOFrame,
      public FinalTransferMixin<SDOFrameBlockSubBlockData, 7> {
    SDOFrameBlockSubBlockData(uint8_t seqno) {
        setSequenceNumber(seqno);
    }

    void setData(const uint8_t* data, uint8_t size) {
        size = size > 7 ? 7 : size;  // Limit size to 7 bytes
        memcpy(bytes() + 1, data, size);
    }

    const uint8_t* getDataPtr() const { return bytes() + 1; }
};

/**
 * SDO Block download sub-block response.
 *
 * ┌───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │
 * ├─────┬─────┬───┼───────────────┼───────────────┤
 * │  cs │     │ S │    acqseq     │    blksize    │
 * │1 0 1│x x x│1 0│    uint8_t    │    uint8_t    │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────┴───────────────┘
 *
 */
struct SDOFrameBlockSubBlockConfirmation
    : public SDOFrame,
      public AcqSeqMixin<SDOFrameBlockSubBlockConfirmation>,
      public BlockSizeMixin<SDOFrameBlockSubBlockConfirmation, 2> {
    SDOFrameBlockSubBlockConfirmation(uint8_t acqseq) {
        setAcknowledgeSequence(acqseq);
        set_cs(CS::ServerBlockDownloadResponse);
    }
};

/**
 * SDO frame block end
 * CiA 301:2011 §7.2.4.3.11 p. 55
 *
 * ┌───────────────┬───────────────┬───────────────┐
 * │       0       │       1       │      2        │
 * ├─────┬─────┬─┬─┼───────────────┴───────────────┤
 * │  cs │  n  │ │C│              CRC              │
 * │1 1 0│     │x│1│            uint16_t           │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────┘
 */
struct SDOFrameBlockEndRequest
    : public SDOFrame,
      public UnusedBytes3Mixin<SDOFrameBlockEndRequest, 2>,
      public CRCMixin<SDOFrameBlockEndRequest> {
    SDOFrameBlockEndRequest(uint16_t crc = 0) {
        setCRC(crc);
        setCS(CS::ClientBlockDownload); // Same value for both direction
    }
};

/**
 * SDO Block download end confirmation
 *
 * ┌───────────────┬───────────────────────────────────────────────┐
 * │       0       │                     1..7                      │
 * ├─────┬─────┬───┼───────────────────────────────────────────────┤
 * │  cs │     │ S │                   reserved                    │
 * │1 0 1│x x x│0 1│                                               │
 * └─┴─┴─┴─┴─┴─┴─┴─┴───────────────────────────────────────────────┘
 */
struct SDOFrameBlockEndConfirmation
    : public SDOFrame {
    SDOFrameBlockEndConfirmation() {
        set_cs(CS::ServerBlockDownloadResponse);
    }
};