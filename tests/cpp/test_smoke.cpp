#include "doctest.h"
#include "full-harness.hpp"
#include "node.hpp"

using namespace CANopen;

TEST_CASE("init sends a boot-up message and enters pre-operational") {
    FakeHardware &hardware = initNodeOnce();
    REQUIRE_FALSE(hardware.sent.empty());
    const Frame &bootUp = hardware.sent.front();
    CHECK(bootUp.getCobID() == 0x700 + OD_NODE_ID);
    CHECK(bootUp.dlc == 1);
    CHECK(bootUp.data[0] == NMTState_Initialisation);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("heartbeat is produced once 0x1017 is written") {
    FakeHardware &hardware = initNodeOnce();
    Data period;
    period.u64 = 0;
    period.u16 = 100;
    REQUIRE(node.od().writeData(period, 0x1017, 0) == 0);
    node.update();

    hardware.clearSent();
    hardware.advance(50 * 1000);
    node.update();
    CHECK(hardware.sent.empty());

    hardware.advance(60 * 1000);
    node.update();
    REQUIRE(hardware.sent.size() == 1);
    CHECK(hardware.sent.front().getCobID() == 0x700 + OD_NODE_ID);
    CHECK(hardware.sent.front().data[0] == NMTState_PreOperational);

    period.u16 = 0;
    REQUIRE(node.od().writeData(period, 0x1017, 0) == 0);
}

TEST_CASE("NMT start command switches to operational") {
    initNodeOnce();
    node.nmt().setTransition(NMTServiceCommand_Start);
    CHECK(node.nmt().getState() == NMTState_Operational);
    node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("update drains the transport before running the services") {
    FakeHardware &hardware = initNodeOnce();
    Frame start(0, FunctionCode_NMT);
    start.dlc = 2;
    start.data[0] = NMTServiceCommand_Start;
    start.data[1] = OD_NODE_ID;
    hardware.incoming.push_back(start);
    node.update();
    CHECK(hardware.incoming.empty());
    CHECK(node.nmt().getState() == NMTState_Operational);
    node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("reset node resets the remote side and runs the application hook") {
    FakeHardware &hardware = initNodeOnce();
    static bool hookCalled = false;
    node.nmt().onReset = []() { hookCalled = true; };
    const unsigned before = hardware.remoteResets;
    node.nmt().setTransition(NMTServiceCommand_ResetNode);
    CHECK(hardware.remoteResets == before + 1);
    CHECK(hookCalled);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
    node.nmt().onReset = nullptr;
}
