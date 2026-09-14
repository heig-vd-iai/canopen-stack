#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "fake-hardware.hpp"

namespace CANopen {
FakeHardware fakeHardware;
FullNode canopen(fakeHardware, fakeHardware, fakeHardware);
Node &node = canopen.node;
}  // namespace CANopen
