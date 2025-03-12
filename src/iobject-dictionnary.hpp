#pragma once

#include <cstdint>
#include <functional>
#include <utility>

#include "enums.hpp"
#include "unions.hpp"

namespace CANopen {

#pragma pack(push, 1)
struct Metadata {
    Access access;
    DataType dataType;
    virtual Data getDefaultValue(){
        Data data;
        return data;
    }
};

struct Metadata_bool : public Metadata {
    bool defaultValue;
    Metadata_bool(uint8_t access, bool defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = (DataType)0x01;
    }
    Data getDefaultValue() override {
        Data data;
        data.b = defaultValue;
        return data;
    }
};

struct Metadata_uint8_t : public Metadata {
    uint8_t defaultValue;
    Metadata_uint8_t() {}
    Metadata_uint8_t(uint8_t access, uint8_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED8;
    }
    Data getDefaultValue() override {
        Data data;
        data.u8 = defaultValue;
        return data;
    }
};

struct Metadata_uint8_t_limited : public Metadata_uint8_t {
    uint8_t min;
    uint8_t max;
    Metadata_uint8_t_limited(uint8_t access, uint8_t defaultValue,
                             uint8_t min, uint8_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED8;
    }
    Data getDefaultValue() override {
        Data data;
        data.u8 = defaultValue;
        return data;
    }
};

struct Metadata_uint16_t : public Metadata {
    uint16_t defaultValue;
    Metadata_uint16_t() {}
    Metadata_uint16_t(uint8_t access, uint16_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED16;
    }
    Data getDefaultValue() override {
        Data data;
        data.u16 = defaultValue;
        return data;
    }
};

struct Metadata_uint16_t_limited : public Metadata_uint16_t {
    uint16_t min;
    uint16_t max;
    Metadata_uint16_t_limited(uint8_t access, uint16_t defaultValue,
                              uint16_t min, uint16_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED16;
    }
    Data getDefaultValue() override {
        Data data;
        data.u16 = defaultValue;
        return data;
    }
};

struct Metadata_uint32_t : public Metadata {
    uint32_t defaultValue;
    Metadata_uint32_t() {}
    Metadata_uint32_t(uint8_t access, uint32_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED32;
    }
    Data getDefaultValue() override {
        Data data;
        data.u32 = defaultValue;
        return data;
    }
};

struct Metadata_uint32_t_limited : public Metadata_uint32_t {
    uint32_t min;
    uint32_t max;
    Metadata_uint32_t_limited(uint8_t access, uint32_t defaultValue,
                              uint32_t min, uint32_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED32;
    }
    Data getDefaultValue() override {
        Data data;
        data.u32 = defaultValue;
        return data;
    }
};

struct Metadata_uint64_t : public Metadata {
   uint64_t defaultValue;
   Metadata_uint64_t() {}
   Metadata_uint64_t(uint8_t access, uint64_t defaultValue) {
       this->defaultValue = defaultValue;
       this->access.value = access;
       this->dataType = DataType::UNSIGNED64;
   }
   Data getDefaultValue() override {
       Data data;
       data.u64 = defaultValue;
       return data;
   }
};

struct Metadata_uint64_t_limited : public Metadata_uint64_t {
   uint64_t min;
   uint64_t max;
   Metadata_uint64_t_limited(uint8_t access, uint64_t defaultValue,
                             uint64_t min, uint64_t max) {
       this->defaultValue = defaultValue;
       this->min = min;
       this->max = max;
       this->access.value = access;
       this->dataType = DataType::UNSIGNED64;
   }
   Data getDefaultValue() override {
       Data data;
       data.u64 = defaultValue;
       return data;
   }
};

struct Metadata_int8_t : public Metadata {
    int8_t defaultValue;
    Metadata_int8_t() {}
    Metadata_int8_t(uint8_t access, int8_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::INTEGER8;
    }
    Data getDefaultValue() override {
        Data data;
        data.i8 = defaultValue;
        return data;
    }
};

struct Metadata_int8_t_limited : public Metadata_int8_t {
    int8_t min;
    int8_t max;
    Metadata_int8_t_limited(uint8_t access, int8_t defaultValue, int8_t min,
                            int8_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER8;
    }
    Data getDefaultValue() override {
        Data data;
        data.i8 = defaultValue;
        return data;
    }
};

struct Metadata_int16_t : public Metadata {
    int16_t defaultValue;
    Metadata_int16_t() {}
    Metadata_int16_t(uint8_t access, int16_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::INTEGER16;
    }
    Data getDefaultValue() override {
        Data data;
        data.i16 = defaultValue;
        return data;
    }
};

struct Metadata_int16_t_limited : public Metadata_int16_t {
    int16_t min;
    int16_t max;
    Metadata_int16_t_limited(uint8_t access, int16_t defaultValue,
                             int16_t min, int16_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER16;
    }
    Data getDefaultValue() override {
        Data data;
        data.i16 = defaultValue;
        return data;
    }
};

struct Metadata_int32_t : public Metadata {
    int32_t defaultValue;
    Metadata_int32_t() {}
    Metadata_int32_t(uint8_t access, int32_t defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::INTEGER32;
    }
    Data getDefaultValue() override {
        Data data;
        data.i32 = defaultValue;
        return data;
    }
};

struct Metadata_int32_t_limited : public Metadata_int32_t {
    int32_t min;
    int32_t max;
    Metadata_int32_t_limited(uint8_t access, int32_t defaultValue,
                             int32_t min, int32_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER32;
    }
    Data getDefaultValue() override {
        Data data;
        data.i32 = defaultValue;
        return data;
    }
};

struct Metadata_int64_t : public Metadata {
   int64_t defaultValue;
   Metadata_int64_t() {}
   Metadata_int64_t(uint8_t access, int64_t defaultValue) {
       this->defaultValue = defaultValue;
       this->access.value = access;
       this->dataType = DataType::INTEGER64;
   }
   Data getDefaultValue() override {
       Data data;
       data.i64 = defaultValue;
       return data;
   }
};

struct Metadata_int64_t_limited : public Metadata_int64_t {
   int64_t min;
   int64_t max;
   Metadata_int64_t_limited(uint8_t access, int64_t defaultValue,
                            int64_t min, int64_t max) {
       this->defaultValue = defaultValue;
       this->min = min;
       this->max = max;
       this->access.value = access;
       this->dataType = DataType::INTEGER64;
   }
   Data getDefaultValue() override {
       Data data;
       data.i64 = defaultValue;
       return data;
   }
};

struct Metadata_float : public Metadata {
    float defaultValue;
    Metadata_float() {}
    Metadata_float(uint8_t access, float defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = (DataType)0x08;
    }
    Data getDefaultValue() override {
        Data data;
        data.f32 = defaultValue;
        return data;
    }
};

struct Metadata_float_limited : public Metadata_float {
    float min;
    float max;
    Metadata_float_limited(uint8_t access, float defaultValue, float min,
                           float max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = (DataType)0x08;
    }
    Data getDefaultValue() override {
        Data data;
        data.f32 = defaultValue;
        return data;
    }
};

struct Metadata_double : public Metadata {
   double defaultValue;
   Metadata_double() {}
   Metadata_double(uint8_t access, double defaultValue) {
       this->defaultValue = defaultValue;
       this->access.value = access;
       this->dataType = (DataType)0x11;
   }
   Data getDefaultValue() override {
       Data data;
       data.f64 = defaultValue;
       return data;
   }
};

struct Metadata_double_limited : public Metadata_double {
   double min;
   double max;
   Metadata_double_limited(uint8_t access, double defaultValue, double min,
                           double max) {
       this->defaultValue = defaultValue;
       this->min = min;
       this->max = max;
       this->access.value = access;
       this->dataType = (DataType)0x11;
   }
   Data getDefaultValue() override {
       Data data;
       data.f64 = defaultValue;
       return data;
   }
};

struct Metadata_string : public Metadata {
    std::string defaultValue;
    uint16_t length;
    Metadata_string(uint8_t access, std::string defaultValue, uint16_t length) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::VISIBLE_STRING;
    }
};

struct Metadata_domain : public Metadata {
    Metadata_domain(uint8_t access) {
        this->access.value = access;
        this->dataType = DataType::DOMAIN;
    }
};
#pragma pack(pop)

class IObjectDictionnary {
   public:
    virtual int8_t readData(Data &data, uint16_t index, uint8_t subindex,
                            SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(const Data &data, uint16_t index, uint8_t subindex,
                             SDOAbortCodes &abortCode) = 0;
    virtual int8_t readData(Data &data, int32_t id,
                            SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(const Data &data, int32_t id,
                             SDOAbortCodes &abortCode) = 0;
    virtual int8_t readData(Data &data, uint16_t index, uint8_t subindex) = 0;
    virtual int8_t writeData(const Data &data, uint16_t index,
                             uint8_t subindex) = 0;
    virtual int8_t readData(Data &data, int32_t id) = 0;
    virtual int8_t writeData(const Data &data, int32_t id) = 0;
    virtual bool saveData(uint8_t parameterGroup) = 0;
    virtual bool loadData(uint8_t parameterGroup) = 0;
    virtual bool restoreData(uint8_t parameterGroup) = 0;
    virtual bool isSubValid(uint16_t index, uint8_t subindex) = 0;
    virtual int32_t findObject(uint16_t index) = 0;
    virtual int32_t findObject(uint16_t index, uint8_t subindex) = 0;
    virtual Metadata* getMetadata(uint16_t index, uint8_t subindex) = 0;
    virtual Metadata* getMetadata(int32_t id) = 0;
    virtual Data* getData(uint16_t index, uint8_t subindex) = 0;
    virtual Data* getData(int32_t id) = 0;
    virtual void setData(const Data &data, uint16_t index,
                         uint8_t subindex) = 0;
    virtual void setData(const Data &data, int32_t id) = 0;
    virtual uint16_t getSize(uint16_t index, uint8_t subindex) = 0;
    virtual uint16_t getSize(int32_t id) = 0;
};
} // namespace CANopen
