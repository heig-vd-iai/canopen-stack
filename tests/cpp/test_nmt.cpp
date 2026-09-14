#include "doctest.h"
#include "full-harness.hpp"
#include "nmt/frames.hpp"
#include "od.hpp"

using namespace CANopen;

namespace {

/** Leaves the shared node in Pre-Operational, as the other tests expect. */
struct NmtBench {
    FakeHardware &hardware;
    size_t baseline;

    NmtBench() : hardware(initNodeOnce()) {
        hardware.signatureStored = false;
        toPreOperational();
        baseline = hardware.sent.size();
    }

    ~NmtBench() { toPreOperational(); }

    void toPreOperational() {
        if (node.nmt().getState() != NMTState_PreOperational)
            node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    }

    void inject(Frame frame) { node.receiveFrame(frame); }

    void command(NMTServiceCommands command, uint8_t target = OD_NODE_ID) {
        inject(nmt::makeCommand(command, target));
    }

    size_t bootUps() const {
        size_t count = 0;
        for (size_t i = baseline; i < hardware.sent.size(); i++) {
            const Frame &frame = hardware.sent[i];
            if (frame.getCobID() == 0x700 + OD_NODE_ID && frame.dlc == 1 &&
                frame.data[0] == NMTState_Initialisation)
                count++;
        }
        return count;
    }
};

}  // namespace

TEST_CASE("NMT commands follow the CiA 301 state diagram") {
    NmtBench b;
    b.command(NMTServiceCommand_Start);
    CHECK(node.nmt().getState() == NMTState_Operational);
    b.command(NMTServiceCommand_Start);
    CHECK(node.nmt().getState() == NMTState_Operational);
    b.command(NMTServiceCommand_EnterPreOperational);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
    b.command(NMTServiceCommand_Stop);
    CHECK(node.nmt().getState() == NMTState_Stopped);
    b.command(NMTServiceCommand_Stop);
    CHECK(node.nmt().getState() == NMTState_Stopped);
    b.command(NMTServiceCommand_Start);
    CHECK(node.nmt().getState() == NMTState_Operational);
    b.command(NMTServiceCommand_Stop);
    CHECK(node.nmt().getState() == NMTState_Stopped);
    b.command(NMTServiceCommand_EnterPreOperational);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("unknown commands are ignored") {
    NmtBench b;
    b.command(static_cast<NMTServiceCommands>(0x03));
    b.command(static_cast<NMTServiceCommands>(0x7F));
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("broadcast commands are honoured, other targets are ignored") {
    NmtBench b;
    b.command(NMTServiceCommand_Start, OD_NODE_ID + 1);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
    b.command(NMTServiceCommand_Start, nmt::BROADCAST);
    CHECK(node.nmt().getState() == NMTState_Operational);
}

TEST_CASE("a command must come from COB-ID 0 with two data bytes") {
    NmtBench b;
    Frame fromAnotherNode =
        nmt::makeCommand(NMTServiceCommand_Start, OD_NODE_ID);
    fromAnotherNode.nodeId = 5;
    b.inject(fromAnotherNode);
    CHECK(node.nmt().getState() == NMTState_PreOperational);

    Frame tooShort = nmt::makeCommand(NMTServiceCommand_Start, OD_NODE_ID);
    tooShort.dlc = 1;
    b.inject(tooShort);
    CHECK(node.nmt().getState() == NMTState_PreOperational);

    Frame request = nmt::makeCommand(NMTServiceCommand_Start, OD_NODE_ID);
    request.rtr = true;
    b.inject(request);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("reset communication reloads the communication group and boots") {
    NmtBench b;
    b.hardware.signature = od_signature;
    b.hardware.signatureStored = true;
    b.hardware.loadedGroups.clear();
    const unsigned remoteResets = b.hardware.remoteResets;
    b.command(NMTServiceCommand_Start);

    b.command(NMTServiceCommand_ResetCommunication);
    CHECK(node.nmt().getState() == NMTState_PreOperational);
    CHECK(b.hardware.loadedGroups ==
          std::vector<uint8_t>({ParameterGroup_Communication}));
    CHECK(b.hardware.remoteResets == remoteResets);
    CHECK(b.bootUps() == 1);
}

TEST_CASE("reset node reloads every group, resets the remote side and boots") {
    NmtBench b;
    b.hardware.signature = od_signature;
    b.hardware.signatureStored = true;
    b.hardware.loadedGroups.clear();
    const unsigned remoteResets = b.hardware.remoteResets;
    static bool hookCalled = false;
    hookCalled = false;
    node.nmt().onReset = []() { hookCalled = true; };
    b.command(NMTServiceCommand_Stop);

    b.command(NMTServiceCommand_ResetNode);
    node.nmt().onReset = nullptr;
    CHECK(node.nmt().getState() == NMTState_PreOperational);
    CHECK(b.hardware.loadedGroups ==
          std::vector<uint8_t>({ParameterGroup_Communication,
                                ParameterGroup_Application,
                                ParameterGroup_ManufacturerA}));
    CHECK(b.hardware.remoteResets == remoteResets + 1);
    CHECK(hookCalled);
    CHECK(b.bootUps() == 1);
}

TEST_CASE("a stale dictionary image is replaced by the defaults") {
    NmtBench b;
    b.hardware.signature = od_signature + 1;
    b.hardware.signatureStored = true;
    b.hardware.loadedGroups.clear();
    Data period;
    period.u64 = 0;
    period.u16 = 250;
    SDOAbortCodes code = SDOAbortCode_OK;
    REQUIRE(node.od().writeData(period, 0x1017, 0, code) == 0);

    b.command(NMTServiceCommand_ResetCommunication);
    CHECK(b.hardware.loadedGroups.empty());
    REQUIRE(node.od().readData(period, 0x1017, 0, code) == 0);
    CHECK(period.u16 == 1000);
    period.u16 = 0;
    REQUIRE(node.od().writeData(period, 0x1017, 0, code) == 0);
}

TEST_CASE("every state change reaches the attached services") {
    NmtBench b;
    const unsigned enabled = b.hardware.pdoEnabled;
    const unsigned disabled = b.hardware.pdoDisabled;
    b.command(NMTServiceCommand_Start);
    CHECK(b.hardware.pdoEnabled == enabled + 1);
    b.command(NMTServiceCommand_Stop);
    CHECK(b.hardware.pdoDisabled == disabled + 1);
    b.command(NMTServiceCommand_Start);
    b.command(NMTServiceCommand_Start);
    CHECK(b.hardware.pdoEnabled == enabled + 2);
}
