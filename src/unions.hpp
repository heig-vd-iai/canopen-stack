/**
 * Contains all of the shared unions used in this project.
 */
#pragma once
#include <cstdint>

namespace CANopen
{
    /**
     * Access type of an ObjectEntry.
     * The access type specifies the read and write permissions, as well as the PDO mappability.
     */
    union AccessType
    {
        uint8_t value;
        struct
        {
            bool readable : 1;
            bool writeable : 1;
            bool mappable : 1;
        } bits;
    };

    /**
     * COB-ID entry in PDO communication parameter (0x1400, 0x1800)
     */
    union PDOCobidEntry
    {
        uint32_t value;
        struct
        {
            uint32_t canId : 29;
            bool frame : 1;
            bool rtr : 1;
            bool valid : 1;
        } bits;
    };

    /**
     * Union representing the mapping value in PDO mapping parameter (0x1600, 0x1A00).
     */
    union PDOMapEntry
    {
        uint32_t value;
        struct
        {
            uint32_t length : 8;
            uint32_t subindex : 8;
            uint32_t index : 16;
        } bits;
    };

    /**
     * Union representing the error register value in error register (0x1001).
     */
    union ErrorRegister
    {
        uint8_t value;
        struct
        {
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
    union SDOCommandByte
    {
        uint8_t value;
        // TODO: Better to have a union with short and long names: e/expedited, s/size, n/number, ccs/commandSpecifier
        struct
        {
            bool s : 1;
            bool e : 1;
            unsigned n : 2;
            unsigned reserved : 1;
            unsigned ccs : 3;
        } bits_initiate;
        struct
        {
            bool c : 1;
            unsigned n : 3;
            bool t : 1;
            unsigned ccs : 3;
        } bits_segment;
    };

    /**
     * Union representing the SDO command byte (byte 0) in SDO block transactions.
     * Multiple structures are used depending on the operation.
     */
    union SDOBlockCommandByte
    {
        uint8_t value;
        struct
        {
            bool cs : 1;
            bool s : 1;
            bool cc : 1;
            unsigned reserved : 2;
            unsigned ccs : 3;
        } bits_downClientInitiate;
        struct
        {
            unsigned seqno : 7;
            bool c : 1;
        } bits_downClientSub;
        struct
        {
            bool cs : 1;
            bool reserved : 1;
            unsigned n : 3;
            unsigned ccs : 3;
        } bits_downClientEnd;
        struct
        {
            unsigned ss : 2;
            bool sc : 1;
            unsigned reserved : 2;
            unsigned scs : 3;
        } bits_downServer;
        struct
        {
            unsigned cs : 2;
            bool cc : 1;
            unsigned reserved : 2;
            unsigned ccs : 3;
        } bits_upClient;
        struct
        {
            bool ss : 1;
            bool s : 1;
            bool sc : 1;
            unsigned reserved : 2;
            unsigned scs : 3;
        } bits_upServerInitiate;
        struct
        {
            unsigned seqno : 7;
            bool c : 1;
        } bits_upServerSub;
        struct
        {
            unsigned ss : 2;
            unsigned n : 3;
            unsigned scs : 3;
        } bits_upServerEnd;
    };
}
