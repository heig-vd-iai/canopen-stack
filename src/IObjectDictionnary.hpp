#pragma once

#include <cstdint>
#include <functional>
#include <utility>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {

struct MetaData{
    MetaBitfield bitfield;
    DataType dataType;
    void* defaultValue;
    void* min;
    void* max;
};

class IObjectDictionnary {
    //    protected:
    // virtual uint8_t getData(Data &data, uint16_t id,
    //                         SDOAbortCodes &abortCode) = 0;
    // virtual uint8_t setData(Data data, uint16_t id,
    //                         SDOAbortCodes &abortCode) = 0;

   public:
    Data *objectDataTable;
    // MetaBitfield *objectMetadataTable;
    friend class MapParameter;
    friend class CommParameter;
    virtual int8_t readData(Data &data, uint16_t index, uint8_t subindex,
                            SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(Data const &data, uint16_t index, uint8_t subindex,
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
    virtual void setData(Data const &data, uint16_t index, uint8_t subindex) = 0;
    virtual void setData(Data const &data, uint16_t id) = 0;
};
}  // namespace CANopen
