#pragma once
#include <cstdint>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {
class Frame;

class HardwareInterface {
   public:
    virtual void init() = 0;
    virtual void update() = 0;
     /**
     * Send a CANopen frame to the CAN network.
     * @param frame The CANopen frame to send.
     */
    virtual void sendFrame(const Frame &frame) = 0;
    /**
     * Get the relative clock time.
     * This value is used internally and doesn't have to be absolute.
     * The counter MUST count up to 0xFFFFFFFF in order to avoid clocking
     * issues.
     * @return Current clock time in microseconds.
     */
    virtual uint32_t getTime_us() = 0;
    virtual void updateError() = 0;
    virtual void resetRemote() = 0;
    virtual int8_t getRemoteData(Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual int8_t setRemoteData(const Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDS[]) = 0;
    virtual void enablePDO() = 0;
    virtual void disablePDO() = 0;
    virtual void getRemoteTPDO(uint16_t pdoIndex, Data data[]) = 0;
    virtual void setRemoteRPDO(uint16_t pdoIndex, Data data[]) = 0;
    virtual bool saveDataToFlash(uint8_t parameterGroup) = 0;
    virtual bool loadDataFromFlash(uint8_t parameterGroup) = 0;
    virtual bool saveSignatureToFlash(uint64_t signature) = 0;
    virtual bool loadSignatureFromFlash(uint64_t &signature) = 0;
    virtual bool reboot() = 0;
};
}  // namespace CANopen