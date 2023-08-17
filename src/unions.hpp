#pragma once
#include <cstdint>

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
