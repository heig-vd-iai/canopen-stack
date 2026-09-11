#pragma once
#include <cstdint>
#include <map>
#include <vector>

#include "frame.hpp"
#include "hardware-interface.hpp"
#include "node.hpp"

namespace CANopen {

class FakeHardware : public HardwareInterface {
   public:
    std::vector<Frame> sent;
    uint32_t now_us = 0;
    std::map<int32_t, Data> remote;
    unsigned remotePendingTurns = 0;
    uint64_t signature = 0;
    bool signatureStored = false;
    std::vector<uint8_t> savedGroups;
    std::vector<uint8_t> loadedGroups;

    void init() override {}
    void update() override {}
    void sendFrame(const Frame &frame) override { sent.push_back(frame); }
    uint32_t getTime_us() override { return now_us; }
    void updateError() override {}
    void resetRemote() override {}

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

    void configRemoteRPDO(uint16_t, int32_t[]) override {}
    void configRemoteTPDO(uint16_t, int32_t[]) override {}
    void enablePDO() override {}
    void disablePDO() override {}
    void getRemoteTPDO(uint16_t, Data[]) override {}
    void setRemoteRPDO(uint16_t, Data[]) override {}

    bool saveDataToFlash(uint8_t parameterGroup) override {
        savedGroups.push_back(parameterGroup);
        return true;
    }
    bool loadDataFromFlash(uint8_t parameterGroup) override {
        loadedGroups.push_back(parameterGroup);
        return true;
    }
    bool saveSignatureToFlash(uint64_t value) override {
        signature = value;
        signatureStored = true;
        return true;
    }
    bool loadSignatureFromFlash(uint64_t &value) override {
        value = signature;
        return signatureStored;
    }
    void advance(uint32_t delta_us) { now_us += delta_us; }
    void clearSent() { sent.clear(); }
};

inline FakeHardware &initNodeOnce() {
    static FakeHardware hardware;
    static bool initialised = false;
    if (!initialised) {
        initialised = true;
        node.init(&hardware);
    }
    return hardware;
}

}  // namespace CANopen
