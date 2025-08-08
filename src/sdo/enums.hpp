#pragma once

#include <cstdint>

/**
 * Enumeration representing the command specifier part in the SDO command byte.
 * See CiA301:2011§7.2.4.3 (p. 49)
 *
 * It concerns the ccs (command code specifier) bits
 */
enum class CCS : uint8_t {
    DownloadSegmentRequest = 0,
    InitiateDownloadRequest = 1,
    InitiateUploadRequest = 2,
    UploadSegmentRequest = 3,
    AbortTransfer = 4,
    BlockUpload = 5,
    BlockDownload = 6,
    Reserved = 7
};

enum class SCS : uint8_t {
    InitiateDownloadResponse = 0,
    DownloadSegmentResponse = 1,
    InitiateUploadResponse = 2,
    UploadSegmentResponse = 3,
    AbortTransfer = 4,
    BlockUploadResponse = 5,
    BlockDownloadResponse = 6,
    Reserved = 7
};

enum class SDOAbortCodes : uint32_t {
    SDOAbortCode_OK = 0,
    SDOAbortCode_CancelWrite = 1,
    SDOAbortCode_ToggleBitNotAlternated = 0x05030000,
    SDOAbortCode_TimedOut = 0x05040000,
    SDOAbortCode_CommandSpecifierInvalid = 0x05040001,
    SDOAbortCode_InvalidBlockSize = 0x05040002,
    SDOAbortCode_InvalidSequenceNumber = 0x05040003,
    SDOAbortCode_CRCError = 0x05040004,
    SDOAbortCode_OutOfMemory = 0x05040005,
    SDOAbortCode_UnsupportedObjectAccess = 0x06010000,
    SDOAbortCode_AttemptReadOnWriteOnly = 0x06010001,
    SDOAbortCode_AttemptWriteOnReadOnly = 0x06010002,
    SDOAbortCode_ObjectNonExistent = 0x06020000,
    SDOAbortCode_CannotMapToPDO = 0x06040041,
    SDOAbortCode_MappedPDOLengthExceeded = 0x06040042,
    SDOAbortCode_GeneralParameterIncompatibility = 0x06040043,
    SDOAbortCode_GeneralInternalIncompatibility = 0x06040047,
    SDOAbortCode_AccessFailedHardwareError = 0x06060000,
    SDOAbortCode_DataTypeMismatch_LengthParameterMismatch = 0x06070010,
    SDOAbortCode_DataTypeMismatch_LengthParameterTooHigh = 0x06070012,
    SDOAbortCode_DataTypeMismatch_LengthParameterTooLow = 0x06070013,
    SDOAbortCode_SubindexNonExistent = 0x06090011,
    SDOAbortCode_InvalidDownloadParameterValue = 0x06090030,
    SDOAbortCode_DownloadValueTooHigh = 0x06090031,
    SDOAbortCode_DownloadValueTooLow = 0x06090032,
    SDOAbortCode_MaxValueLessThanMinimumValue = 0x06090036,
    SDOAbortCode_ResourceNotAvailable = 0x060A0023,
    SDOAbortCode_GeneralError = 0x08000000,
    SDOAbortCode_CannotStoreOrTransfer = 0x08000020,
    SDOAbortCode_CannotStoreOrTransfer_LocalControl = 0x08000021,
    SDOAbortCode_CannotStoreOrTransfer_DeviceState = 0x08000022,
    SDOAbortCode_NoDataAvailable = 0x08000024
};