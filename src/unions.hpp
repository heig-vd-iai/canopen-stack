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

/**
 * COB-ID entry in PDO communication parameter (0x1400, 0x1800)
 */
union PDOCobidEntry {
    uint32_t value;
    struct {
        uint32_t canId : 29;
        bool frame : 1;
        bool rtr : 1;
        bool valid : 1;
    } bits;
};

/**
 * Union representing the mapping value in PDO mapping parameter (0x1600,
 * 0x1A00).
 */
union PDOMapEntry {
    uint32_t value;
    struct {
        uint32_t length : 8;
        uint32_t subindex : 8;
        uint32_t index : 16;
    } bits;
};

/**
 * Union representing the error register value in error register (0x1001).
 */
union ErrorRegisterValue {
    uint8_t value;
    struct {
        bool genericError : 1;
        bool current : 1;
        bool voltage : 1;
        bool temperature : 1;
        bool communicationError : 1;
        bool deviceProfileSpecific : 1;
        bool reserved : 1;
        bool manufacturerSpecific : 1;
    } bits;
};

/**
 * Union representing the SDO command byte (byte 0) in SDO transactions.
 * Multiple structures are used depending on the operation.
 */
union SDOCommandByte {
    uint8_t value;
    struct {
        bool s : 1;      // size indicator
        bool e : 1;      // expedited transfer
        unsigned n : 2;  // n bytes that do not contain data
        unsigned reserved : 1;
        unsigned ccs : 3;  // client command specifier
    } bits_initiate;
    struct {
        bool c : 1;        // final segment
        unsigned n : 3;    // n bytes that do not contain data
        bool t : 1;        // toggle bit
        unsigned ccs : 3;  // client command specifier
    } bits_segment;
};

/**
 * Union representing the SDO command byte (byte 0) in SDO block transactions.
 * Multiple structures are used depending on the operation.
 */
union SDOBlockCommandByte {
    uint8_t value;
    struct {
        bool cs : 1;  // client subcommand
        bool s : 1;   // size indicator
        bool cc : 1;  // client CRC support
        unsigned reserved : 2;
        unsigned ccs : 3;  // client command specifier
    } bits_downClientInitiate;
    struct {
        unsigned seqno : 7;  // sequence number
        bool c : 1;          // final segment
    } bits_downClientSub;
    struct {
        bool cs : 1;  // client subcommand
        bool reserved : 1;
        unsigned n : 3;    // n bytes that do not contain data
        unsigned ccs : 3;  // client command specifier
    } bits_downClientEnd;
    struct {
        unsigned ss : 2;  // server subcommand
        bool sc : 1;      // server CRC support
        unsigned reserved : 2;
        unsigned scs : 3;  // server command specifier
    } bits_downServer;
    struct {
        unsigned cs : 2;  // client subcommand
        bool cc : 1;      // client CRC support
        unsigned reserved : 2;
        unsigned ccs : 3;  // client command specifier
    } bits_upClient;
    struct {
        bool ss : 1;  // server subcommand
        bool s : 1;   // size indicator
        bool sc : 1;  // server CRC support
        unsigned reserved : 2;
        unsigned scs : 3;  // server command specifier
    } bits_upServerInitiate;
    struct {
        unsigned seqno : 7;  // sequence number
        bool c : 1;          // final segment
    } bits_upServerSub;
    struct {
        unsigned ss : 2;   // server subcommand
        unsigned n : 3;    // n bytes that do not contain data
        unsigned scs : 3;  // server command specifier
    } bits_upServerEnd;
};

union Data{
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
};
}  // namespace CANopen
