/**
 * Contains the declarations of the ObjectDictionnary class.
 */
#include "od.hpp"

#include "node.hpp"
using namespace CANopen;

ObjectDictionnary::ObjectDictionnary(Node &node) : node(node) {}

Object *ObjectDictionnary::findObject(uint16_t objectIndex) {
    int32_t lower = 0;
    int32_t upper = length - 1;
    while (lower <= upper) {
        int32_t mid = lower + (upper - lower) / 2;
        if (objectsArray[mid]->index == objectIndex)
            return objectsArray[mid];
        else if (objectsArray[mid]->index < objectIndex)
            lower = mid + 1;
        else
            upper = mid - 1;
    }
    return nullptr;
}

Object *ObjectDictionnary::at(uint16_t index) {
    return index < OD_OBJECTS_COUNT ? objectsArray[index] : nullptr;
}

Object *ObjectDictionnary::operator[](uint16_t index) { return at(index); }




uint8_t ObjectDictionnary::getLocalData(Data &data, uint16_t id){
    data = dataTable[id];
    return 0;
}

uint8_t ObjectDictionnary::setLocalData(Data data, uint16_t id){
    dataTable[id] = data;
    return 0;
}

uint8_t ObjectDictionnary::get1A00(Data &data, uint16_t id) {
    return getLocalData(data, id);
}

uint8_t ObjectDictionnary::set1A00(Data data, uint16_t id,
                                   SDOAbortCodes &abortCode) {
    if (objectIndexTable[id].second == OBJECT_INDEX_COUNT) {
        uint8_t value = data.u8;
        if (value > OD_PDO_MAPPING_MAX)
            abortCode = SDOAbortCode_DownloadValueTooHigh;
        if (value > X1A00_MAP_DISABLED) {
            uint32_t sizeSum = 0;
            for (unsigned i = 0; i < value; i++) {
                PDOMapEntry entry = dataTable[i + id + 1].u32;
                sizeSum += dataTable[i + id + 1].size;
            }
            if (sizeSum > PDO_DLC)
                abortCode = SDOAbortCode_MappedPDOLengthExceeded;
        }
    } else {
        if (objectMetaDataTable[id].bits.mappable == 0 ||
            objectMetaDataTable[id].bits.readable == 0) {
            abortCode = SDOAbortCode_CannotMapToPDO;
        }
    }
    abortCode = SDOAbortCode_OK;
    return 0;
}


