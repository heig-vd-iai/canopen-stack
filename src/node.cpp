#include "node.hpp"

CANopen_Node::CANopen_Node(uint8_t id) : nodeId(id), nmt(*this), hb(*this), sdo(*this), pdo(*this)
{
}

void CANopen_Node::receiveFrame(CANopen_Frame frame)
{
    uint32_t timestamp = getTime_us();
    switch (frame.functionCode)
    {
    case FunctionCode_NMT:
        nmt.receiveFrame(frame);
        break;
    case FunctionCode_SYNC: // Also FunctionCode_EMCY
        break;
    case FunctionCode_TIME:
        break;
    case FunctionCode_TPDO1:
    case FunctionCode_TPDO2:
    case FunctionCode_TPDO3:
    case FunctionCode_TPDO4:
        break;
    case FunctionCode_RPDO1:
    case FunctionCode_RPDO2:
    case FunctionCode_RPDO3:
    case FunctionCode_RPDO4:
        break;
    case FunctionCode_TSDO:
    case FunctionCode_RSDO:
        sdo.receiveFrame(frame, timestamp);
        break;
    case FunctionCode_HEARTBEAT:
        break;
    }
}

// void CANopen_Node::sendFrame(CANopen_Frame frame)
// {
// }

void CANopen_Node::update()
{
    uint32_t timestamp = getTime_us();
    nmt.update();
    hb.update(timestamp);
    sdo.update(timestamp);
    pdo.update(timestamp);
}
