/**
 * Contains the declarations of the ObjectDictionnary class.
 */
#include "od.hpp"
#include "node.hpp"


using namespace CANopen;

int8_t getobject1400sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub0(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub1(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub2(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub0(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub1(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub2(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub3(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub4(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub5(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub6(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub7(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub7(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub8(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub8(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub0(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub1(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub2(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub3(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub4(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub5(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub6(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub0(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub1(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub2(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub3(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub4(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub5(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub6(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub7(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub7(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub8(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub8(const Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}

int8_t getLocalDataBool(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.b = node.od().boolTable[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataBool(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().boolTable[node.od().dataIndexTable[id]] = data.b;
    return 0;
}

int8_t getLocalDataI8(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.i8 = node.od().i8Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataI8(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().i8Table[node.od().dataIndexTable[id]] = data.i8;
    return 0;
}

int8_t getLocalDataI16(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.i16 = node.od().i16Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataI16(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().i16Table[node.od().dataIndexTable[id]] = data.i16;
    return 0;
}

int8_t getLocalDataI32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.i32 = node.od().i32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataI32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().i32Table[node.od().dataIndexTable[id]] = data.i32;
    return 0;
}

int8_t getLocalDataI64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.i64 = node.od().i64Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataI64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().i64Table[node.od().dataIndexTable[id]] = data.i64;
    return 0;
}

int8_t getLocalDataU8(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.u8 = node.od().u8Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataU8(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().u8Table[node.od().dataIndexTable[id]] = data.u8;
    return 0;
}

int8_t getLocalDataU16(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.u16 = node.od().u16Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataU16(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().u16Table[node.od().dataIndexTable[id]] = data.u16;
    return 0;
}

int8_t getLocalDataU32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.u32 = node.od().u32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataU32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().u32Table[node.od().dataIndexTable[id]] = data.u32;
    return 0;
}

int8_t getLocalDataU64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.u64 = node.od().u64Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataU64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().u64Table[node.od().dataIndexTable[id]] = data.u64;
    return 0;
}

int8_t getLocalDataF32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.f32 = node.od().f32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataF32(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().f32Table[node.od().dataIndexTable[id]] = data.f32;
    return 0;
}

int8_t getLocalDataF64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    data.f64 = node.od().f64Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalDataF64(Data data, uint32_t id, SDOAbortCodes &abortCode) {
    node.od().f64Table[node.od().dataIndexTable[id]] = data.f64;
    return 0;
}

//TODO: add get/set for string

int64_t ObjectDictionnary::findObject(uint16_t objectIndex) {
    int32_t lower = 0;
    int32_t upper = length - 1;
    while (lower <= upper) {
        int32_t mid = lower + (upper - lower) / 2;
        if (objectIndexTable[mid].first == objectIndex)
            return mid;
        else if (objectIndexTable[mid].first < objectIndex)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return -1;
}