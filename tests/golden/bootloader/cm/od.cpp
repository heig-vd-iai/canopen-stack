/**
 * Contains the declarations of the ObjectDictionnary class.
 * This is an auto-generated file, do not edit it directly.
 */
#include "od.hpp"

using namespace CANopen;

const uint64_t od_signature = 202609141328u;

/**
 * Default values and limits of each object, then their metadata.
 */
namespace {
constexpr uint32_t object1000sub0Default = 0x00000000;
constexpr uint8_t object1001sub0Default = 0;
constexpr uint16_t object1017sub0Default = 1000;
constexpr uint8_t object1018sub0Default = 4;
constexpr uint32_t object1018sub1Default = 0;
constexpr uint32_t object1018sub2Default = 2;
constexpr uint32_t object1018sub3Default = 1;
constexpr uint32_t object1018sub4Default = 0;
constexpr uint8_t object1f50sub0Default = 1;
constexpr uint8_t object1f51sub0Default = 1;
constexpr uint8_t object1f51sub1Default = 0;
constexpr uint8_t object1f56sub0Default = 1;
constexpr uint32_t object1f56sub1Default = 0;
constexpr uint8_t object1f57sub0Default = 1;
constexpr uint32_t object1f57sub1Default = 0;
}  // namespace

const Metadata ObjectDictionnary::objectMetadataTable[16] = {
    /* 0000 */ makeMetadata(0b00001, object1000sub0Default),
    /* 0001 */ makeMetadata(0b00101, object1001sub0Default),
    /* 0002 */ makeMetadata(0b00011, object1017sub0Default),
    /* 0003 */ makeMetadata(0b00001, object1018sub0Default),
    /* 0004 */ makeMetadata(0b00001, object1018sub1Default),
    /* 0005 */ makeMetadata(0b00001, object1018sub2Default),
    /* 0006 */ makeMetadata(0b00001, object1018sub3Default),
    /* 0007 */ makeMetadata(0b00001, object1018sub4Default),
    /* 0008 */ makeMetadata(0b00001, object1f50sub0Default),
    /* 0009 */ makeDomainMetadata(0b00010),
    /* 0010 */ makeMetadata(0b00001, object1f51sub0Default),
    /* 0011 */ makeMetadata(0b00011, object1f51sub1Default),
    /* 0012 */ makeMetadata(0b00001, object1f56sub0Default),
    /* 0013 */ makeMetadata(0b00001, object1f56sub1Default),
    /* 0014 */ makeMetadata(0b00001, object1f57sub0Default),
    /* 0015 */ makeMetadata(0b00001, object1f57sub1Default),
};

/**
 * Getters/Setters for local data.
 */
static int8_t getobject1017sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return hbGetData(data, id, abortCode);
}
static int8_t setobject1017sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return hbSetData(data, id, abortCode);
}

int8_t (*ObjectDictionnary::objectGetterTable[16])(Data &data, int32_t id, SDOAbortCodes &abortCode) = {
    /* 0000 - 0x1000 */    &getLocalData_uint32_t,
    /* 0001 - 0x1001 */    &getLocalData_uint8_t,
    /* 0002 - 0x1017 */    &getobject1017sub0,
    /* 0003 - 0x1018 */    &getLocalData_uint8_t,
    /* 0004 - 0x1018 */    &getLocalData_uint32_t,
    /* 0005 - 0x1018 */    &getLocalData_uint32_t,
    /* 0006 - 0x1018 */    &getLocalData_uint32_t,
    /* 0007 - 0x1018 */    &getLocalData_uint32_t,
    /* 0008 - 0x1f50 */    &getLocalData_uint8_t,
    /* 0009 - 0x1f50 */    &getLocalData_domain,
    /* 0010 - 0x1f51 */    &getLocalData_uint8_t,
    /* 0011 - 0x1f51 */    &getLocalData_uint8_t,
    /* 0012 - 0x1f56 */    &getLocalData_uint8_t,
    /* 0013 - 0x1f56 */    &getLocalData_uint32_t,
    /* 0014 - 0x1f57 */    &getLocalData_uint8_t,
    /* 0015 - 0x1f57 */    &getLocalData_uint32_t,
};

// This table contain all the objects setter
int8_t (*ObjectDictionnary::objectSetterTable[16])(const Data &data, int32_t id, SDOAbortCodes &abortCode) = {
    /* 0000 */ &setLocalData_uint32_t,
    /* 0001 */ &setLocalData_uint8_t,
    /* 0002 */ &setobject1017sub0,
    /* 0003 */ &setLocalData_uint8_t,
    /* 0004 */ &setLocalData_uint32_t,
    /* 0005 */ &setLocalData_uint32_t,
    /* 0006 */ &setLocalData_uint32_t,
    /* 0007 */ &setLocalData_uint32_t,
    /* 0008 */ &setLocalData_uint8_t,
    /* 0009 */ &setLocalData_domain,
    /* 0010 */ &setLocalData_uint8_t,
    /* 0011 */ &setLocalData_uint8_t,
    /* 0012 */ &setLocalData_uint8_t,
    /* 0013 */ &setLocalData_uint32_t,
    /* 0014 */ &setLocalData_uint8_t,
    /* 0015 */ &setLocalData_uint32_t,
};
// END GETTER/SETTER


// Default values for u8
uint8_t ObjectDictionnary::u8Table[7] = {
    /* 0000 - 001 */ 0,
    /* 0001 - 003 */ 4,
    /* 0002 - 008 */ 1,
    /* 0003 - 010 */ 1,
    /* 0004 - 011 */ 0,
    /* 0005 - 012 */ 1,
    /* 0006 - 014 */ 1,
};
// Default values for u16
uint16_t ObjectDictionnary::u16Table[1] = {
    /* 0000 - 002 */ 1000,
};
// Default values for u32
uint32_t ObjectDictionnary::u32Table[7] = {
    /* 0000 - 000 */ 0x00000000,
    /* 0001 - 004 */ 0,
    /* 0002 - 005 */ 2,
    /* 0003 - 006 */ 1,
    /* 0004 - 007 */ 0,
    /* 0005 - 013 */ 0,
    /* 0006 - 015 */ 0,
};

// string type data variable


// index data table

uint8_t ObjectDictionnary::dataIndexTable[16] = {
    0,
    0,
    0,
    1,
    1,
    2,
    3,
    4,
    2,
    0,
    3,
    4,
    5,
    5,
    6,
    6,
};
//END DATA
