#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "fake-hardware.hpp"

namespace CANopen {
FakeHardware fakeHardware;
Node node(fakeHardware, fakeHardware, fakeHardware);
}  // namespace CANopen
