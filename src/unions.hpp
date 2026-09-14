/**
 * Contains all of the shared unions used in this project.
 */
#pragma once
#include <stdint.h>

#include <string>

namespace CANopen {

/**
 * Metadata bitfield of an ObjectEntry.
 * The metadata specifies the read and write permissions, PDO mappability and
 * remote data update flag.
 */
union Access {
    uint8_t value;
    struct {
        bool readable : 1;
        bool writeable : 1;
        bool mappable : 1;
        bool limited : 1;
        bool remote : 1;
    } bits;
};

union Data {
    bool b;
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
    std::string* str;
    uint16_t* domain;
};
}  // namespace CANopen
