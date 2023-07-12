#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include "CANopen.hpp"

using namespace std;

int main()
{
    OD_ObjectDictionnary od;
    od.load();

    OD_ObjectEntry *entryPtr = od.findEntry(0x6040);
    if (entryPtr == NULL)
        return EXIT_FAILURE;
    for (int i = 0; i < entryPtr->subNumber; i++)
    {
        printf("%d\n", *(int *)entryPtr->objects[i].valueSrc);
    }

    return EXIT_SUCCESS;
}
