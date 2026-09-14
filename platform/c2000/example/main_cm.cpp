#include "C2000CanTransport.hpp"
#include "C2000Persistence.hpp"
#include "canopen.hpp"
#include "hal/remote-objects.hpp"
extern "C" {
#include "cm.h"
}

namespace {
CANopen::C2000CanTransport transport(OD_NODE_ID);
CANopen::C2000Persistence persistence;
CANopen::NullRemote remote;
}  // namespace

CANopen::Node CANopen::node(transport, persistence, remote);

int main() {
    CM_init();
    CANopen::node.init();
    while (true) {
        CANopen::node.update();
    }
}
