/******************************************************************************
 * [Filename]:      unions.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains all of the shared unions used in this project.
 *****************************************************************************/
#pragma once
#include <cstdint>

namespace CANopen
{
    /**
     * @brief Union representing the access type of an ObjectEntry.
     * The access type specifies the read and write permissions, as well as the PDO mappability.
     */
    union AccessType
    {
        uint8_t value;
        struct
        {
            uint8_t readable : 1;
            uint8_t writeable : 1;
            uint8_t mappable : 1;
        } bits;
    };

    /**
     * @brief Union representing the COB-ID entry in PDO communication parameter (0x1400, 0x1800)
     */
    union PDOCobidEntry
    {
        uint32_t value;
        struct
        {
            uint32_t canId : 29;
            uint32_t frame : 1;
            uint32_t rtr : 1;
            uint32_t valid : 1;
        } bits;
    };

    /**
     * @brief Union representing the mapping value in PDO mapping parameter (0x1600, 0x1A00).
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
     * @brief Union representing the error register value in error register (0x1001).
     */
    union ErrorRegister
    {
        uint8_t value;
        struct
        {
            uint8_t genericError : 1;
            uint8_t current : 1;
            uint8_t voltage : 1;
            uint8_t temperature : 1;
            uint8_t communicationError : 1;
            uint8_t deviceProfileSpecific : 1;
            uint8_t reserved : 1;
            uint8_t manufacturerSpecific : 1;
        } bits;
    };

    /**
     * @brief Union representing the SDO command byte (byte 0) in SDO transactions.
     * Multiple structures are used depending on the operation.
     */
    union SDOCommandByte
    {
        uint8_t value;
        struct
        {
            uint8_t s : 1;
            uint8_t e : 1;
            uint8_t n : 2;
            uint8_t reserved : 1;
            uint8_t ccs : 3;
        } bits_initiate;
        struct
        {
            uint8_t c : 1;
            uint8_t n : 3;
            uint8_t t : 1;
            uint8_t ccs : 3;
        } bits_segment;
    };

    /**
     * @brief Union representing the SDO command byte (byte 0) in SDO block transactions.
     * Multiple structures are used depending on the operation.
     */
    union SDOBlockCommandByte
    {
        uint8_t value;
        struct
        {
            uint8_t cs : 1;
            uint8_t s : 1;
            uint8_t cc : 1;
            uint8_t reserved : 2;
            uint8_t ccs : 3;
        } bits_downClientInitiate;
        struct
        {
            uint8_t seqno : 7;
            uint8_t c : 1;
        } bits_downClientSub;
        struct
        {
            uint8_t cs : 1;
            uint8_t reserved : 1;
            uint8_t n : 3;
            uint8_t ccs : 3;
        } bits_downClientEnd;
        struct
        {
            uint8_t ss : 2;
            uint8_t sc : 1;
            uint8_t reserved : 2;
            uint8_t scs : 3;
        } bits_downServer;
        struct
        {
            uint8_t cs : 2;
            uint8_t cc : 1;
            uint8_t reserved : 2;
            uint8_t ccs : 3;
        } bits_upClient;
        struct
        {
            uint8_t ss : 1;
            uint8_t s : 1;
            uint8_t sc : 1;
            uint8_t reserved : 2;
            uint8_t scs : 3;
        } bits_upServerInitiate;
        struct
        {
            uint8_t seqno : 7;
            uint8_t c : 1;
        } bits_upServerSub;
        struct
        {
            uint8_t ss : 2;
            uint8_t n : 3;
            uint8_t scs : 3;
        } bits_upServerEnd;
    };
}
