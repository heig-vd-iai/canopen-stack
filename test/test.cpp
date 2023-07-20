#include <cstdint>
#include <cstdio>
#include <iostream>
#include "od.hpp"
using namespace std;
using namespace od;



int main()
{
    uint8_t buffer[8] = {0};
    ObjectDictionnary od;
    printf("sizeof(ObjectDictionnary) = %ld bytes\n", sizeof(od));

    Object *object = od.findObject(0x1800);
    object->writeBytes(0, buffer, 1);

    return 0;
}
