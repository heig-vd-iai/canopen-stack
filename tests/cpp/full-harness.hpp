#pragma once

#include "fake-hardware.hpp"
#include "full-node.hpp"

namespace CANopen {

extern FakeHardware fakeHardware;
extern FullNode canopen;
extern Node &node;

inline FakeHardware &initNodeOnce() {
    static bool initialised = false;
    if (!initialised) {
        initialised = true;
        canopen.init();
    }
    return fakeHardware;
}

}  // namespace CANopen
