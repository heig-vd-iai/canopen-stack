#include "node.hpp"
#include "frame.hpp"
using namespace CANopen;

Node::Node(uint8_t id) : nmt(*this), hb(*this), sdo(*this), pdo(*this), sync(*this), emcy(*this), nodeId(id) {}

void Node::receiveFrame(Frame frame)
{
    uint32_t timestamp = getTime_us();
    switch (frame.cobId.bits.functionCode)
    {
    case FunctionCode_NMT:
        nmt.receiveFrame(frame);
        break;
    case FunctionCode_SYNC: // Also FunctionCode_EMCY
        sync.receiveFrame(frame, timestamp);
        break;
    case FunctionCode_TIME:
        break;
    case FunctionCode_TPDO1:
    case FunctionCode_TPDO2:
    case FunctionCode_TPDO3:
    case FunctionCode_TPDO4:
        pdo.receiveFrame(frame, timestamp);
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
    default:
        break;
    }
}

void Node::update()
{
    uint32_t timestamp = getTime_us();
    nmt.update();
    hb.update(timestamp);
    sdo.update(timestamp);
    pdo.update(timestamp);
}

void Node::transmitPDO(unsigned index)
{
    pdo.transmitTPDO(index);
}

void Node::reloadTPDO()
{
    pdo.reload();
}

void Node::saveOD()
{
    od.saveData();
}

void Node::loadOD()
{
    od.loadData();
    pdo.reload();
}

Object *Node::findObject(uint16_t index)
{
    return od.findObject(index);
}

void Node::setNmtTransition(NMTServiceCommands command)
{
    nmt.setTransition(command);
}

NMTStates Node::getNmtState()
{
    return nmt.getState();
}
