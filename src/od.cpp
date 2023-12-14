/**
 * Contains the declarations of the ObjectDictionnary class.
 */
#include "od.hpp"
#include "node.hpp"
using namespace CANopen;

ObjectDictionnary::ObjectDictionnary(Node &node, ObjectDictionnaryData &data) : data(data), node(node) {}

Object *ObjectDictionnary::findObject(uint16_t objectIndex)
{
    int32_t lower = 0;
    int32_t upper = length - 1;
    while (lower <= upper)
    {
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

Object *ObjectDictionnary::at(uint16_t index)
{
    return index < OD_OBJECTS_COUNT ? objectsArray[index] : nullptr;
}

Object *ObjectDictionnary::operator[](uint16_t index)
{
    return at(index);
}
