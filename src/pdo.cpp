/**
 * Contains the definitions of the PDO class.
 */
#include "pdo.hpp"

#include <cstring>

#include "enums.hpp"
#include "frame.hpp"
#include "node.hpp"
using namespace CANopen;

MapParameter::MapParameter(int16_t index) {
    odID = node.od().findObject(index);
    if (odID < 0) {
        entriesNumber = 0;
    } else {
        Data tmp;
        SDOAbortCodes abortCode;
        getLocalData_uint8_t(tmp, odID, abortCode);
        entriesNumber = tmp.u8;
        for (uint8_t i = 0; i < entriesNumber; i++) {
            getLocalData_uint32_t(tmp, odID + i + 1, abortCode);
            mappedObjects[i] = tmp.u32;
        }
    }
}

MapParameter::MapParameter() {
    odID = -1;
    entriesNumber = 0;
    for (uint8_t i = 0; i < OD_PDO_MAPPING_MAX; i++) {
        mappedObjects[i] = -1;
    }
}

uint8_t MapParameter::getCount() { return entriesNumber; }

uint32_t MapParameter::getMappedValue(uint8_t entry) {
    return mappedObjects[entry];
}

int8_t MapParameter::getData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCodes::SDOAbortCode_OK;
    if (id == odID) {
        data.u8 = entriesNumber;
        return 0;
    } else if (id > odID && id < odID + entriesNumber + 1) {
        data.u32 = mappedObjects[id - odID - 1];
        return 0;
    } else {
        data.u32 = 0;
        abortCode = SDOAbortCodes::SDOAbortCode_SubindexNonExistent;
        return -1;
    }
}

int8_t MapParameter::setData(Data data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCodes::SDOAbortCode_OK;
    if (id == odID) {
        uint8_t value = data.u8;
        if (value > OD_PDO_MAPPING_MAX) {
            abortCode = SDOAbortCodes::SDOAbortCode_DownloadValueTooHigh;
            return -1;
        }
        if (value > X1A00_MAP_DISABLED) {
            uint32_t sizeSum = 0;
            for (unsigned i = 0; i < value; i++) {
                PDOMapEntry entry = {getMappedValue(i)};
                sizeSum +=
                    node.od().getSize(entry.bits.index, entry.bits.subindex);
            }
            if (sizeSum > PDO_DLC) {
                abortCode = SDOAbortCodes::SDOAbortCode_MappedPDOLengthExceeded;
                return -1;
            }
        }
        entriesNumber = value;
    } else {
        PDOMapEntry entry;
        entry.value = data.u32;
        if (!node.od().isSubValid(entry.bits.index, entry.bits.subindex)) {
            abortCode = SDOAbortCodes::SDOAbortCode_ObjectNonExistent;
            entry.value = 0;
        } else {
            Metadata meta =
                *node.od().getMetadata(entry.bits.index, entry.bits.subindex);
            if (!meta.access.bits.mappable || !meta.access.bits.readable) {
                abortCode = SDOAbortCodes::SDOAbortCode_CannotMapToPDO;
                return -1;
            }
        }
        mappedObjects[id - odID - 1] = entry.value;
    }
    abortCode = SDOAbortCodes::SDOAbortCode_OK;
    return 0;
}

CANopen::MapParameter &CANopen::MapParameter::operator=(
    const MapParameter &other) {
    entriesNumber = other.entriesNumber;
    odID = other.odID;
    for (uint8_t i = 0; i < entriesNumber; i++) {
        mappedObjects[i] = other.mappedObjects[i];
    }
    return *this;
}

CommParameter::CommParameter(int16_t index) {
    odID = node.od().findObject(index);
    if (odID < 0) {
        entriesNumber = 6;
        cobId = 0;
        transmissionType = 0;
        inhibitTime = 0;
        eventTimer = 0;
        syncStartValue = 0;  // TODO: add default value by define

    } else {
        SDOAbortCodes abortCode;
        Data tmp;
        getLocalData_uint8_t(tmp, odID, abortCode);
        entriesNumber = tmp.u8;
        getLocalData_uint32_t(tmp, odID + 1, abortCode);
        cobId = tmp.u32;
        getLocalData_uint8_t(tmp, odID + 2, abortCode);
        transmissionType = tmp.u8;
        // getLocalData_uint32_t(tmp, odID + 3, abortCode); //not supported
        // inhibitTime = tmp.u32;
        inhibitTime = 0;
        getLocalData_uint32_t(tmp, odID + 4, abortCode);
        compatibilityEntry = tmp.u32;
        getLocalData_uint32_t(tmp, odID + 5, abortCode);
        eventTimer = tmp.u32;
        getLocalData_uint8_t(tmp, odID + 6, abortCode);
        syncStartValue = tmp.u8;
    }
}

CommParameter::CommParameter() {
    odID = -1;
    entriesNumber = 6;
    cobId = 0;
    transmissionType = 0;
    inhibitTime = 0;
    eventTimer = 0;
    syncStartValue = 0;
}

uint32_t CommParameter::getCobId() { return cobId; }

uint16_t CommParameter::getActualCobId() { return cobId & COBID_MASK; }

uint8_t CommParameter::getTransmissionType() { return transmissionType; }

uint16_t CommParameter::getInhibitTime_us() { return inhibitTime * 100; }

uint16_t CommParameter::getEventTimer_us() { return eventTimer * 1000; }

uint8_t CommParameter::getSyncStart() { return syncStartValue; }

bool CommParameter::isSynchronous() { return transmissionType <= SYNC_MAX; }

bool CommParameter::isTimerSupported() { return entriesNumber >= INDEX_EVENT; }

bool CommParameter::isInhibitSupported() {
    return entriesNumber >= INDEX_INHIBIT;
}

bool CommParameter::isEnabled() { return ~cobId & COBID_VALID_MASK; }

int8_t CommParameter::getData(Data &data, int32_t id,
                              SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCodes::SDOAbortCode_OK;
    if (id - odID > entriesNumber) {
        abortCode = SDOAbortCodes::SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (id == odID) {
        data.u8 = entriesNumber;
        return 0;
    } else if (id == odID + INDEX_COBID) {
        data.u32 = cobId;
        return 0;
    } else if (id == odID + INDEX_TRANSMISSION) {
        data.u8 = transmissionType;
        return 0;
    } else if (id == odID + INDEX_INHIBIT) {
        data.u16 = inhibitTime;
        return 0;
    } else if (id == odID + INDEX_EVENT) {
        data.u16 = eventTimer;
        return 0;
    } else if (id == odID + INDEX_SYNC) {
        data.u8 = syncStartValue;
        return 0;
    } else {
        abortCode = SDOAbortCodes::SDOAbortCode_SubindexNonExistent;
        return -1;
    }
}

int8_t CommParameter::setData(Data data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCodes::SDOAbortCode_OK;
    bool enabled = isEnabled();
    if (id - odID > entriesNumber) {
        abortCode = SDOAbortCodes::SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (id == odID) {
        uint8_t value = data.u8;
        if (value > OD_PDO_COMM_PARAM_MAX) {
            abortCode = SDOAbortCodes::SDOAbortCode_DownloadValueTooHigh;
            return -1;
        }
        entriesNumber = value;
    } else if (id == odID + INDEX_COBID) {
        // PDOCobidEntry current = {cobId};
        // PDOCobidEntry dataEntry = {data.u32};
        // Check if bits 0 to 30 are modified
        //        if (enabled && (current.value ^ dataEntry.value) &
        //        ~COBID_VALID_MASK) { //FIXME: don't call at restore or del if
        //        not necessary
        //            abortCode =
        //                SDOAbortCodes::SDOAbortCode_InvalidDownloadParameterValue;
        //            return -1;
        //        }
        // If a PDO was enabled
        //        if (current.bits.valid && dataEntry.bits.valid) { //TODO: dont
        //        work all time identify why
        remap = true;
        //        }
        cobId = data.u32;
    } else if (id == odID + INDEX_TRANSMISSION) {
        if (SYNC_MAX < data.u8 && data.u8 < RTR_SYNC) {
            abortCode =
                SDOAbortCodes::SDOAbortCode_InvalidDownloadParameterValue;
            return -1;
        }
        transmissionType = data.u8;
    } else if (id == odID + INDEX_INHIBIT) {
        if (enabled) {
            abortCode = SDOAbortCodes::SDOAbortCode_UnsupportedObjectAccess;
            return -1;
        }
        inhibitTime = data.u16;
    } else if (id == odID + INDEX_EVENT) {
        eventTimer = data.u16;
    } else if (id == odID + INDEX_SYNC) {
        if (enabled) {
            abortCode = SDOAbortCodes::SDOAbortCode_UnsupportedObjectAccess;
            return -1;
        }
    } else {
        abortCode = SDOAbortCodes::SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    abortCode = SDOAbortCodes::SDOAbortCode_OK;

    if (remap) {
        node.pdo().reloadTPDO();
        node.pdo().reloadRPDO();
    }
    return 0;
}

CommParameter &CommParameter::operator=(const CommParameter &other) {
    entriesNumber = other.entriesNumber;
    odID = other.odID;
    cobId = other.cobId;
    transmissionType = other.transmissionType;
    inhibitTime = other.inhibitTime;
    compatibilityEntry = other.compatibilityEntry;
    eventTimer = other.eventTimer;
    syncStartValue = other.syncStartValue;
    return *this;
}

PDO::PDO() {}

void PDO::init() {
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) initTPDO(i);
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) initRPDO(i);
}

void PDO::enable() {
    enabled = true;
    node.hardware().enablePDO();
}

void PDO::disable() {
    enabled = false;
    node.hardware().disablePDO();
}

void PDO::initTPDO(unsigned index) {
    tpdos[index].commParameter =
        CommParameter(TPDO_COMMUNICATION_INDEX + index);
    tpdos[index].mapParameter = MapParameter(TPDO_MAPPING_INDEX + index);
    remapTPDO(index);
}

void PDO::initRPDO(unsigned index) {
    rpdos[index].commParameter =
        CommParameter(RPDO_COMMUNICATION_INDEX + index);
    rpdos[index].mapParameter = MapParameter(RPDO_MAPPING_INDEX + index);
    remapRPDO(index);
}

void PDO::remapTPDO(unsigned index) {
    TPDO *tpdo = tpdos + index;
    // unsigned count = tpdo->mapParameter.getCount();
    uint32_t sizeSum = tpdo->size = tpdo->count = 0;
    for (unsigned i = 0; i < OD_PDO_MAPPING_MAX; i++) {
        PDOMapEntry content = {tpdo->mapParameter.getMappedValue(i)};
        int32_t id =
            node.od().findObject(content.bits.index, content.bits.subindex);
        sizeSum += node.od().getSize(id);
        if (sizeSum > PDO_DLC) break;
        tpdo->mappedEntries[i] = id;
        tpdo->size = sizeSum;
        if (id != -1) tpdo->count++;
    }
    node.hardware().configRemoteTPDO(
        index, tpdo->mappedEntries);  // assuming all TPDOs are remote TODO: add
                                      // check if remote
}

void PDO::remapRPDO(unsigned index) {
    RPDO *rpdo = rpdos + index;
    // unsigned count = rpdo->mapParameter.getCount();
    uint32_t sizeSum = rpdo->size = rpdo->count = 0;
    for (unsigned i = 0; i < OD_PDO_MAPPING_MAX; i++) {
        PDOMapEntry content = {rpdo->mapParameter.getMappedValue(i)};
        int32_t id =
            node.od().findObject(content.bits.index, content.bits.subindex);
        sizeSum += node.od().getSize(id);
        if (sizeSum > PDO_DLC) break;
        rpdo->mappedEntries[i] = id;
        rpdo->size = sizeSum;
        if (id != -1) rpdo->count++;
    }
    node.hardware().configRemoteRPDO(index, rpdo->mappedEntries);
}

void PDO::bufferizeTPDO(unsigned index, uint8_t *buffer) {
    TPDO *tpdo = tpdos + index;
    uint32_t bytesTransferred = 0;
    Data tmp[OD_PDO_MAPPING_MAX];
    node.hardware().getRemoteTPDO(
        index, tmp);  // assuming all TPDOs are remote TODO: add check if remote
    for (unsigned i = 0; i < tpdo->count; i++) {
        int32_t id = tpdo->mappedEntries[i];
        uint32_t size = node.od().getSize(id);
        if (bytesTransferred + size > PDO_DLC) break;
        memcpy(buffer + bytesTransferred, &tmp[i], size);
        bytesTransferred += size;
    }
}

void PDO::unpackRPDO(unsigned index, uint8_t *buffer, uint32_t timestamp_us) {
    RPDO *rpdo = rpdos + index;
    if (!enabled || !rpdo->commParameter.isEnabled() || rpdo->count == 0)
        return;
    uint32_t bytesTransferred = 0;
    Data tmp[OD_PDO_MAPPING_MAX];
    for (unsigned i = 0; i < rpdo->count; i++) {
        int32_t id = rpdo->mappedEntries[i];
        uint32_t size = node.od().getSize(id);
        memcpy(&tmp[i], buffer + bytesTransferred, size);
        bytesTransferred += size;
    }
    node.hardware().setRemoteRPDO(index, tmp);
    rpdo->timestamp_us = timestamp_us;
    rpdo->watchTimeoutFlag = true;
}

void PDO::sendTPDO(unsigned index, uint32_t timestamp_us) {
    TPDO *tpdo = tpdos + index;
    Frame frame = Frame::fromCobId(tpdo->commParameter.getActualCobId());
    frame.dlc = tpdo->size;
    bufferizeTPDO(index, frame.data);
    tpdo->syncFlag = false;
    node.hardware().sendFrame(frame);
    tpdo->timestamp_us = timestamp_us;
}

void PDO::receiveTPDO(Frame &frame, uint32_t timestamp_us) {
    if (!enabled || frame.nodeId != node.nodeId || !frame.rtr) return;
    uint8_t index;
    switch ((FunctionCodes)frame.functionCode) {
        case FunctionCode_TPDO1:
            index = 0;
            break;
        case FunctionCode_TPDO2:
            index = 1;
            break;
        case FunctionCode_TPDO3:
            index = 2;
            break;
        case FunctionCode_TPDO4:
            index = 3;
            break;
        default:
            return;
    }
    TPDO *tpdo = tpdos + index;
    uint8_t transmission = tpdo->commParameter.getTransmissionType();
    if (transmission == RTR_SYNC)
        tpdo->syncFlag = true;
    else if (transmission == RTR_EVENT)
        sendTPDO(index, timestamp_us);
}

void PDO::receiveRPDO(Frame &frame, uint32_t timestamp_us) {
    if (!enabled || frame.nodeId != node.nodeId) return;
    uint8_t index;
    switch ((FunctionCodes)frame.functionCode) {
        case FunctionCode_RPDO1:
            index = 0;
            break;
        case FunctionCode_RPDO2:
            index = 1;
            break;
        case FunctionCode_RPDO3:
            index = 2;
            break;
        case FunctionCode_RPDO4:
            index = 3;
            break;
        default:
            return;
    }
    RPDO *rpdo = rpdos + index;
    if (rpdo->size != frame.dlc) return;
    uint8_t transmission = rpdo->commParameter.getTransmissionType();
    if (transmission <= SYNC_MAX) {
        memcpy(rpdo->buffer, frame.data, frame.dlc);
        rpdo->syncFlag = true;
    } else
        unpackRPDO(index, frame.data, timestamp_us);
    if (onReceiveFunc) onReceiveFunc(index + 1);
}

void PDO::update(uint32_t timestamp_us) {
    if (!enabled) return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
        if (!tpdos[i].commParameter.isEnabled() || tpdos[i].count == 0)
            continue;
        TPDO *tpdo = tpdos + i;
        uint8_t transmission = tpdo->commParameter.getTransmissionType();
        if ((transmission != EVENT1 && transmission != EVENT2) ||
            !tpdo->commParameter.isTimerSupported())
            continue;
        uint32_t timer_us = tpdo->commParameter.getEventTimer_us();
        if (timer_us == 0 || timestamp_us - tpdo->timestamp_us < timer_us)
            continue;
        // Only event-driven PDOs can be sent periodically
        sendTPDO(i, timestamp_us);
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        if (!rpdos[i].commParameter.isEnabled() || rpdos[i].count == 0)
            continue;
        RPDO *rpdo = rpdos + i;
        if (!rpdo->commParameter.isTimerSupported()) continue;
        uint32_t timer_us = rpdo->commParameter.getEventTimer_us();
        if (!rpdo->watchTimeoutFlag || timer_us == 0 ||
            timestamp_us - rpdo->timestamp_us < timer_us)
            continue;
        rpdo->watchTimeoutFlag = false;
        if (onTimeoutFunc) onTimeoutFunc(i + 1);
    }
}

void PDO::onSync(uint8_t counter, uint32_t timestamp_us) {
    if (!enabled) return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
        TPDO *tpdo = tpdos + i;
        if (!tpdo->commParameter.isSynchronous()) continue;
        uint8_t transmission = tpdo->commParameter.getTransmissionType();
        // synchronous acyclic || synchronous cyclic || RTR synchronous
        bool send = transmission == 0 ||
                    (transmission <= SYNC_MAX &&
                     (counter && !(counter % transmission))) ||
                    (transmission == RTR_SYNC && tpdo->syncFlag);
        if (send) {
            uint32_t syncWindow = getSyncWindow_us();
            if (syncWindow != 0 &&
                node.hardware().getTime_us() - timestamp_us > syncWindow)
                break;
            sendTPDO(i, timestamp_us);
        }
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        RPDO *rpdo = rpdos + i;
        if (!rpdo->commParameter.isSynchronous() || !rpdo->syncFlag) continue;
        unpackRPDO(i, rpdo->buffer, timestamp_us);
        rpdo->syncFlag = false;
    }
}

uint32_t PDO::getSyncWindow_us() {
    uint32_t value = 0;
    Data tmp;
    if (node.od().isSubValid(0x1007, 0x00)) {
        node.od().readData(tmp, 0x1007, 0x00);
        value = tmp.u32;
    };
    return value * 1000;
}

void PDO::transmitTPDO(unsigned index) {
    if (!enabled || index >= OD_TPDO_COUNT) return;
    TPDO *tpdo = tpdos + index;
    uint8_t transmission = tpdo->commParameter.getTransmissionType();
    if (transmission == ACYCLIC) {
        tpdo->syncFlag = true;
    } else if (transmission >= EVENT1) {
        uint32_t timestamp_us = node.hardware().getTime_us();
        bool supported = tpdo->commParameter.isInhibitSupported();
        if (!supported ||
            (supported && (tpdo->commParameter.getInhibitTime_us() == 0 ||
                           timestamp_us - tpdo->timestamp_us >=
                               tpdo->commParameter.getInhibitTime_us()))) {
            sendTPDO(index, timestamp_us);
        }
    }
}

void PDO::reloadTPDO() {
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) remapTPDO(i);
}

void PDO::reloadRPDO() {
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) remapRPDO(i);
}

void PDO::onReceive(std::function<void(unsigned)> callback) {
    onReceiveFunc = callback;
}

void PDO::onTimeout(std::function<void(unsigned)> callback) {
    onTimeoutFunc = callback;
}
