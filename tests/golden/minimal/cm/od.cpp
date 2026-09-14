/**
 * Contains the declarations of the ObjectDictionnary class.
 * This is an auto-generated file, do not edit it directly.
 */
#include "od.hpp"

using namespace CANopen;

const uint64_t od_signature = 202609141111u;

/**
 * Metadata declaration for each object.
 */
static Metadata_uint32_t object1000sub0Metadata{0b00001, 0x00000192};
static Metadata_uint8_t object1001sub0Metadata{0b00101, 0};
static Metadata_uint8_t object1010sub0Metadata{0b00001, 5};
static Metadata_uint32_t object1010sub1Metadata{0b00011, 0};
static Metadata_uint32_t object1010sub2Metadata{0b00011, 0};
static Metadata_uint32_t object1010sub3Metadata{0b00011, 0};
static Metadata_uint32_t object1010sub4Metadata{0b00011, 0};
static Metadata_uint32_t object1010sub5Metadata{0b00011, 0};
static Metadata_uint8_t object1011sub0Metadata{0b00001, 5};
static Metadata_uint32_t object1011sub1Metadata{0b00011, 0};
static Metadata_uint32_t object1011sub2Metadata{0b00011, 0};
static Metadata_uint32_t object1011sub3Metadata{0b00011, 0};
static Metadata_uint32_t object1011sub4Metadata{0b00011, 0};
static Metadata_uint32_t object1011sub5Metadata{0b00011, 0};
static Metadata_uint16_t object1017sub0Metadata{0b00011, 1000};
static Metadata_uint8_t object1018sub0Metadata{0b00001, 4};
static Metadata_uint32_t object1018sub1Metadata{0b00001, 0};
static Metadata_uint32_t object1018sub2Metadata{0b00001, 1};
static Metadata_uint32_t object1018sub3Metadata{0b00001, 1};
static Metadata_uint32_t object1018sub4Metadata{0b00001, 0};
static Metadata_uint8_t object1400sub0Metadata{0b00001, 2};
static Metadata_uint32_t object1400sub1Metadata{0b00011, 2147484161};
static Metadata_uint8_t object1400sub2Metadata{0b00011, 0};
static Metadata_uint8_t object1600sub0Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub1Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub2Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub3Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub4Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub5Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub6Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub7Metadata{0b00011, 0};
static Metadata_uint32_t object1600sub8Metadata{0b00011, 0};
static Metadata_uint8_t object1800sub0Metadata{0b00001, 6};
static Metadata_uint32_t object1800sub1Metadata{0b00011, 2147484033};
static Metadata_uint8_t object1800sub2Metadata{0b00011, 0};
static Metadata_uint16_t object1800sub3Metadata{0b00011, 0};
static Metadata_uint8_t object1800sub4Metadata{0b00011, 0};
static Metadata_uint16_t object1800sub5Metadata{0b00011, 0};
static Metadata_uint8_t object1800sub6Metadata{0b00011, 0};
static Metadata_uint8_t object1a00sub0Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub1Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub2Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub3Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub4Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub5Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub6Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub7Metadata{0b00011, 0};
static Metadata_uint32_t object1a00sub8Metadata{0b00011, 0};
static Metadata_uint8_t object2000sub0Metadata{0b00001, 10};
static Metadata_bool object2000sub1Metadata{0b00011, 1};
static Metadata_int8_t object2000sub2Metadata{0b00011, -8};
static Metadata_int16_t object2000sub3Metadata{0b00011, -16};
static Metadata_int32_t object2000sub4Metadata{0b00011, -32};
static Metadata_int64_t object2000sub5Metadata{0b00011, -64};
static Metadata_uint16_t object2000sub6Metadata{0b00011, 16};
static Metadata_uint32_t object2000sub7Metadata{0b00011, 32};
static Metadata_uint64_t object2000sub8Metadata{0b00011, 64};
static Metadata_float object2000sub9Metadata{0b00011, 1.5};
static Metadata_double object2000sub10Metadata{0b00011, 2.5};
static Metadata_string object2001sub0Metadata{0b00001, "", 16};
static Metadata_domain object2002sub0Metadata{0b00011};
static Metadata_uint16_t_limited object2003sub0Metadata{0b01011, 50, 10, 100};
static Metadata_uint8_t object2004sub0Metadata{0b00001, 3};
static Metadata_uint32_t object2004sub1Metadata{0b00011, 1};
static Metadata_uint32_t object2004sub2Metadata{0b00011, 2};
static Metadata_uint32_t object2004sub3Metadata{0b00011, 3};
static Metadata_uint32_t object2005sub0Metadata{0b10011, 0};
static Metadata_float object2006sub0Metadata{0b10011, 0};
static Metadata_uint8_t object2007sub0Metadata{0b00011, 0};
static Metadata_uint16_t object6040sub0Metadata{0b00111, 0};
static Metadata_uint16_t object6041sub0Metadata{0b00101, 0};
static Metadata_int16_t object6060sub0Metadata{0b00111, 0};
static Metadata_int32_t object6064sub0Metadata{0b10101, 0};

const Metadata *ObjectDictionnary::objectMetadataTable[73] = {
    /* 0000 */  &object1000sub0Metadata,
    /* 0001 */  &object1001sub0Metadata,
    /* 0002 */  &object1010sub0Metadata,
    /* 0003 */  &object1010sub1Metadata,
    /* 0004 */  &object1010sub2Metadata,
    /* 0005 */  &object1010sub3Metadata,
    /* 0006 */  &object1010sub4Metadata,
    /* 0007 */  &object1010sub5Metadata,
    /* 0008 */  &object1011sub0Metadata,
    /* 0009 */  &object1011sub1Metadata,
    /* 0010 */  &object1011sub2Metadata,
    /* 0011 */  &object1011sub3Metadata,
    /* 0012 */  &object1011sub4Metadata,
    /* 0013 */  &object1011sub5Metadata,
    /* 0014 */  &object1017sub0Metadata,
    /* 0015 */  &object1018sub0Metadata,
    /* 0016 */  &object1018sub1Metadata,
    /* 0017 */  &object1018sub2Metadata,
    /* 0018 */  &object1018sub3Metadata,
    /* 0019 */  &object1018sub4Metadata,
    /* 0020 */  &object1400sub0Metadata,
    /* 0021 */  &object1400sub1Metadata,
    /* 0022 */  &object1400sub2Metadata,
    /* 0023 */  &object1600sub0Metadata,
    /* 0024 */  &object1600sub1Metadata,
    /* 0025 */  &object1600sub2Metadata,
    /* 0026 */  &object1600sub3Metadata,
    /* 0027 */  &object1600sub4Metadata,
    /* 0028 */  &object1600sub5Metadata,
    /* 0029 */  &object1600sub6Metadata,
    /* 0030 */  &object1600sub7Metadata,
    /* 0031 */  &object1600sub8Metadata,
    /* 0032 */  &object1800sub0Metadata,
    /* 0033 */  &object1800sub1Metadata,
    /* 0034 */  &object1800sub2Metadata,
    /* 0035 */  &object1800sub3Metadata,
    /* 0036 */  &object1800sub4Metadata,
    /* 0037 */  &object1800sub5Metadata,
    /* 0038 */  &object1800sub6Metadata,
    /* 0039 */  &object1a00sub0Metadata,
    /* 0040 */  &object1a00sub1Metadata,
    /* 0041 */  &object1a00sub2Metadata,
    /* 0042 */  &object1a00sub3Metadata,
    /* 0043 */  &object1a00sub4Metadata,
    /* 0044 */  &object1a00sub5Metadata,
    /* 0045 */  &object1a00sub6Metadata,
    /* 0046 */  &object1a00sub7Metadata,
    /* 0047 */  &object1a00sub8Metadata,
    /* 0048 */  &object2000sub0Metadata,
    /* 0049 */  &object2000sub1Metadata,
    /* 0050 */  &object2000sub2Metadata,
    /* 0051 */  &object2000sub3Metadata,
    /* 0052 */  &object2000sub4Metadata,
    /* 0053 */  &object2000sub5Metadata,
    /* 0054 */  &object2000sub6Metadata,
    /* 0055 */  &object2000sub7Metadata,
    /* 0056 */  &object2000sub8Metadata,
    /* 0057 */  &object2000sub9Metadata,
    /* 0058 */  &object2000sub10Metadata,
    /* 0059 */  &object2001sub0Metadata,
    /* 0060 */  &object2002sub0Metadata,
    /* 0061 */  &object2003sub0Metadata,
    /* 0062 */  &object2004sub0Metadata,
    /* 0063 */  &object2004sub1Metadata,
    /* 0064 */  &object2004sub2Metadata,
    /* 0065 */  &object2004sub3Metadata,
    /* 0066 */  &object2005sub0Metadata,
    /* 0067 */  &object2006sub0Metadata,
    /* 0068 */  &object2007sub0Metadata,
    /* 0069 */  &object6040sub0Metadata,
    /* 0070 */  &object6041sub0Metadata,
    /* 0071 */  &object6060sub0Metadata,
    /* 0072 */  &object6064sub0Metadata,
};

/**
 * Getters/Setters for local data.
 */
static int8_t getobject1001sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return emcyGetErrorRegister(data, id, abortCode);
}
static int8_t getobject1010sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t getobject1010sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t setobject1010sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odSaveData(data, id, abortCode);
}
static int8_t getobject1010sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t setobject1010sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odSaveData(data, id, abortCode);
}
static int8_t getobject1010sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t setobject1010sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odSaveData(data, id, abortCode);
}
static int8_t getobject1010sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t setobject1010sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odSaveData(data, id, abortCode);
}
static int8_t getobject1010sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetSave(data, id, abortCode);
}
static int8_t setobject1010sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odSaveData(data, id, abortCode);
}
static int8_t getobject1011sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t getobject1011sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t setobject1011sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odRestoreData(data, id, abortCode);
}
static int8_t getobject1011sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t setobject1011sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odRestoreData(data, id, abortCode);
}
static int8_t getobject1011sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t setobject1011sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odRestoreData(data, id, abortCode);
}
static int8_t getobject1011sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t setobject1011sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odRestoreData(data, id, abortCode);
}
static int8_t getobject1011sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odGetRestore(data, id, abortCode);
}
static int8_t setobject1011sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return odRestoreData(data, id, abortCode);
}
static int8_t getobject1017sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return hbGetData(data, id, abortCode);
}
static int8_t setobject1017sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return hbSetData(data, id, abortCode);
}
static int8_t getobject1400sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoComm(data, id, abortCode);
}
static int8_t getobject1400sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoComm(data, id, abortCode);
}
static int8_t setobject1400sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoComm(data, id, abortCode);
}
static int8_t getobject1400sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoComm(data, id, abortCode);
}
static int8_t setobject1400sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoComm(data, id, abortCode);
}
static int8_t getobject1600sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1600sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetRpdoMap(data, id, abortCode);
}
static int8_t setobject1600sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetRpdoMap(data, id, abortCode);
}
static int8_t getobject1800sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1800sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoComm(data, id, abortCode);
}
static int8_t setobject1800sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoComm(data, id, abortCode);
}
static int8_t getobject1a00sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t getobject1a00sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoGetTpdoMap(data, id, abortCode);
}
static int8_t setobject1a00sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return pdoSetTpdoMap(data, id, abortCode);
}
static int8_t setobject2003sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    abortCode = data.u16 < 10 ? SDOAbortCode_DownloadValueTooLow : data.u16 > 100 ? SDOAbortCode_DownloadValueTooHigh : SDOAbortCode_OK;
    if (abortCode != SDOAbortCode_OK) return -1;
    return setLocalData_uint16_t(data, id, abortCode);
}

int8_t (*ObjectDictionnary::objectGetterTable[73])(Data &data, int32_t id, SDOAbortCodes &abortCode) = {
    /* 0000 - 0x1000 */    &getLocalData_uint32_t,
    /* 0001 - 0x1001 */    &getobject1001sub0,
    /* 0002 - 0x1010 */    &getobject1010sub0,
    /* 0003 - 0x1010 */    &getobject1010sub1,
    /* 0004 - 0x1010 */    &getobject1010sub2,
    /* 0005 - 0x1010 */    &getobject1010sub3,
    /* 0006 - 0x1010 */    &getobject1010sub4,
    /* 0007 - 0x1010 */    &getobject1010sub5,
    /* 0008 - 0x1011 */    &getobject1011sub0,
    /* 0009 - 0x1011 */    &getobject1011sub1,
    /* 0010 - 0x1011 */    &getobject1011sub2,
    /* 0011 - 0x1011 */    &getobject1011sub3,
    /* 0012 - 0x1011 */    &getobject1011sub4,
    /* 0013 - 0x1011 */    &getobject1011sub5,
    /* 0014 - 0x1017 */    &getobject1017sub0,
    /* 0015 - 0x1018 */    &getLocalData_uint8_t,
    /* 0016 - 0x1018 */    &getLocalData_uint32_t,
    /* 0017 - 0x1018 */    &getLocalData_uint32_t,
    /* 0018 - 0x1018 */    &getLocalData_uint32_t,
    /* 0019 - 0x1018 */    &getLocalData_uint32_t,
    /* 0020 - 0x1400 */    &getobject1400sub0,
    /* 0021 - 0x1400 */    &getobject1400sub1,
    /* 0022 - 0x1400 */    &getobject1400sub2,
    /* 0023 - 0x1600 */    &getobject1600sub0,
    /* 0024 - 0x1600 */    &getobject1600sub1,
    /* 0025 - 0x1600 */    &getobject1600sub2,
    /* 0026 - 0x1600 */    &getobject1600sub3,
    /* 0027 - 0x1600 */    &getobject1600sub4,
    /* 0028 - 0x1600 */    &getobject1600sub5,
    /* 0029 - 0x1600 */    &getobject1600sub6,
    /* 0030 - 0x1600 */    &getobject1600sub7,
    /* 0031 - 0x1600 */    &getobject1600sub8,
    /* 0032 - 0x1800 */    &getobject1800sub0,
    /* 0033 - 0x1800 */    &getobject1800sub1,
    /* 0034 - 0x1800 */    &getobject1800sub2,
    /* 0035 - 0x1800 */    &getobject1800sub3,
    /* 0036 - 0x1800 */    &getobject1800sub4,
    /* 0037 - 0x1800 */    &getobject1800sub5,
    /* 0038 - 0x1800 */    &getobject1800sub6,
    /* 0039 - 0x1a00 */    &getobject1a00sub0,
    /* 0040 - 0x1a00 */    &getobject1a00sub1,
    /* 0041 - 0x1a00 */    &getobject1a00sub2,
    /* 0042 - 0x1a00 */    &getobject1a00sub3,
    /* 0043 - 0x1a00 */    &getobject1a00sub4,
    /* 0044 - 0x1a00 */    &getobject1a00sub5,
    /* 0045 - 0x1a00 */    &getobject1a00sub6,
    /* 0046 - 0x1a00 */    &getobject1a00sub7,
    /* 0047 - 0x1a00 */    &getobject1a00sub8,
    /* 0048 - 0x2000 */    &getLocalData_uint8_t,
    /* 0049 - 0x2000 */    &getLocalData_bool,
    /* 0050 - 0x2000 */    &getLocalData_int8_t,
    /* 0051 - 0x2000 */    &getLocalData_int16_t,
    /* 0052 - 0x2000 */    &getLocalData_int32_t,
    /* 0053 - 0x2000 */    &getLocalData_int64_t,
    /* 0054 - 0x2000 */    &getLocalData_uint16_t,
    /* 0055 - 0x2000 */    &getLocalData_uint32_t,
    /* 0056 - 0x2000 */    &getLocalData_uint64_t,
    /* 0057 - 0x2000 */    &getLocalData_float,
    /* 0058 - 0x2000 */    &getLocalData_double,
    /* 0059 - 0x2001 */    &getLocalData_string,
    /* 0060 - 0x2002 */    &getLocalData_domain,
    /* 0061 - 0x2003 */    &getLocalData_uint16_t,
    /* 0062 - 0x2004 */    &getLocalData_uint8_t,
    /* 0063 - 0x2004 */    &getLocalData_uint32_t,
    /* 0064 - 0x2004 */    &getLocalData_uint32_t,
    /* 0065 - 0x2004 */    &getLocalData_uint32_t,
    /* 0066 - 0x2005 */    &getRemoteData,
    /* 0067 - 0x2006 */    &getRemoteData,
    /* 0068 - 0x2007 */    &getLocalData_uint8_t,
    /* 0069 - 0x6040 */    &getLocalData_uint16_t,
    /* 0070 - 0x6041 */    &getLocalData_uint16_t,
    /* 0071 - 0x6060 */    &getLocalData_int16_t,
    /* 0072 - 0x6064 */    &getRemoteData,
};

// This table contain all the objects setter
int8_t (*ObjectDictionnary::objectSetterTable[73])(const Data &data, int32_t id, SDOAbortCodes &abortCode) = {
    /* 0000 */ &setLocalData_uint32_t,
    /* 0001 */ &setLocalData_uint8_t,
    /* 0002 */ &setLocalData_uint8_t,
    /* 0003 */ &setobject1010sub1,
    /* 0004 */ &setobject1010sub2,
    /* 0005 */ &setobject1010sub3,
    /* 0006 */ &setobject1010sub4,
    /* 0007 */ &setobject1010sub5,
    /* 0008 */ &setLocalData_uint8_t,
    /* 0009 */ &setobject1011sub1,
    /* 0010 */ &setobject1011sub2,
    /* 0011 */ &setobject1011sub3,
    /* 0012 */ &setobject1011sub4,
    /* 0013 */ &setobject1011sub5,
    /* 0014 */ &setobject1017sub0,
    /* 0015 */ &setLocalData_uint8_t,
    /* 0016 */ &setLocalData_uint32_t,
    /* 0017 */ &setLocalData_uint32_t,
    /* 0018 */ &setLocalData_uint32_t,
    /* 0019 */ &setLocalData_uint32_t,
    /* 0020 */ &setLocalData_uint8_t,
    /* 0021 */ &setobject1400sub1,
    /* 0022 */ &setobject1400sub2,
    /* 0023 */ &setobject1600sub0,
    /* 0024 */ &setobject1600sub1,
    /* 0025 */ &setobject1600sub2,
    /* 0026 */ &setobject1600sub3,
    /* 0027 */ &setobject1600sub4,
    /* 0028 */ &setobject1600sub5,
    /* 0029 */ &setobject1600sub6,
    /* 0030 */ &setobject1600sub7,
    /* 0031 */ &setobject1600sub8,
    /* 0032 */ &setLocalData_uint8_t,
    /* 0033 */ &setobject1800sub1,
    /* 0034 */ &setobject1800sub2,
    /* 0035 */ &setobject1800sub3,
    /* 0036 */ &setobject1800sub4,
    /* 0037 */ &setobject1800sub5,
    /* 0038 */ &setobject1800sub6,
    /* 0039 */ &setobject1a00sub0,
    /* 0040 */ &setobject1a00sub1,
    /* 0041 */ &setobject1a00sub2,
    /* 0042 */ &setobject1a00sub3,
    /* 0043 */ &setobject1a00sub4,
    /* 0044 */ &setobject1a00sub5,
    /* 0045 */ &setobject1a00sub6,
    /* 0046 */ &setobject1a00sub7,
    /* 0047 */ &setobject1a00sub8,
    /* 0048 */ &setLocalData_uint8_t,
    /* 0049 */ &setLocalData_bool,
    /* 0050 */ &setLocalData_int8_t,
    /* 0051 */ &setLocalData_int16_t,
    /* 0052 */ &setLocalData_int32_t,
    /* 0053 */ &setLocalData_int64_t,
    /* 0054 */ &setLocalData_uint16_t,
    /* 0055 */ &setLocalData_uint32_t,
    /* 0056 */ &setLocalData_uint64_t,
    /* 0057 */ &setLocalData_float,
    /* 0058 */ &setLocalData_double,
    /* 0059 */ &setLocalData_string,
    /* 0060 */ &setLocalData_domain,
    /* 0061 */ &setobject2003sub0,
    /* 0062 */ &setLocalData_uint8_t,
    /* 0063 */ &setLocalData_uint32_t,
    /* 0064 */ &setLocalData_uint32_t,
    /* 0065 */ &setLocalData_uint32_t,
    /* 0066 */ &setRemoteData,
    /* 0067 */ &setRemoteData,
    /* 0068 */ &setLocalData_uint8_t,
    /* 0069 */ &setLocalData_uint16_t,
    /* 0070 */ &setLocalData_uint16_t,
    /* 0071 */ &setLocalData_int16_t,
    /* 0072 */ &setLocalData_int32_t,
};
// END GETTER/SETTER


// Default values for bool
bool ObjectDictionnary::boolTable[1] = {
    /* 0000 - 049 */ 1,
};
// Default values for i8
int8_t ObjectDictionnary::i8Table[1] = {
    /* 0000 - 050 */ -8,
};
// Default values for i16
int16_t ObjectDictionnary::i16Table[2] = {
    /* 0000 - 051 */ -16,
    /* 0001 - 071 */ 0,
};
// Default values for i32
int32_t ObjectDictionnary::i32Table[2] = {
    /* 0000 - 052 */ -32,
    /* 0001 - 072 */ 0,
};
// Default values for i64
int64_t ObjectDictionnary::i64Table[1] = {
    /* 0000 - 053 */ -64,
};
// Default values for u8
uint8_t ObjectDictionnary::u8Table[15] = {
    /* 0000 - 001 */ 0,
    /* 0001 - 002 */ 5,
    /* 0002 - 008 */ 5,
    /* 0003 - 015 */ 4,
    /* 0004 - 020 */ 2,
    /* 0005 - 022 */ 0,
    /* 0006 - 023 */ 0,
    /* 0007 - 032 */ 6,
    /* 0008 - 034 */ 0,
    /* 0009 - 036 */ 0,
    /* 0010 - 038 */ 0,
    /* 0011 - 039 */ 0,
    /* 0012 - 048 */ 10,
    /* 0013 - 062 */ 3,
    /* 0014 - 068 */ 0,
};
// Default values for u16
uint16_t ObjectDictionnary::u16Table[7] = {
    /* 0000 - 014 */ 1000,
    /* 0001 - 035 */ 0,
    /* 0002 - 037 */ 0,
    /* 0003 - 054 */ 16,
    /* 0004 - 061 */ 50,
    /* 0005 - 069 */ 0,
    /* 0006 - 070 */ 0,
};
// Default values for u32
uint32_t ObjectDictionnary::u32Table[38] = {
    /* 0000 - 000 */ 0x00000192,
    /* 0001 - 003 */ 0,
    /* 0002 - 004 */ 0,
    /* 0003 - 005 */ 0,
    /* 0004 - 006 */ 0,
    /* 0005 - 007 */ 0,
    /* 0006 - 009 */ 0,
    /* 0007 - 010 */ 0,
    /* 0008 - 011 */ 0,
    /* 0009 - 012 */ 0,
    /* 0010 - 013 */ 0,
    /* 0011 - 016 */ 0,
    /* 0012 - 017 */ 1,
    /* 0013 - 018 */ 1,
    /* 0014 - 019 */ 0,
    /* 0015 - 021 */ 2147484161,
    /* 0016 - 024 */ 0,
    /* 0017 - 025 */ 0,
    /* 0018 - 026 */ 0,
    /* 0019 - 027 */ 0,
    /* 0020 - 028 */ 0,
    /* 0021 - 029 */ 0,
    /* 0022 - 030 */ 0,
    /* 0023 - 031 */ 0,
    /* 0024 - 033 */ 2147484033,
    /* 0025 - 040 */ 0,
    /* 0026 - 041 */ 0,
    /* 0027 - 042 */ 0,
    /* 0028 - 043 */ 0,
    /* 0029 - 044 */ 0,
    /* 0030 - 045 */ 0,
    /* 0031 - 046 */ 0,
    /* 0032 - 047 */ 0,
    /* 0033 - 055 */ 32,
    /* 0034 - 063 */ 1,
    /* 0035 - 064 */ 2,
    /* 0036 - 065 */ 3,
    /* 0037 - 066 */ 0,
};
// Default values for u64
uint64_t ObjectDictionnary::u64Table[1] = {
    /* 0000 - 056 */ 64,
};
// Default values for f32
float ObjectDictionnary::f32Table[2] = {
    /* 0000 - 057 */ 1.5,
    /* 0001 - 067 */ 0,
};
// Default values for f64
double ObjectDictionnary::f64Table[1] = {
    /* 0000 - 058 */ 2.5,
};

// string type data variable
char ObjectDictionnary::object2001sub0[16] = "";

// string type data table
char *ObjectDictionnary::stringTable[1] = {
    object2001sub0,
};

// index data table

uint8_t ObjectDictionnary::dataIndexTable[73] = {
    0,
    0,
    1,
    1,
    2,
    3,
    4,
    5,
    2,
    6,
    7,
    8,
    9,
    10,
    0,
    3,
    11,
    12,
    13,
    14,
    4,
    15,
    5,
    6,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    7,
    24,
    8,
    1,
    9,
    2,
    10,
    11,
    25,
    26,
    27,
    28,
    29,
    30,
    31,
    32,
    12,
    0,
    0,
    0,
    0,
    0,
    3,
    33,
    0,
    0,
    0,
    0,
    0,
    4,
    13,
    34,
    35,
    36,
    37,
    1,
    14,
    5,
    6,
    1,
    1,
};
//END DATA
