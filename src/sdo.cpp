#include "sdo.hpp"
#include "node.hpp"

CANopen_SDO::CANopen_SDO(CANopen_Node &node) : node(node)
{
}

void CANopen_SDO::receiveFrame(CANopen_Frame frame)
{
    if (frame.nodeId != node.nodeId)
        return;
    switch (frame.functionCode)
    {
    case FunctionCode_TSDO:
        break;
    case FunctionCode_RSDO:
        break;
    }
}
