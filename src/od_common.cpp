#include "od_common.hpp"

#include "hal/can-transport.hpp"
#include "hal/persistence.hpp"
#include "hal/remote-objects.hpp"
#include "od.hpp"
#include "od/parameterGroup.hpp"
#include "od_lookup.hpp"

namespace {
constexpr uint16_t SAVE_INDEX = 0x1010;
constexpr uint32_t SAVE_SIGNATURE = 0x65766173;
constexpr uint16_t RESTORE_INDEX = 0x1011;
constexpr uint32_t RESTORE_SIGNATURE = 0x64616F6C;
constexpr int32_t MAX_SUB_INDEX = 255;
constexpr uint32_t REMOTE_ACCESS_TIMEOUT_US = 10000;

CanTransport *clock = nullptr;
Persistence *store = nullptr;
RemoteObjects *remoteObjects = nullptr;
ObjectDictionnary dictionary;
}  // namespace

void CANopen::bindHardware(CanTransport &transport, Persistence &persistence,
                           RemoteObjects &remote) {
    clock = &transport;
    store = &persistence;
    remoteObjects = &remote;
}

int8_t readDataWait(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    const uint32_t start = clock->getTime_us();
    int8_t result;
    while ((result = dictionary.readData(data, id, abortCode)) == 1) {
        if (clock->getTime_us() - start > REMOTE_ACCESS_TIMEOUT_US) break;
    }
    return result;
}

int8_t writeDataWait(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    const uint32_t start = clock->getTime_us();
    int8_t result;
    while ((result = dictionary.writeData(data, id, abortCode)) == 1) {
        if (clock->getTime_us() - start > REMOTE_ACCESS_TIMEOUT_US) break;
    }
    return result;
}

int8_t odGetSave(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return dictionary.getSave(data, id, abortCode);
}

int8_t odSaveData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return dictionary.saveData(data, id, abortCode);
}

int8_t odGetRestore(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return dictionary.getRestore(data, id, abortCode);
}

int8_t odRestoreData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return dictionary.restoreData(data, id, abortCode);
}

#if OD_BOOL_COUNT > 0
int8_t getLocalData_bool(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.b =
        ObjectDictionnary::boolTable[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_bool(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::boolTable[ObjectDictionnary::dataIndexTable[id]] =
        data.b;
    return 0;
}
#endif

#if OD_I8_COUNT > 0
int8_t getLocalData_int8_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i8 = ObjectDictionnary::i8Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int8_t(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::i8Table[ObjectDictionnary::dataIndexTable[id]] = data.i8;
    return 0;
}
#endif

#if OD_I16_COUNT > 0
int8_t getLocalData_int16_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i16 =
        ObjectDictionnary::i16Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int16_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::i16Table[ObjectDictionnary::dataIndexTable[id]] =
        data.i16;
    return 0;
}
#endif

#if OD_I32_COUNT > 0
int8_t getLocalData_int32_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i32 =
        ObjectDictionnary::i32Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int32_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::i32Table[ObjectDictionnary::dataIndexTable[id]] =
        data.i32;
    return 0;
}
#endif

#if OD_I64_COUNT > 0
int8_t getLocalData_int64_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i64 =
        ObjectDictionnary::i64Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int64_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::i64Table[ObjectDictionnary::dataIndexTable[id]] =
        data.i64;
    return 0;
}
#endif

#if OD_U8_COUNT > 0
int8_t getLocalData_uint8_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u8 = ObjectDictionnary::u8Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint8_t(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::u8Table[ObjectDictionnary::dataIndexTable[id]] = data.u8;
    return 0;
}
#endif

#if OD_U16_COUNT > 0
int8_t getLocalData_uint16_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u16 =
        ObjectDictionnary::u16Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint16_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::u16Table[ObjectDictionnary::dataIndexTable[id]] =
        data.u16;
    return 0;
}
#endif

#if OD_U32_COUNT > 0
int8_t getLocalData_uint32_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u32 =
        ObjectDictionnary::u32Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint32_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::u32Table[ObjectDictionnary::dataIndexTable[id]] =
        data.u32;
    return 0;
}
#endif

#if OD_U64_COUNT > 0
int8_t getLocalData_uint64_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u64 =
        ObjectDictionnary::u64Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint64_t(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::u64Table[ObjectDictionnary::dataIndexTable[id]] =
        data.u64;
    return 0;
}
#endif

#if OD_F32_COUNT > 0
int8_t getLocalData_float(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.f32 =
        ObjectDictionnary::f32Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_float(const Data &data, int32_t id,
                          SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::f32Table[ObjectDictionnary::dataIndexTable[id]] =
        data.f32;
    return 0;
}
#endif

#if OD_F64_COUNT > 0
int8_t getLocalData_double(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.f64 =
        ObjectDictionnary::f64Table[ObjectDictionnary::dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_double(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    ObjectDictionnary::f64Table[ObjectDictionnary::dataIndexTable[id]] =
        data.f64;
    return 0;
}
#endif

int8_t getLocalData_string(Data &, int32_t, SDOAbortCodes &) {
    return -1;  // TODO: implement
}

int8_t setLocalData_string(const Data &, int32_t, SDOAbortCodes &) {
    return -1;  // TODO: implement
}

int8_t getLocalData_domain(Data &, int32_t, SDOAbortCodes &) {
    return -1;  // TODO: implement
}

int8_t setLocalData_domain(const Data &, int32_t, SDOAbortCodes &) {
    return -1;  // TODO: implement
}

int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return remoteObjects->getRemoteData(data, id, abortCode);
}

int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return remoteObjects->setRemoteData(data, id, abortCode);
}

int32_t ObjectDictionnary::findObject(uint16_t index, uint8_t subindex) {
    return phf::find(static_cast<uint32_t>(index << 8) | (subindex & 0xffu));
}

int8_t ObjectDictionnary::readData(Data &data, uint16_t index, uint8_t subindex,
                                   SDOAbortCodes &abortCode) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        abortCode = SDOAbortCodes::SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    return readData(data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, uint16_t index,
                                    uint8_t subindex,
                                    SDOAbortCodes &abortCode) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        abortCode = SDOAbortCodes::SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    return writeData(data, id, abortCode);
}

int8_t ObjectDictionnary::readData(Data &data, int32_t id,
                                   SDOAbortCodes &abortCode) {
    return objectGetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, int32_t id,
                                    SDOAbortCodes &abortCode) {
    return objectSetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::readData(Data &data, uint16_t index,
                                   uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return -1;
    }
    return readData(data, id);
}

int8_t ObjectDictionnary::writeData(const Data &data, uint16_t index,
                                    uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return -1;
    }
    return writeData(data, id);
}

int8_t ObjectDictionnary::readData(Data &data, int32_t id) {
    SDOAbortCodes abortCode;
    return objectGetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, int32_t id) {
    SDOAbortCodes abortCode;
    return objectSetterTable[id](data, id, abortCode);
}

bool ObjectDictionnary::saveData(uint8_t parameterGroup) {
    Persistence &persistence = *store;
    if (parameterGroup == ParameterGroup_All) {
        uint64_t signature = 0;
        if (!persistence.loadSignature(signature) ||
            signature != od_signature) {
            persistence.saveSignature(od_signature);
        }
        bool result = true;
        result &= persistence.saveGroup(ParameterGroup_Communication);
        result &= persistence.saveGroup(ParameterGroup_Application);
        result &= persistence.saveGroup(ParameterGroup_ManufacturerA);
        return result;
    }
    return persistence.saveGroup(parameterGroup);
}

int8_t ObjectDictionnary::saveData(const Data &data, int32_t id,
                                   SDOAbortCodes &abortCode) {
    if (data.u32 != SAVE_SIGNATURE) {
        abortCode = SDOAbortCode_CannotStoreOrTransfer;
        return 0;
    }
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (saveData(static_cast<uint8_t>(id - odID))) {
        return 0;
    } else {
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
}

int8_t ObjectDictionnary::getSave(Data &data, int32_t id,
                                  SDOAbortCodes &abortCode) {
    data.u32 = 0x01;  // bit 1: auto save, bit 0: save at command
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    return 0;
}

bool ObjectDictionnary::loadData(uint8_t parameterGroup) {
    Persistence &persistence = *store;
    uint64_t signature = 0;
    if (!persistence.loadSignature(signature) || signature != od_signature) {
        return false;
    }
    if (parameterGroup == ParameterGroup_All) {
        bool result = true;
        result &= persistence.loadGroup(ParameterGroup_Communication);
        result &= persistence.loadGroup(ParameterGroup_Application);
        result &= persistence.loadGroup(ParameterGroup_ManufacturerA);
        return result;
    }
    return persistence.loadGroup(parameterGroup);
}

bool ObjectDictionnary::restoreData(uint8_t parameterGroup) {
    SDOAbortCodes abortCode;
    for (int32_t i = 0; i < static_cast<int32_t>(length); i++) {
        if (!inParameterGroup(CANopenOD::objectIndexTable[i].first,
                              parameterGroup))
            continue;
        const Data tmp = objectMetadataTable[i].getDefaultValue();
        writeDataWait(tmp, i, abortCode);
    }
    return true;
}

int8_t ObjectDictionnary::restoreData(const Data &data, int32_t id,
                                      SDOAbortCodes &abortCode) {
    if (data.u32 != RESTORE_SIGNATURE) {
        abortCode = SDOAbortCode_CannotStoreOrTransfer;
        return 0;
    }
    int32_t odID = findObject(RESTORE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (restoreData(static_cast<uint8_t>(id - odID))) {
        return 0;
    } else {
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
}

int8_t ObjectDictionnary::getRestore(Data &data, int32_t id,
                                     SDOAbortCodes &abortCode) {
    data.u32 = 0x01;  // bit 0: restore at command
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    return 0;
}

bool ObjectDictionnary::isSubValid(uint16_t index, uint8_t subindex) {
    return findObject(index, subindex) != -1;
}

const Metadata *ObjectDictionnary::getMetadata(uint16_t index,
                                               uint8_t subindex) {
    return getMetadata(findObject(index, subindex));
}

const Metadata *ObjectDictionnary::getMetadata(int32_t id) {
    if (id < 0 || id >= static_cast<int32_t>(length)) {
        return nullptr;
    }
    return &objectMetadataTable[id];
}

uint16_t ObjectDictionnary::getSize(uint16_t index, uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return UINT16_MAX;
    }
    return getSize(id);
}

uint16_t ObjectDictionnary::getSize(int32_t id) {
    if (id < 0 || id >= static_cast<int32_t>(length)) {
        return 0;
    }
    const Metadata &metadata = objectMetadataTable[id];
    return metadata.dataType == DataType::DOMAIN ? DOMAIN_MAX_SIZE
                                                 : metadata.size;
}
