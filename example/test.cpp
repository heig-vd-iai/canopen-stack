#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include "od.hpp"

using namespace std;

enum FunctionCodes
{
    FunctionCode_NMT = 0b0000,
    FunctionCode_SYNC = 0b0001,
    FunctionCode_EMCY = 0b0001,
    FunctionCode_TIME = 0b0010,
    FunctionCode_HEARTBEAT = 0b1110
};

struct CANopen_Frame
{
    uint8_t data[8];
    uint8_t dlc;
    uint8_t functionCode;
    uint8_t nodeId;
    uint8_t rtr;
};

class CANopen
{
private:
public:
    inline void receiveFrame(CANopen_Frame frame)
    {
        switch (frame.functionCode)
        {
        case FunctionCode_NMT:
            break;
        default:
            break;
        }
    }
};

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
