#include "pdo/pdo.hpp"

#include <cstring>

#include "frame.hpp"
#include "od_common.hpp"

using namespace CANopen;

namespace {

PDO *boundPdo = nullptr;

int8_t missingPdo(SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_ObjectNonExistent;
    return -1;
}

bool isRemote(ObjectDictionnary &od, int32_t id) {
    const Metadata *metadata = od.getMetadata(id);
    return metadata != nullptr && metadata->access.bits.remote;
}

}  // namespace

void CANopen::bindPdo(PDO &pdo) { boundPdo = &pdo; }

int8_t pdoGetRpdoComm(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->readComm(PdoDirection::Receive, data, id, abortCode);
}

int8_t pdoSetRpdoComm(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->writeComm(PdoDirection::Receive, data, id, abortCode);
}

int8_t pdoGetTpdoComm(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->readComm(PdoDirection::Transmit, data, id, abortCode);
}

int8_t pdoSetTpdoComm(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->writeComm(PdoDirection::Transmit, data, id, abortCode);
}

int8_t pdoGetRpdoMap(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->readMap(PdoDirection::Receive, data, id, abortCode);
}

int8_t pdoSetRpdoMap(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->writeMap(PdoDirection::Receive, data, id, abortCode);
}

int8_t pdoGetTpdoMap(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->readMap(PdoDirection::Transmit, data, id, abortCode);
}

int8_t pdoSetTpdoMap(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundPdo == nullptr) return missingPdo(abortCode);
    return boundPdo->writeMap(PdoDirection::Transmit, data, id, abortCode);
}

PDO::PDO(ObjectDictionnary &od, CanTransport &transport, RemoteObjects &remote,
         uint8_t nodeId)
    : od(od), transport(transport), remote(remote), nodeId(nodeId) {}

void PDO::init() {
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        tpdos[i].bind(od, PdoDirection::Transmit, i);
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++)
        rpdos[i].bind(od, PdoDirection::Receive, i);
    reload();
}

bool PDO::consumes(FunctionCodes functionCode) const {
    return functionCode >= FunctionCode_TPDO1 &&
           functionCode <= FunctionCode_RPDO4;
}

void PDO::onFrame(Frame &frame, uint32_t now_us) {
    if (!enabled) return;
    const uint16_t cobId = frame.getCobID();
    if (frame.rtr) {
        for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
            if (tpdos[i].isValid() && tpdos[i].canId() == cobId)
                onTransmitRequest(tpdos[i], now_us);
        }
        return;
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        if (rpdos[i].isValid() && rpdos[i].canId() == cobId)
            onReceivedFrame(rpdos[i], frame, now_us);
    }
}

void PDO::onTransmitRequest(PdoChannel &channel, uint32_t now_us) {
    if (channel.transmissionType() == pdo::TRANSMISSION_RTR_SYNC)
        channel.syncFlag = true;
    else if (channel.transmissionType() == pdo::TRANSMISSION_RTR_EVENT)
        send(channel, now_us);
}

void PDO::onReceivedFrame(PdoChannel &channel, const Frame &frame,
                          uint32_t now_us) {
    if (channel.mappedCount() == 0 || frame.dlc < channel.frameSize()) return;
    if (channel.isSynchronous()) {
        memcpy(channel.buffer, frame.data, pdo::DLC);
        channel.syncFlag = true;
    } else {
        unpack(channel, frame.data, now_us);
    }
    if (onReceiveFunc) onReceiveFunc(channel.number() + 1);
}

void PDO::update(uint32_t now_us) {
    if (!enabled) return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
        PdoChannel &channel = tpdos[i];
        if (!channel.isValid() || channel.mappedCount() == 0 ||
            !channel.isEventDriven())
            continue;
        const uint32_t timer_us = channel.eventTimer_us();
        if (timer_us != 0 && now_us - channel.lastEvent_us >= timer_us)
            send(channel, now_us);
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        PdoChannel &channel = rpdos[i];
        if (!channel.isValid() || channel.mappedCount() == 0) continue;
        const uint32_t timer_us = channel.eventTimer_us();
        if (!channel.watchTimeout || timer_us == 0 ||
            now_us - channel.lastEvent_us < timer_us)
            continue;
        channel.watchTimeout = false;
        if (onTimeoutFunc) onTimeoutFunc(channel.number() + 1);
    }
}

void PDO::onSync(uint8_t counter, uint32_t now_us) {
    if (!enabled) return;
    const uint32_t window_us = syncWindow_us();
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
        PdoChannel &channel = tpdos[i];
        if (!channel.isValid() || channel.mappedCount() == 0) continue;
        const uint8_t type = channel.transmissionType();
        bool due = false;
        if (type == pdo::TRANSMISSION_SYNC_ACYCLIC ||
            type == pdo::TRANSMISSION_RTR_SYNC) {
            due = channel.syncFlag;
        } else if (channel.isSynchronous()) {
            if (!channel.syncStarted) {
                if (channel.syncStart() != 0 && counter != channel.syncStart())
                    continue;
                channel.syncStarted = true;
                channel.syncCounter = 0;
            }
            channel.syncCounter++;
            due = channel.syncCounter >= type;
            if (due) channel.syncCounter = 0;
        }
        if (!due) continue;
        if (window_us != 0 && transport.getTime_us() - now_us > window_us)
            break;
        send(channel, now_us);
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        PdoChannel &channel = rpdos[i];
        if (!channel.isSynchronous() || !channel.syncFlag) continue;
        unpack(channel, channel.buffer, now_us);
        channel.syncFlag = false;
    }
}

void PDO::transmitTPDO(unsigned number) {
    if (!enabled || number >= OD_TPDO_COUNT) return;
    PdoChannel &channel = tpdos[number];
    if (!channel.isValid() || channel.mappedCount() == 0) return;
    if (channel.transmissionType() == pdo::TRANSMISSION_SYNC_ACYCLIC) {
        channel.syncFlag = true;
        return;
    }
    if (!channel.isEventDriven()) return;
    const uint32_t now_us = transport.getTime_us();
    if (!inhibited(channel, now_us)) send(channel, now_us);
}

bool PDO::inhibited(const PdoChannel &channel, uint32_t now_us) const {
    const uint32_t inhibit_us = channel.inhibitTime_us();
    return inhibit_us != 0 && now_us - channel.lastEvent_us < inhibit_us;
}

void PDO::send(PdoChannel &channel, uint32_t now_us) {
    Frame frame = Frame::fromCobId(channel.canId());
    frame.dlc = channel.frameSize();
    pack(channel, frame.data);
    channel.syncFlag = false;
    transport.sendFrame(frame);
    channel.lastEvent_us = now_us;
}

void PDO::pack(PdoChannel &channel, uint8_t *payload) {
    Data values[pdo::MAPPING_MAX] = {};
    bool anyRemote = false;
    for (unsigned i = 0; i < channel.mappedCount(); i++)
        anyRemote |= isRemote(od, channel.mappedId(i));
    if (anyRemote)
        remote.getRemoteTPDO(static_cast<uint16_t>(channel.number()), values);
    unsigned offset = 0;
    for (unsigned i = 0; i < channel.mappedCount(); i++) {
        const int32_t id = channel.mappedId(i);
        const uint16_t size = od.getSize(id);
        if (!isRemote(od, id)) od.readData(values[i], id);
        memcpy(payload + offset, &values[i], size);
        offset += size;
    }
}

void PDO::unpack(PdoChannel &channel, const uint8_t *payload, uint32_t now_us) {
    Data values[pdo::MAPPING_MAX] = {};
    bool anyRemote = false;
    unsigned offset = 0;
    for (unsigned i = 0; i < channel.mappedCount(); i++) {
        const int32_t id = channel.mappedId(i);
        const uint16_t size = od.getSize(id);
        memcpy(&values[i], payload + offset, size);
        offset += size;
        if (isRemote(od, id))
            anyRemote = true;
        else
            od.writeData(values[i], id);
    }
    if (anyRemote)
        remote.setRemoteRPDO(static_cast<uint16_t>(channel.number()), values);
    channel.lastEvent_us = now_us;
    channel.watchTimeout = true;
}

uint32_t PDO::syncWindow_us() {
    Data window;
    window.u64 = 0;
    if (od.readData(window, pdo::SYNC_WINDOW_INDEX, 0) != 0) return 0;
    return window.u32 * pdo::SYNC_WINDOW_UNIT_US;
}

void PDO::onNmtState(NMTStates next) {
    if (state == NMTState_Initialisation && next == NMTState_PreOperational)
        reload();
    if (next == NMTState_Operational)
        enable();
    else
        disable();
    state = next;
}

void PDO::enable() {
    const uint32_t now_us = transport.getTime_us();
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) {
        tpdos[i].lastEvent_us = now_us;
        tpdos[i].syncFlag = false;
        tpdos[i].syncStarted = false;
        tpdos[i].syncCounter = 0;
        if (tpdos[i].takeRemapRequest()) applyRemap(tpdos[i]);
    }
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) {
        rpdos[i].lastEvent_us = now_us;
        rpdos[i].syncFlag = false;
        rpdos[i].watchTimeout = false;
        if (rpdos[i].takeRemapRequest()) applyRemap(rpdos[i]);
    }
    enabled = true;
    remote.enablePDO();
}

void PDO::disable() {
    enabled = false;
    remote.disablePDO();
}

void PDO::reload() {
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++) applyRemap(tpdos[i]);
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++) applyRemap(rpdos[i]);
}

void PDO::applyRemap(PdoChannel &channel) {
    channel.remap();
    configureRemote(channel);
}

void PDO::configureRemote(PdoChannel &channel) {
    const uint16_t number = static_cast<uint16_t>(channel.number());
    if (channel.direction() == PdoDirection::Transmit)
        remote.configRemoteTPDO(number, channel.mappedIds());
    else
        remote.configRemoteRPDO(number, channel.mappedIds());
}

PdoChannel *PDO::findComm(PdoDirection direction, int32_t id) {
    const bool transmit = direction == PdoDirection::Transmit;
    PdoChannel *channels = transmit ? tpdos : rpdos;
    const unsigned count = transmit ? tpdoCount() : rpdoCount();
    for (unsigned i = 0; i < count; i++) {
        if (channels[i].ownsCommEntry(id)) return &channels[i];
    }
    return nullptr;
}

PdoChannel *PDO::findMap(PdoDirection direction, int32_t id) {
    const bool transmit = direction == PdoDirection::Transmit;
    PdoChannel *channels = transmit ? tpdos : rpdos;
    const unsigned count = transmit ? tpdoCount() : rpdoCount();
    for (unsigned i = 0; i < count; i++) {
        if (channels[i].ownsMapEntry(id)) return &channels[i];
    }
    return nullptr;
}

int8_t PDO::readComm(PdoDirection direction, Data &data, int32_t id,
                     SDOAbortCodes &abortCode) {
    PdoChannel *channel = findComm(direction, id);
    if (channel == nullptr) return missingPdo(abortCode);
    return channel->readComm(data, id, abortCode);
}

int8_t PDO::writeComm(PdoDirection direction, const Data &data, int32_t id,
                      SDOAbortCodes &abortCode) {
    PdoChannel *channel = findComm(direction, id);
    if (channel == nullptr) return missingPdo(abortCode);
    const int8_t result = channel->writeComm(data, id, abortCode);
    if (result == 0 && enabled && channel->takeRemapRequest())
        applyRemap(*channel);
    return result;
}

int8_t PDO::readMap(PdoDirection direction, Data &data, int32_t id,
                    SDOAbortCodes &abortCode) {
    PdoChannel *channel = findMap(direction, id);
    if (channel == nullptr) return missingPdo(abortCode);
    return channel->readMap(data, id, abortCode);
}

int8_t PDO::writeMap(PdoDirection direction, const Data &data, int32_t id,
                     SDOAbortCodes &abortCode) {
    PdoChannel *channel = findMap(direction, id);
    if (channel == nullptr) return missingPdo(abortCode);
    const int8_t result = channel->writeMap(data, id, abortCode);
    if (result == 0 && enabled && channel->takeRemapRequest())
        applyRemap(*channel);
    return result;
}

void PDO::onReceive(std::function<void(unsigned)> callback) {
    onReceiveFunc = callback;
}

void PDO::onTimeout(std::function<void(unsigned)> callback) {
    onTimeoutFunc = callback;
}
