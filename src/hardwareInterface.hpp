#pragma once
#include <cstdint>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {
class Frame;

class HardwareInterface {
   public:
    virtual void configCan() = 0;
    virtual void update() = 0;
    virtual void sendFrame(const Frame &frame) = 0;
    virtual uint32_t getTime_us() = 0;
    virtual void updateError() = 0;
    virtual int8_t getRemoteData(Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual int8_t setRemoteData(const Data &data, int32_t id,
                                 SDOAbortCodes &abortCode) = 0;
    virtual void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) = 0;
    virtual void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDS[]) = 0;
    virtual void getRemoteTPDO(uint16_t pdoIndex, Data data[]) = 0;
    virtual void setRemoteRPDO(uint16_t pdoIndex, Data data[]) = 0;
};
}  // namespace CANopen