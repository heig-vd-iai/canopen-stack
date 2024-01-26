/**
 * Contains the definitions of the Node class.
 */
#include "node.hpp"

#include "frame.hpp"

namespace CANopen {
Node node;
}  // namespace CANopen

using namespace CANopen;

Node::Node() : _od(), _nmt(), _hb(), _sdo(), _pdo(), _sync(), _emcy() {}

ObjectDictionnary &Node::od() { return _od; }

NMT &Node::nmt() { return _nmt; }

HB &Node::hb() { return _hb; }

SDO &Node::sdo() { return _sdo; }

PDO &Node::pdo() { return _pdo; }

SYNC &Node::sync() { return _sync; }

EMCY &Node::emcy() { return _emcy; }

void Node::init() { _nmt.initSM(); }

void Node::receiveFrame(Frame frame) {
    uint32_t timestamp = getTime_us();
    switch ((FunctionCodes)frame.functionCode) {
        case FunctionCode_NMT:
            _nmt.receiveFrame((NMTFrame &)frame);
            break;
        case FunctionCode_HEARTBEAT:
            _hb.receiveFrame(frame);
            break;
        case FunctionCode_SYNC:
            _sync.receiveFrame((SYNCFrame &)frame, timestamp);
            break;
        case FunctionCode_TPDO1:
        case FunctionCode_TPDO2:
        case FunctionCode_TPDO3:
        case FunctionCode_TPDO4:
            _pdo.receiveTPDO(frame, timestamp);
            break;
        case FunctionCode_RPDO1:
        case FunctionCode_RPDO2:
        case FunctionCode_RPDO3:
        case FunctionCode_RPDO4:
            _pdo.receiveRPDO(frame, timestamp);
            break;
        case FunctionCode_RSDO:
            _sdo.receiveFrame((SDOFrame &)frame, timestamp);
            break;
        default:
            break;
    }
}

void Node::update() {
    uint32_t timestamp = getTime_us();
    _hb.update(timestamp);
    _sdo.update(timestamp);
    _pdo.update(timestamp);
}
