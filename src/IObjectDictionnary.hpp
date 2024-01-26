#pragma once

#include <cstdint>
#include <functional>
#include <utility>

#include "enums.hpp"
#include "unions.hpp"

enum DataType {
    INTEGER8,
    INTEGER16,
    INTEGER32,
    INTEGER64,
    UNSIGNED8,
    UNSIGNED16,
    UNSIGNED32,
    UNSIGNED64,
    REAL32,
    REAL64,
    VISIBLE_STRING,
    ARRAY
};

struct Data {
    Data(int64_t value, DataType type, uint8_t size) : size(size), type(type) {
        this->value.i64 = value;
    }
    Data(DataType type, uint8_t size) : size(size), type(type) {}
    union {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        float f32;
        double f64;
    } value;
    uint8_t size;
    DataType type;
};
namespace CANopen {

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
    virtual int8_t writeData(Data data, uint16_t index, uint8_t subindex,
                             SDOAbortCodes &abortCode) = 0;
    virtual bool saveData(uint8_t parameterGroup) = 0;
    virtual bool loadData(uint8_t parameterGroup) = 0;
    virtual bool restoreData(uint8_t parameterGroup) = 0;
    virtual bool isSubValid(uint16_t index, uint8_t subindex) = 0;
    virtual int64_t findObject(uint16_t index) = 0;
    virtual int64_t findObject(uint16_t index, uint8_t subindex) = 0;
    virtual MetaBitfield getMetadata(uint16_t index, uint8_t subindex) = 0;
};
}  // namespace CANopen
