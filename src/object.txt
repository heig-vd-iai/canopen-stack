/**
 * Contains the definitions of the Object class.
 */
#include "object.hpp"

#include <cstring>

#include "enums.hpp"
#include "node.hpp"
#include "unions.hpp"
using namespace CANopen;

RecordObject::RecordObject(uint16_t index, uint16_t subObjectCount,
                           ISubObject** subObjects)
    : index(index), subObjectCount(subObjectCount), subObjects(subObjects) {}

uint16_t RecordObject::getIndex() { return index; }

void RecordObject::setData(uint8_t subIndex, uint8_t* buffer, uint16_t size) {
    subObjects[subIndex]->setData(buffer, size);
}

void RecordObject::getData(uint8_t subIndex, uint8_t* buffer, uint16_t size) {
    subObjects[subIndex]->getData(buffer, size);
}

bool RecordObject::isSubValid(uint8_t subIndex) {
    return subIndex < subObjectCount;
}

uint16_t RecordObject::getSize(uint8_t subIndex) {
    return subObjects[subIndex]->getSize();
}

uint16_t RecordObject::getSubObjectCount() { return subObjectCount; }

MetaBitfield RecordObject::getMetadata(uint8_t subIndex) {
    if (isSubValid(subIndex)) {
        return subObjects[subIndex]->getMetadata();
    }
}
