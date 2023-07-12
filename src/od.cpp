#include <fstream>
#include <cstdlib>
#include "od.hpp"

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
    for (int i = 0; i < size; i++)
        if (entries[i].index == index)
            return &entries[i];
    return NULL;
}
