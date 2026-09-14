#pragma once

#include <stdint.h>

#include "enums.hpp"
#include "unions.hpp"

#define OD_PDO_MAPPING_MAX 8
#define DOMAIN_MAX_SIZE 800

namespace CANopen {
class CanTransport;
class Persistence;
class RemoteObjects;
class EMCY;
class HB;
class PDO;
class SYNC;

/**
 * Route the object dictionary to the hardware it needs.
 * Called once by Node::Node. The dictionary tables are generated as static
 * data, so a program holds exactly one dictionary and one such binding.
 */
void bindHardware(CanTransport &transport, Persistence &persistence,
                  RemoteObjects &remote);

/**
 * Route the objects served by a service to its instance.
 * Called by the application when it attaches the service, or by FullNode.
 * A service that is not bound answers SDOAbortCode_ObjectNonExistent.
 */
void bindEmergency(EMCY &emcy);
void bindHeartbeat(HB &hb);
void bindPdo(PDO &pdo);
void bindSync(SYNC &sync);
}  // namespace CANopen

using namespace CANopen;

/**
 * Read or write an object and wait for a pending remote access to complete.
 * The wait is bounded by REMOTE_ACCESS_TIMEOUT_US on the transport clock.
 * @return 0 on success, 1 on timeout, -1 on failure with abortCode set.
 */
int8_t readDataWait(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t writeDataWait(const Data &data, int32_t id, SDOAbortCodes &abortCode);

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
int8_t getLocalData_string(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_string(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
int8_t getLocalData_domain(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setLocalData_domain(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

/* Objects served by a CANopen service, referenced by the generated tables. */
int8_t odGetSave(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t odSaveData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t odGetRestore(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t odRestoreData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t emcyGetErrorRegister(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t emcyGetErrorField(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t emcySetErrorField(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode);
int8_t emcyGetErrorBehavior(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t emcySetErrorBehavior(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode);
int8_t hbGetData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t hbSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t syncGetData(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t syncSetData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoGetRpdoComm(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoSetRpdoComm(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoGetTpdoComm(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoSetTpdoComm(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoGetRpdoMap(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoSetRpdoMap(const Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoGetTpdoMap(Data &data, int32_t id, SDOAbortCodes &abortCode);
int8_t pdoSetTpdoMap(const Data &data, int32_t id, SDOAbortCodes &abortCode);
