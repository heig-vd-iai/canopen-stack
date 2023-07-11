#include <fstream>
#include <cstdlib>
#include "od_data.hpp"

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

void OD_Data::print()
{
    printf("x1005: %d\n", x1005);
    printf("x1006: %d\n", x1006);
    printf("x1007: [%d, %d]\n", x1007[0], x1007[1]);
    printf("x1008: {%d, %d}\n", x1008.sub1, x1008.sub2);
}
