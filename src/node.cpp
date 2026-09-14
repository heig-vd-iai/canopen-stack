/**
 * Contains the definitions of the Node class.
 */
#include "node.hpp"

#include "frame.hpp"
#include "od_common.hpp"

using namespace CANopen;

Node::Node(CanTransport &transport, Persistence &persistence,
           RemoteObjects &remote)
    : _odAccessor(_od),
      _sdo(_odAccessor, transport, OD_NODE_ID),
      _transport(transport),
      _persistence(persistence),
      _remote(remote) {
    bindRemote(remote);
}

ObjectDictionnary &Node::od() { return _od; }

NMT &Node::nmt() { return _nmt; }

HB &Node::hb() { return _hb; }

ODAccessor &Node::odAccessor() { return _odAccessor; }

SDO &Node::sdo() { return _sdo; }

PDO &Node::pdo() { return _pdo; }

SYNC &Node::sync() { return _sync; }

EMCY &Node::emcy() { return _emcy; }

CanTransport &Node::transport() { return _transport; }

Persistence &Node::persistence() { return _persistence; }

RemoteObjects &Node::remote() { return _remote; }

void Node::init() {
    _transport.init();
    _persistence.init();
    _remote.init();
    _pdo.init();
    _sync.init();
    _emcy.init();
    _nmt.initSM();
}

void Node::receiveFrame(Frame frame) {
    uint32_t timestamp = _transport.getTime_us();
    switch (static_cast<FunctionCodes>(frame.functionCode)) {
        case FunctionCode_NMT:
            _nmt.receiveFrame(static_cast<NMTFrame &>(frame));
            break;
        case FunctionCode_HEARTBEAT:
            _hb.receiveFrame(frame);
            break;
        case FunctionCode_SYNC:
            _sync.receiveFrame(static_cast<SYNCFrame &>(frame), timestamp);
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
            _sdo.receiveFrame(frame, timestamp);
            break;
        default:
            break;
    }
}

void Node::update() {
    Frame frame;
    while (_transport.receiveFrame(frame)) receiveFrame(frame);
    _remote.updateError();
    timestamp_us = _transport.getTime_us();
    _hb.update(timestamp_us);
    _sdo.update(timestamp_us);
    _pdo.update(timestamp_us);
}

uint32_t Node::getTime_us() { return timestamp_us; }
