/**
 * Contains the declarations of the ObjectDictionnary class.
 */
#include "od.hpp"
#include "node.hpp"
#include "driverlib_cm.h"
extern "C" {
#include "cm.h"
}

#include <cstring>


using namespace CANopen;

int8_t getobject1001sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorRegister.getData(data, id, abortCode);
}
int8_t setobject1001sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorRegister.setData(data, id, abortCode);
}
int8_t getobject1003sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1003sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.getData(data, id, abortCode);
}
int8_t setobject1003sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().preDefinedErrorField.setData(data, id, abortCode);
}
int8_t getobject1010sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1010sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1010sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1010sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1010sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1010sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getSave(data, id, abortCode);
}
int8_t setobject1010sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().saveData(data, id, abortCode);
}
int8_t getobject1011sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1011sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1011sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1011sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1011sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1011sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().getRestore(data, id, abortCode);
}
int8_t setobject1011sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.od().restoreData(data, id, abortCode);
}
int8_t getobject1017sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.hb().getData(data, id, abortCode);
}
int8_t setobject1017sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.hb().setData(data, id, abortCode);
}
int8_t getobject1019sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.sync().getData(data, id, abortCode);
}
int8_t setobject1019sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.sync().setData(data, id, abortCode);
}
int8_t getobject1029sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.getData(data, id, abortCode);
}
int8_t setobject1029sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.setData(data, id, abortCode);
}
int8_t getobject1029sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.getData(data, id, abortCode);
}
int8_t setobject1029sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.setData(data, id, abortCode);
}
int8_t getobject1029sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.getData(data, id, abortCode);
}
int8_t setobject1029sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.emcy().errorBehavior.setData(data, id, abortCode);
}
int8_t getobject1400sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1400sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1400sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1401sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1401sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1401sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1401sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1401sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1401sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1402sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1402sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1402sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1402sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1402sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1402sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1403sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1403sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1403sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1403sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1403sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1403sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1600sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1600sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1601sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1601sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1602sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1602sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1603sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1603sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().rpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1800sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.getData(data, id, abortCode);
}
int8_t setobject1800sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1801sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.getData(data, id, abortCode);
}
int8_t setobject1801sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1802sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.getData(data, id, abortCode);
}
int8_t setobject1802sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1803sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.getData(data, id, abortCode);
}
int8_t setobject1803sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].commParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a00sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a00sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[0].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a01sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a01sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[1].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a02sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a02sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[2].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub0(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub0(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub1(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub1(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub2(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub2(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub3(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub3(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub4(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub4(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub5(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub5(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub6(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub6(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub7(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub7(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}
int8_t getobject1a03sub8(Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.getData(data, id, abortCode);
}
int8_t setobject1a03sub8(const Data &data, int32_t id, SDOAbortCodes &abortCode){
    return node.pdo().tpdos[3].mapParameter.setData(data, id, abortCode);
}

int8_t getLocalData_bool(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.b = node.od().boolTable[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_bool(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().boolTable[node.od().dataIndexTable[id]] = data.b;
    return 0;
}

int8_t getLocalData_int8_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i8 = node.od().i8Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int8_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().i8Table[node.od().dataIndexTable[id]] = data.i8;
    return 0;
}

int8_t getLocalData_int16_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i16 = node.od().i16Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int16_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().i16Table[node.od().dataIndexTable[id]] = data.i16;
    return 0;
}

int8_t getLocalData_int32_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.i32 = node.od().i32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_int32_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().i32Table[node.od().dataIndexTable[id]] = data.i32;
    return 0;
}

int8_t getLocalData_int64_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   data.i64 = node.od().i64Table[node.od().dataIndexTable[id]];
   return 0;
}

int8_t setLocalData_int64_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   node.od().i64Table[node.od().dataIndexTable[id]] = data.i64;
   return 0;
}

int8_t getLocalData_uint8_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u8 = node.od().u8Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint8_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().u8Table[node.od().dataIndexTable[id]] = data.u8;
    return 0;
}

int8_t getLocalData_uint16_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u16 = node.od().u16Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint16_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().u16Table[node.od().dataIndexTable[id]] = data.u16;
    return 0;
}

int8_t getLocalData_uint32_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.u32 = node.od().u32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_uint32_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().u32Table[node.od().dataIndexTable[id]] = data.u32;
    return 0;
}

int8_t getLocalData_uint64_t(Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   data.u64 = node.od().u64Table[node.od().dataIndexTable[id]];
   return 0;
}

int8_t setLocalData_uint64_t(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   node.od().u64Table[node.od().dataIndexTable[id]] = data.u64;
   return 0;
}

int8_t getLocalData_float(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    data.f32 = node.od().f32Table[node.od().dataIndexTable[id]];
    return 0;
}

int8_t setLocalData_float(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    node.od().f32Table[node.od().dataIndexTable[id]] = data.f32;
    return 0;
}

int8_t getLocalData_double(Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   data.f64 = node.od().f64Table[node.od().dataIndexTable[id]];
   return 0;
}

int8_t setLocalData_double(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
   abortCode = SDOAbortCode_OK;
   node.od().f64Table[node.od().dataIndexTable[id]] = data.f64;
   return 0;
}

int8_t getLocalData_char(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return -1; //TODO: implement
}

int8_t setLocalData_char(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return -1; //TODO: implement
}

int8_t getLocalData_domain(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return -1; //TODO: implement
}

int8_t setLocalData_domain(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    return -1; //TODO: implement
}

int8_t getRemoteData(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    node.hardware().getRemoteData(data, id, abortCode);
}

int8_t setRemoteData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    node.hardware().setRemoteData(data, id, abortCode);
}


int32_t ObjectDictionnary::findObject(uint16_t index) {
    int32_t lower = 0;
    int32_t upper = length - 1;
    while (lower <= upper) {
        int32_t mid = lower + (upper - lower) / 2;
        if (CANopenOD::objectIndexTable[mid].first == index && CANopenOD::objectIndexTable[mid].second == 0)
            return mid;
        else if (CANopenOD::objectIndexTable[mid].first < index)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return -1;
}

int32_t ObjectDictionnary::findObject(uint16_t index, uint8_t subindex) {
    int32_t indexPos = findObject(index);
    if (indexPos == -1) return -1;
    int32_t id = indexPos + subindex;
    if (CANopenOD::objectIndexTable[id].first == index &&
        CANopenOD::objectIndexTable[id].second == subindex)
        return id;
    return -2;
}

int8_t ObjectDictionnary::readData(Data &data, uint16_t index, uint8_t subindex,
                                   SDOAbortCodes &abortCode) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        abortCode = SDOAbortCodes::SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    return readData(data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, uint16_t index, uint8_t subindex,
                                    SDOAbortCodes &abortCode) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        abortCode = SDOAbortCodes::SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    abortCode = SDOAbortCode_OK;
    return writeData(data, id, abortCode);
}

int8_t ObjectDictionnary::readData(Data &data, int32_t id,
                                   SDOAbortCodes &abortCode) {
    return objectGetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, int32_t id,
                                    SDOAbortCodes &abortCode) {
    return objectSetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::readData(Data &data, uint16_t index,
                                   uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return -1;
    }
    return readData(data, id);
}

int8_t ObjectDictionnary::writeData(const Data &data, uint16_t index,
                                    uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return -1;
    }
    return writeData(data, id);
}

int8_t ObjectDictionnary::readData(Data &data, int32_t id) {
    SDOAbortCodes abortCode;
    return objectGetterTable[id](data, id, abortCode);
}

int8_t ObjectDictionnary::writeData(const Data &data, int32_t id) {
    SDOAbortCodes abortCode;
    return objectSetterTable[id](data, id, abortCode);
}

bool ObjectDictionnary::saveData(uint8_t parameterGroup) {
    if (parameterGroup == 1){
        uint64_t signature;
        node.hardware().loadSignatureFromFlash(signature);
        if (signature != od_signature){
            node.hardware().saveSignatureToFlash(od_signature);
        }
        bool result = true;
        result &= node.hardware().saveDataToFlash(ParameterGroup_Communication);
        result &= node.hardware().saveDataToFlash(ParameterGroup_Application); //TODO: meta generate
        result &= node.hardware().saveDataToFlash(ParameterGroup_ManufacturerA); 
        return result;
    }
    return node.hardware().saveDataToFlash(parameterGroup);
}

int8_t ObjectDictionnary::saveData(const Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (data.u32 != SAVE_SIGNATURE){
        abortCode = SDOAbortCode_CannotStoreOrTransfer;
        return 0;
    }
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (saveData(id - odID)) {
        return 0;
    }else{
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
}

int8_t ObjectDictionnary::getSave(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    data.u32 = 0x01; //bit 1: auto save, bit 0: save at command
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    return 0;
}

bool ObjectDictionnary::loadData(uint8_t parameterGroup) {
    uint64_t signature;
    node.hardware().loadSignatureFromFlash(signature);
    if (signature != od_signature){
        return false;
    }
    if (parameterGroup == 1){
        node.hardware().loadDataFromFlash(ParameterGroup_Communication);
        node.hardware().loadDataFromFlash(ParameterGroup_Application); //TODO: meta generate
        node.hardware().loadDataFromFlash(ParameterGroup_Application); // load 2 time for load first unit TODO: find better way
        node.hardware().loadDataFromFlash(ParameterGroup_ManufacturerA);
        return true;
    }
    return node.hardware().loadDataFromFlash(parameterGroup);
}


bool ObjectDictionnary::restoreData(uint8_t parameterGroup) {
    Data tmp;
    int16_t size;
    uint16_t minIndex = 0x0000;
    uint16_t maxIndex = 0xFFFF;
    SDOAbortCodes abortCode;
    int count = 0;

    if (parameterGroup == ParameterGroup_Communication) { //TODO: meta generate
        minIndex = 0x1000; //TODO: move to define
        maxIndex = 0x1FFF;
    } else if (parameterGroup == ParameterGroup_Application) {
        minIndex = 0x6000;
        maxIndex = 0x9FFF;
    } else if (parameterGroup == ParameterGroup_ManufacturerA) {
        minIndex = 0x2000;
        maxIndex = 0x4FFF;
    }
    for (int i = 0; i < length; i++) {
        if (CANopenOD::objectIndexTable[i].first >= minIndex &&
            CANopenOD::objectIndexTable[i].first <= maxIndex) {
            size = getSize(i);
            tmp = getMetadata(i)->getDefaultValue();
            int8_t ret;
            count = 0;
            while (writeData(tmp, i, abortCode) == 1) {
                count++;
                if (count > MAX_WRITE_TRY) {
                    break;
                }
            }
            DEVICE_DELAY_US(100);  // FIXME: understand why this is needed
        }
    }
    return true;
}

int8_t ObjectDictionnary::restoreData(const Data &data, int32_t id,
                                      SDOAbortCodes &abortCode) {
    if (data.u32 != RESTORE_SIGNATURE){
        abortCode = SDOAbortCode_CannotStoreOrTransfer;
        return 0;
    }
    int32_t odID = findObject(RESTORE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    if (restoreData(id - odID)) {
        return 0;
    }else{
        abortCode = SDOAbortCode_AccessFailedHardwareError;
        return -1;
    }
}

int8_t ObjectDictionnary::getRestore(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    data.u32 = 0x01; //bit 0: restore at command
    int32_t odID = findObject(SAVE_INDEX);
    if (id == -1) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    if ((id - odID) > MAX_SUB_INDEX) {
        abortCode = SDOAbortCode_SubindexNonExistent;
        return -1;
    }
    return 0;
}

bool ObjectDictionnary::isSubValid(uint16_t index, uint8_t subindex) {
    return findObject(index, subindex) != -1;
}

struct Metadata* ObjectDictionnary::getMetadata(uint16_t index,
                                               uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id < 0) {
        Metadata metadata;
        return &metadata;
    }
    return (Metadata*)objectMetadataTable[id];
}

struct Metadata* ObjectDictionnary::getMetadata(int32_t id) {
    return (Metadata*)objectMetadataTable[id];
}

Data *ObjectDictionnary::getData(uint16_t index, uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return nullptr;
    }
    return getData((uint32_t)id);
}

Data *ObjectDictionnary::getData(int32_t id) {
    Data data;
    SDOAbortCodes abortCode;
    objectGetterTable[id](data, id, abortCode);
    return &data;
}

void ObjectDictionnary::setData(const Data &data, uint16_t index,
                                uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return;
    }
    setData(data, (uint32_t)id);
}

void ObjectDictionnary::setData(const Data &data, int32_t id) {
    SDOAbortCodes abortCode;
    objectSetterTable[id](data, id, abortCode);
}

uint16_t ObjectDictionnary::getSize(uint16_t index, uint8_t subindex) {
    int32_t id = findObject(index, subindex);
    if (id == -1) {
        return -1;
    }
    return getSize((uint32_t)id);
}

uint16_t ObjectDictionnary::getSize(int32_t id) {
    switch (objectMetadataTable[id]->dataType) {
        case 0x01: //TODO: use enume type. now limitation for TI ethercat stack
            return sizeof(bool);
        case DataType::INTEGER8:
            return sizeof(int8_t);
        case DataType::INTEGER16:
            return sizeof(int16_t);
        case DataType::INTEGER32:
            return sizeof(int32_t);
        case DataType::INTEGER64:
            return sizeof(int64_t);
        case DataType::UNSIGNED8:
            return sizeof(uint8_t);
        case DataType::UNSIGNED16:
            return sizeof(uint16_t);
        case DataType::UNSIGNED32:
            return sizeof(uint32_t);
        case DataType::UNSIGNED64:
            return sizeof(uint64_t);
        case 0x08:
            return sizeof(float);
        case 0x11:
            return sizeof(double);
        case DataType::VISIBLE_STRING:
            return sizeof(char);  // TODO: read string length
        case DataType::DOMAIN:
            return DOMAIN_MAX_SIZE;
        default:
            return 0;
    }
}