#include <fstream>
#include <cstdlib>
#include "od.hpp"
#include "enums.hpp"

void OD_Data::load()
{
    std::ifstream f("file.dat", std::ios::in | std::ios::binary);
    if (!f)
        return;
    f.read((char *)this, sizeof(*this));
    f.close();
}

void OD_Data::save()
{
    std::ofstream f("file.dat", std::ios::out | std::ios::binary);
    if (!f)
        return;
    f.write((char *)this, sizeof(*this));
    f.close();
}

OD_ObjectEntry *OD_ObjectDictionnary::findEntry(uint16_t index)
{
    for (unsigned i = 0; i < size; i++)
        if (entries[i].index == index)
            return &entries[i];
    printf("[OD] entry 0x%04X not found\n", index);
    return NULL;
}

// unsigned OD_ObjectEntry::getSize(uint8_t subIndex)
// {
//     OD_Object obj = objects[subIndex];
//     switch (obj.dataType)
//     {
//     case DataType_BOOLEAN:
//     case DataType_INTEGER8:
//     case DataType_UNSIGNED8:
//         return 1;
//     case DataType_INTEGER16:
//     case DataType_UNSIGNED16:
//         return 2;
//     case DataType_INTEGER32:
//     case DataType_UNSIGNED32:
//     case DataType_REAL32:
//     case DataType_INTEGER24:
//     case DataType_UNSIGNED24:
//         return 4;
//     case DataType_REAL64:
//     case DataType_INTEGER40:
//     case DataType_INTEGER48:
//     case DataType_INTEGER56:
//     case DataType_INTEGER64:
//     case DataType_UNSIGNED40:
//     case DataType_UNSIGNED48:
//     case DataType_UNSIGNED56:
//     case DataType_UNSIGNED64:
//         return 8;
//     default:
//         return 0;
//     }
// }
