#include "node.hpp"

CANopen_Node::CANopen_Node(int id) : nodeId(id), nmt(*this)
{
}

void CANopen_Node::receiveFrame(CANopen_Frame frame)
{
    switch (frame.functionCode)
    {
    case FunctionCode_NMT:
        nmt.receiveFrame(frame);
        break;
    }
}

// void CANopen_Node::sendFrame(CANopen_Frame frame)
// {
// }

void CANopen_Node::update()
{
    nmt.update();
}
