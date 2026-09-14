#include "pdo/pdoChannel.hpp"

using namespace CANopen;

namespace {

int8_t fail(SDOAbortCodes &abortCode, SDOAbortCodes code) {
    abortCode = code;
    return -1;
}

}  // namespace

void PdoChannel::bind(ObjectDictionnary &dictionary, PdoDirection direction,
                      unsigned number) {
    od = &dictionary;
    dir = direction;
    index = number;
    const bool transmit = direction == PdoDirection::Transmit;
    const uint16_t commIndex = static_cast<uint16_t>(
        (transmit ? pdo::TPDO_COMM_INDEX : pdo::RPDO_COMM_INDEX) + number);
    const uint16_t mapIndex = static_cast<uint16_t>(
        (transmit ? pdo::TPDO_MAP_INDEX : pdo::RPDO_MAP_INDEX) + number);
    commId = od->findObject(commIndex);
    mapId = od->findObject(mapIndex);

    commEntries = 0;
    cobId = pdo::COBID_INVALID;
    transmission = 0;
    inhibit = 0;
    eventTimer = 0;
    syncStartValue = 0;
    if (commId >= 0) {
        commEntries = defaultOf(commId).u8;
        if (commEntries >= pdo::COMM_COBID)
            cobId = defaultOf(commId + pdo::COMM_COBID).u32;
        if (commEntries >= pdo::COMM_TRANSMISSION)
            transmission = defaultOf(commId + pdo::COMM_TRANSMISSION).u8;
        if (commEntries >= pdo::COMM_INHIBIT)
            inhibit = defaultOf(commId + pdo::COMM_INHIBIT).u16;
        if (commEntries >= pdo::COMM_EVENT_TIMER)
            eventTimer = defaultOf(commId + pdo::COMM_EVENT_TIMER).u16;
        if (commEntries >= pdo::COMM_SYNC_START)
            syncStartValue = defaultOf(commId + pdo::COMM_SYNC_START).u8;
    }

    mapEntries = 0;
    for (unsigned i = 0; i < pdo::MAPPING_MAX; i++) mapping[i] = 0;
    if (mapId >= 0) {
        mapEntries = defaultOf(mapId).u8;
        if (mapEntries > pdo::MAPPING_MAX) mapEntries = pdo::MAPPING_MAX;
        for (unsigned i = 0; i < mapEntries; i++)
            mapping[i] = defaultOf(mapId + 1 + static_cast<int32_t>(i)).u32;
    }
    remap();
    remapRequested = false;
}

Data PdoChannel::defaultOf(int32_t id) const {
    const Metadata *metadata = od->getMetadata(id);
    Data data;
    data.u64 = 0;
    if (metadata != nullptr) data = metadata->getDefaultValue();
    return data;
}

bool PdoChannel::ownsCommEntry(int32_t id) const {
    return commId >= 0 && id >= commId && id <= commId + commEntries;
}

bool PdoChannel::ownsMapEntry(int32_t id) const {
    return mapId >= 0 && id >= mapId && id <= mapId + pdo::MAPPING_MAX;
}

int8_t PdoChannel::readComm(Data &data, int32_t id,
                            SDOAbortCodes &abortCode) const {
    if (!ownsCommEntry(id))
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    switch (id - commId) {
        case pdo::COMM_COUNT:
            data.u8 = commEntries;
            return 0;
        case pdo::COMM_COBID:
            data.u32 = cobId;
            return 0;
        case pdo::COMM_TRANSMISSION:
            data.u8 = transmission;
            return 0;
        case pdo::COMM_INHIBIT:
            data.u16 = inhibit;
            return 0;
        case pdo::COMM_RESERVED:
            data.u8 = 0;
            return 0;
        case pdo::COMM_EVENT_TIMER:
            data.u16 = eventTimer;
            return 0;
        case pdo::COMM_SYNC_START:
            data.u8 = syncStartValue;
            return 0;
        default:
            return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    }
}

int8_t PdoChannel::writeComm(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) {
    if (!ownsCommEntry(id))
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    switch (id - commId) {
        case pdo::COMM_COUNT:
            if (data.u8 > pdo::COMM_ENTRIES_MAX)
                return fail(abortCode, SDOAbortCode_DownloadValueTooHigh);
            commEntries = data.u8;
            return 0;
        case pdo::COMM_COBID: {
            // CiA301:2011§7.5.2.35: bits 0 to 30 are frozen while valid.
            const bool staysValid = isValid() && pdo::isValidCobId(data.u32);
            if (staysValid && ((cobId ^ data.u32) & pdo::COBID_LOCKED_MASK))
                return fail(abortCode,
                            SDOAbortCode_InvalidDownloadParameterValue);
            cobId = data.u32;
            remapRequested = true;
            return 0;
        }
        case pdo::COMM_TRANSMISSION:
            if (pdo::isReservedType(data.u8))
                return fail(abortCode,
                            SDOAbortCode_InvalidDownloadParameterValue);
            transmission = data.u8;
            return 0;
        case pdo::COMM_INHIBIT:
            if (isValid())
                return fail(abortCode, SDOAbortCode_UnsupportedObjectAccess);
            inhibit = data.u16;
            return 0;
        case pdo::COMM_RESERVED:
            return 0;
        case pdo::COMM_EVENT_TIMER:
            eventTimer = data.u16;
            return 0;
        case pdo::COMM_SYNC_START:
            if (isValid())
                return fail(abortCode, SDOAbortCode_UnsupportedObjectAccess);
            syncStartValue = data.u8;
            return 0;
        default:
            return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    }
}

int8_t PdoChannel::readMap(Data &data, int32_t id,
                           SDOAbortCodes &abortCode) const {
    if (!ownsMapEntry(id))
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    if (id == mapId) {
        data.u8 = mapEntries;
        return 0;
    }
    data.u32 = mapping[id - mapId - 1];
    return 0;
}

int8_t PdoChannel::writeMap(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    if (!ownsMapEntry(id))
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    if (id == mapId) {
        if (data.u8 > pdo::MAPPING_MAX)
            return fail(abortCode, SDOAbortCode_DownloadValueTooHigh);
        if (mappedSize(data.u8) > pdo::DLC)
            return fail(abortCode, SDOAbortCode_MappedPDOLengthExceeded);
        mapEntries = data.u8;
    } else {
        const SDOAbortCodes check = checkMapEntry(data.u32);
        if (check != SDOAbortCode_OK) return fail(abortCode, check);
        mapping[id - mapId - 1] = data.u32;
    }
    remapRequested = true;
    return 0;
}

SDOAbortCodes PdoChannel::checkMapEntry(uint32_t entry) const {
    if (entry == 0) return SDOAbortCode_OK;
    const Metadata *metadata =
        od->getMetadata(pdo::mappedIndex(entry), pdo::mappedSubindex(entry));
    if (metadata == nullptr) return SDOAbortCode_ObjectNonExistent;
    const bool usable = dir == PdoDirection::Transmit
                            ? metadata->access.bits.readable
                            : metadata->access.bits.writeable;
    if (!metadata->access.bits.mappable || !usable)
        return SDOAbortCode_CannotMapToPDO;
    return SDOAbortCode_OK;
}

uint32_t PdoChannel::mappedSize(uint8_t entries) const {
    uint32_t total = 0;
    for (unsigned i = 0; i < entries; i++) {
        if (mapping[i] == 0) continue;
        total += od->getSize(pdo::mappedIndex(mapping[i]),
                             pdo::mappedSubindex(mapping[i]));
    }
    return total;
}

void PdoChannel::remap() {
    count = 0;
    size = 0;
    for (unsigned i = 0; i < pdo::MAPPING_MAX; i++) ids[i] = -1;
    if (od == nullptr) return;
    uint32_t total = 0;
    for (unsigned i = 0; i < mapEntries; i++) {
        if (mapping[i] == 0) continue;
        const int32_t id = od->findObject(pdo::mappedIndex(mapping[i]),
                                          pdo::mappedSubindex(mapping[i]));
        if (id < 0) continue;
        total += od->getSize(id);
        if (total > pdo::DLC) break;
        ids[count++] = id;
        size = static_cast<uint8_t>(total);
    }
    remapRequested = false;
}

bool PdoChannel::takeRemapRequest() {
    const bool requested = remapRequested;
    remapRequested = false;
    return requested;
}
