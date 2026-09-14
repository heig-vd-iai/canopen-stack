#pragma once
#include <cstdint>
#include <deque>
#include <map>
#include <vector>

#include "frame.hpp"
#include "hal/can-transport.hpp"
#include "hal/persistence.hpp"
#include "hal/remote-objects.hpp"
#include "od.hpp"

namespace CANopen {

class FakeHardware : public CanTransport,
                     public Persistence,
                     public RemoteObjects {
   public:
    std::vector<Frame> sent;
    std::deque<Frame> incoming;
    uint32_t now_us = 0;
    std::map<int32_t, Data> remote;
    unsigned remotePendingTurns = 0;
    unsigned remoteResets = 0;
    uint64_t signature = 0;
    bool signatureStored = false;
    std::vector<uint8_t> savedGroups;
    std::vector<uint8_t> loadedGroups;
    std::map<uint16_t, std::vector<int32_t>> remoteTpdo;
    std::map<uint16_t, std::vector<int32_t>> remoteRpdo;
    unsigned pdoEnabled = 0;
    unsigned pdoDisabled = 0;

    void init() override {}
    void sendFrame(const Frame &frame) override { sent.push_back(frame); }
    bool receiveFrame(Frame &frame) override {
        if (incoming.empty()) return false;
        frame = incoming.front();
        incoming.pop_front();
        return true;
    }
    uint32_t getTime_us() override { return now_us; }

    void updateError() override {}
    void resetRemote() override { remoteResets++; }

    int8_t getRemoteData(Data &data, int32_t id,
                         SDOAbortCodes &abortCode) override {
        abortCode = SDOAbortCode_OK;
        if (remotePendingTurns > 0) {
            remotePendingTurns--;
            return 1;
        }
        data = remote[id];
        return 0;
    }

    int8_t setRemoteData(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode) override {
        abortCode = SDOAbortCode_OK;
        if (remotePendingTurns > 0) {
            remotePendingTurns--;
            return 1;
        }
        remote[id] = data;
        return 0;
    }

    void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) override {
        remoteRpdo[pdoIndex].assign(odIDs, odIDs + OD_PDO_MAPPING_MAX);
    }
    void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDs[]) override {
        remoteTpdo[pdoIndex].assign(odIDs, odIDs + OD_PDO_MAPPING_MAX);
    }
    void enablePDO() override { pdoEnabled++; }
    void disablePDO() override { pdoDisabled++; }
    void getRemoteTPDO(uint16_t pdoIndex, Data data[]) override {
        const std::vector<int32_t> &ids = remoteTpdo[pdoIndex];
        for (size_t i = 0; i < ids.size(); i++) {
            if (ids[i] >= 0) data[i] = remote[ids[i]];
        }
    }
    void setRemoteRPDO(uint16_t pdoIndex, Data data[]) override {
        const std::vector<int32_t> &ids = remoteRpdo[pdoIndex];
        for (size_t i = 0; i < ids.size(); i++) {
            if (ids[i] >= 0) remote[ids[i]] = data[i];
        }
    }

    bool saveGroup(uint8_t parameterGroup) override {
        savedGroups.push_back(parameterGroup);
        return true;
    }
    bool loadGroup(uint8_t parameterGroup) override {
        loadedGroups.push_back(parameterGroup);
        return true;
    }
    bool saveSignature(uint64_t value) override {
        signature = value;
        signatureStored = true;
        return true;
    }
    bool loadSignature(uint64_t &value) override {
        value = signature;
        return signatureStored;
    }
    void advance(uint32_t delta_us) { now_us += delta_us; }
    void clearSent() { sent.clear(); }
};

}  // namespace CANopen
