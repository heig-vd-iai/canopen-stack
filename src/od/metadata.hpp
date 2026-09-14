#pragma once

#include <stdint.h>
#include <string.h>

#include "od/dataType.hpp"
#include "unions.hpp"

namespace CANopen {

/**
 * Static description of one dictionary entry, built at compile time by the
 * generated dictionary and stored in read-only memory.
 * defaultValue, low and high point to constants of the entry's C type. A
 * VISIBLE_STRING points to its default text and size is its capacity. A
 * DOMAIN has no default and a size of 0.
 */
struct Metadata {
    Access access;
    DataType dataType;
    uint16_t size;
    const void *defaultValue;
    const void *low;
    const void *high;

    bool isScalar() const {
        return dataType != DataType::VISIBLE_STRING &&
               dataType != DataType::DOMAIN;
    }

    Data getDefaultValue() const {
        Data data;
        data.u64 = 0;
        if (isScalar()) memcpy(&data, defaultValue, size);
        return data;
    }
};

template <typename T>
struct DataTypeOf;

template <>
struct DataTypeOf<bool> {
    static constexpr DataType value = DataType::BOOLEAN;
};
template <>
struct DataTypeOf<int8_t> {
    static constexpr DataType value = DataType::INTEGER8;
};
template <>
struct DataTypeOf<int16_t> {
    static constexpr DataType value = DataType::INTEGER16;
};
template <>
struct DataTypeOf<int32_t> {
    static constexpr DataType value = DataType::INTEGER32;
};
template <>
struct DataTypeOf<int64_t> {
    static constexpr DataType value = DataType::INTEGER64;
};
template <>
struct DataTypeOf<uint8_t> {
    static constexpr DataType value = DataType::UNSIGNED8;
};
template <>
struct DataTypeOf<uint16_t> {
    static constexpr DataType value = DataType::UNSIGNED16;
};
template <>
struct DataTypeOf<uint32_t> {
    static constexpr DataType value = DataType::UNSIGNED32;
};
template <>
struct DataTypeOf<uint64_t> {
    static constexpr DataType value = DataType::UNSIGNED64;
};
template <>
struct DataTypeOf<float> {
    static constexpr DataType value = DataType::REAL32;
};
template <>
struct DataTypeOf<double> {
    static constexpr DataType value = DataType::REAL64;
};

template <typename T>
constexpr Metadata makeMetadata(uint8_t access, const T &defaultValue) {
    return Metadata{Access{access}, DataTypeOf<T>::value,
                    sizeof(T),      &defaultValue,
                    nullptr,        nullptr};
}

template <typename T>
constexpr Metadata makeMetadata(uint8_t access, const T &defaultValue,
                                const T &low, const T &high) {
    return Metadata{Access{access}, DataTypeOf<T>::value,
                    sizeof(T),      &defaultValue,
                    &low,           &high};
}

constexpr Metadata makeStringMetadata(uint8_t access, const char *defaultValue,
                                      uint16_t capacity) {
    return Metadata{Access{access}, DataType::VISIBLE_STRING,
                    capacity,       defaultValue,
                    nullptr,        nullptr};
}

constexpr Metadata makeDomainMetadata(uint8_t access) {
    return Metadata{Access{access}, DataType::DOMAIN, 0,
                    nullptr,        nullptr,          nullptr};
}

}  // namespace CANopen
