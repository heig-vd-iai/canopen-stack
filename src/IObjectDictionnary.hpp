#pragma once

#include <cstdint>
#include <functional>
#include <utility>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {

struct Metadata {
    MetaBitfield bitfield;
    DataType dataType;
    void *defaultValue;
    void *min;
    void *max;
};

class IObjectDictionnary {
   public:
    virtual int8_t readData(Data &data, uint16_t index, uint8_t subindex,
                            SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(const Data &data, uint16_t index, uint8_t subindex,
                             SDOAbortCodes &abortCode) = 0;
    virtual int8_t readData(Data &data, uint32_t id, SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(const Data &data, uint32_t id,
                             SDOAbortCodes &abortCode) = 0;
    virtual bool saveData(uint8_t parameterGroup) = 0;
    virtual bool loadData(uint8_t parameterGroup) = 0;
    virtual bool restoreData(uint8_t parameterGroup) = 0;
    virtual bool isSubValid(uint16_t index, uint8_t subindex) = 0;
    virtual int64_t findObject(uint16_t index) = 0;
    virtual int64_t findObject(uint16_t index, uint8_t subindex) = 0;
    virtual MetaBitfield getMetadata(uint16_t index, uint8_t subindex) = 0;
    virtual void getData(Data &data, uint16_t index, uint8_t subindex) = 0;
    virtual void getData(Data &data, uint16_t id) = 0;
    virtual void setData(const Data &data, uint16_t index,
                         uint8_t subindex) = 0;
    virtual void setData(const Data &data, uint16_t id) = 0;
};
}  // namespace CANopen
