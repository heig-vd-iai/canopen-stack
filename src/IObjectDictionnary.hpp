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
};

struct Metadatabool : public Metadata {
    bool defaultValue;
    Metadatabool(bool defaultValue, AccessType access) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::BOOL;
    }
};

struct Metadata_uint8_t : public Metadata {
    uint8_t defaultValue;
    uint8_t min;
    uint8_t max;
    Metadata_uint8_t(AccessType access, uint8_t defaultValue, uint8_t min,
                    uint8_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED8;
    }
};

struct Metadata_uint16_t : public Metadata {
    uint16_t defaultValue;
    uint16_t min;
    uint16_t max;
    Metadata_uint16_t(AccessType access, uint16_t defaultValue, uint16_t min,
                     uint16_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED16;
    }
};

struct Metadata_uint32_t : public Metadata {
    uint32_t defaultValue;
    uint32_t min;
    uint32_t max;
    Metadata_uint32_t(AccessType access, uint32_t defaultValue, uint32_t min,
                     uint32_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED32;
    }
};

struct Metadata_uint64_t : public Metadata {
    uint64_t defaultValue;
    uint64_t min;
    uint64_t max;
    Metadata_uint64_t(AccessType access, uint64_t defaultValue, uint64_t min,
                     uint64_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::UNSIGNED64;
    }
};

struct Metadata_int8_t : public Metadata {
    int8_t defaultValue;
    int8_t min;
    int8_t max;
    Metadata_int8_t(AccessType access, int8_t defaultValue, int8_t min,
                   int8_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER8;
    }
};

struct Metadata_int16_t : public Metadata {
    int16_t defaultValue;
    int16_t min;
    int16_t max;
    Metadata_int16_t(AccessType access, int16_t defaultValue, int16_t min,
                    int16_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER16;
    }
};

struct Metadata_int32_t : public Metadata {
    int32_t defaultValue;
    int32_t min;
    int32_t max;
    Metadata_int32_t(AccessType access, int32_t defaultValue, int32_t min,
                    int32_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER32;
    }
};

struct Metadata_int64_t : public Metadata {
    int64_t defaultValue;
    int64_t min;
    int64_t max;
    Metadata_int64_t(AccessType access, int64_t defaultValue, int64_t min,
                    int64_t max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::INTEGER64;
    }
};

struct Metadata_float : public Metadata {
    float defaultValue;
    float min;
    float max;
    Metadata_float(AccessType access, float defaultValue, float min, float max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::REAL32;
    }
};

struct Metadata_double : public Metadata {
    double defaultValue;
    double min;
    double max;
    Metadata_double(AccessType access, double defaultValue, double min,
                   double max) {
        this->defaultValue = defaultValue;
        this->min = min;
        this->max = max;
        this->access.value = access;
        this->dataType = DataType::REAL64;
    }
};

struct Metadata_char : public Metadata {
    char *defaultValue;
    Metadata_char(AccessType access, char *defaultValue) {
        this->defaultValue = defaultValue;
        this->access.value = access;
        this->dataType = DataType::VISIBLE_STRING;
    }
};
#pragma pack(pop)

class IObjectDictionnary {
   public:
    virtual int8_t readData(Data &data, uint16_t index, uint8_t subindex,
                            SDOAbortCodes &abortCode) = 0;
    virtual int8_t writeData(const Data &data, uint16_t index, uint8_t subindex,
                             SDOAbortCodes &abortCode) = 0;
    virtual bool saveData(uint8_t parameterGroup) = 0;
    virtual bool loadData(uint8_t parameterGroup) = 0;
    virtual bool restoreData(uint8_t parameterGroup) = 0;
    virtual bool isSubValid(uint16_t index, uint8_t subindex) = 0;
    virtual int64_t findObject(uint16_t index) = 0;
    virtual int64_t findObject(uint16_t index, uint8_t subindex) = 0;
    virtual Metadata getMetadata(uint16_t index, uint8_t subindex) = 0;
    virtual Data *getData(uint16_t index, uint8_t subindex) = 0;
    virtual Data *getData(uint32_t id) = 0;
    virtual void setData(const Data &data, uint16_t index,
                         uint8_t subindex) = 0;
    virtual void setData(const Data &data, uint32_t id) = 0;
    virtual uint16_t getSize(uint16_t index, uint8_t subindex) = 0;
    virtual uint16_t getSize(uint32_t id) = 0;
};
}  // namespace CANopen
