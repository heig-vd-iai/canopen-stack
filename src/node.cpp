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
      _transport(transport),
      _persistence(persistence),
      _remote(remote),
      _nmt(_od, remote, *this, OD_NODE_ID),
      _services() {
    bindHardware(transport, persistence, remote);
}

bool Node::attach(Service &service) {
    if (_serviceCount >= CANOPEN_MAX_SERVICES) return false;
    _services[_serviceCount++] = &service;
    return true;
}

ObjectDictionnary &Node::od() { return _od; }

NMT &Node::nmt() { return _nmt; }

ODAccessor &Node::odAccessor() { return _odAccessor; }

CanTransport &Node::transport() { return _transport; }

Persistence &Node::persistence() { return _persistence; }

RemoteObjects &Node::remote() { return _remote; }

void Node::init() {
    _transport.init();
    _persistence.init();
    _remote.init();
    for (uint8_t i = 0; i < _serviceCount; i++) _services[i]->init();
    _nmt.initSM();
}

void Node::receiveFrame(Frame frame) {
    const FunctionCodes functionCode =
        static_cast<FunctionCodes>(frame.functionCode);
    const uint32_t now_us = _transport.getTime_us();
    if (functionCode == FunctionCode_NMT) {
        _nmt.receiveFrame(static_cast<NMTFrame &>(frame));
        return;
    }
    for (uint8_t i = 0; i < _serviceCount; i++) {
        if (_services[i]->consumes(functionCode))
            _services[i]->onFrame(frame, now_us);
    }
}

void Node::update() {
    Frame frame;
    while (_transport.receiveFrame(frame)) receiveFrame(frame);
    _remote.updateError();
    timestamp_us = _transport.getTime_us();
    for (uint8_t i = 0; i < _serviceCount; i++)
        _services[i]->update(timestamp_us);
}

uint32_t Node::getTime_us() { return timestamp_us; }

void Node::publishNmtState(NMTStates state) {
    for (uint8_t i = 0; i < _serviceCount; i++) _services[i]->onNmtState(state);
}
