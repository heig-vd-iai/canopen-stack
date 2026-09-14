#pragma once

#include <cstdint>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {

/**
 * Objects served by another core or device.
 * Data accessors return 0 when done, 1 while the request is pending and -1
 * on failure with `abortCode` set.
 */
class RemoteObjects {
   public:
    virtual ~RemoteObjects() = default;
    virtual void init() = 0;
    /** Poll the remote side for errors; called from Node::update(). */
    virtual void updateError() = 0;
    /** Reset the remote application; called on NMT reset node. */
    virtual void resetRemote() = 0;
    virtual int8_t getRemoteData(Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual int8_t setRemoteData(const Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void enablePDO() = 0;
    virtual void disablePDO() = 0;
    virtual void getRemoteTPDO(uint16_t pdoIndex, Data data[]) = 0;
    virtual void setRemoteRPDO(uint16_t pdoIndex, Data data[]) = 0;
};

/**
 * Single-core implementation: every remote access fails with a hardware
 * error and PDO hooks do nothing.
 */
struct NullRemote : public RemoteObjects {
    void init() override {}
    void updateError() override {}
    void resetRemote() override {}
    int8_t getRemoteData(Data &, int32_t, SDOAbortCodes &abortCode) override {
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
    int8_t setRemoteData(const Data &, int32_t,
                         SDOAbortCodes &abortCode) override {
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
    void configRemoteRPDO(uint16_t, int32_t[]) override {}
    void configRemoteTPDO(uint16_t, int32_t[]) override {}
    void enablePDO() override {}
    void disablePDO() override {}
    void getRemoteTPDO(uint16_t, Data[]) override {}
    void setRemoteRPDO(uint16_t, Data[]) override {}
};
}  // namespace CANopen
