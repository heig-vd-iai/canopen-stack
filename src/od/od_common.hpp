#pragma once

#include <stdint.h>

#include "enums.hpp"
#include "unions.hpp"

#define OD_GRANULARITY 8
#define OD_PDO_MAPPING_MAX 8
#define OD_ENTRY_SIZE_MAX 1024
#define OBJECT_INDEX_COUNT 0

#define X1A00_MAP_DISABLED 0
#define X1A00_SAM_MPDO 0xFE
#define X1A00_DAM_MPDO 0xFF

#define SAVE_INDEX 0x1010
#define SAVE_SIGNATURE 0x65766173
#define RESTORE_INDEX 0x1011
#define RESTORE_SIGNATURE 0x64616F6C

#define MAX_SUB_INDEX 255
#define MAX_WRITE_TRY 100

#define DOMAIN_MAX_SIZE 800

using namespace CANopen;

int8_t getLocalData_bool(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_bool(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode);
int8_t getLocalData_int8_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int8_t(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
int8_t getLocalData_int16_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int16_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode);
int8_t getLocalData_int32_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int32_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode);
int8_t getLocalData_int64_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_int64_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode);
int8_t getLocalData_uint8_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint8_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode);
int8_t getLocalData_uint16_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint16_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode);
int8_t getLocalData_uint32_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint32_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode);
int8_t getLocalData_uint64_t(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_uint64_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode);
int8_t getLocalData_float(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_float(const Data &data, int32_t id,
                          SDOAbortCodes &abortCode);
int8_t getLocalData_double(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_double(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
int8_t getLocalData_char(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_char(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode);
int8_t getLocalData_domain(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_domain(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
