/**
 * Contains the declarations of the ObjectDictionnary class.
 */
#include "od.hpp"
#include "node.hpp"


using namespace CANopen;

//TODO: add const ref

int8_t getobject1400sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub0(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub1(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub2(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub0(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub1(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub2(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub3(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub4(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub5(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub6(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub7(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub7(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub8(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub8(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub0(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub1(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub2(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub3(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub4(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub5(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub6(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub0(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub0(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub1(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub1(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub2(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub2(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub3(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub3(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub4(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub4(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub5(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub5(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub6(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub6(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub7(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub7(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1A00sub8(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1A00sub8(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}

int8_t getLocalData(Data &data, uint32_t id, SDOAbortCodes &abortCode){
    data = node.od().objectDataTable[id];
    return 0;
}

int8_t setLocalData(Data const &data, uint32_t id, SDOAbortCodes &abortCode){
    node.od().objectDataTable[id] = data;
    return 0;
}

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