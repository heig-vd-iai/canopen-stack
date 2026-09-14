#include <vector>

#include "doctest.h"
#include "full-harness.hpp"
#include "hb/frames.hpp"
#include "od.hpp"

using namespace CANopen;

namespace {

const uint16_t HB_COBID = 0x700 + OD_NODE_ID;

struct Heartbeat {
    NMTStates state;
    bool toggle;
};

/** Starts silent in Pre-Operational and leaves the node that way. */
struct HbBench {
    FakeHardware &hardware;
    size_t baseline;

    HbBench() : hardware(initNodeOnce()) {
        toPreOperational();
        setPeriod(0);
        baseline = hardware.sent.size();
    }

    ~HbBench() {
        toPreOperational();
        setPeriod(0);
    }

    void toPreOperational() {
        if (node.nmt().getState() != NMTState_PreOperational)
            node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    }

    void setPeriod(uint16_t period_ms) {
        Data data;
        data.u64 = 0;
        data.u16 = period_ms;
        SDOAbortCodes code = SDOAbortCode_OK;
        REQUIRE(node.od().writeData(data, 0x1017, 0, code) == 0);
    }

    uint16_t periodFromOd() {
        Data data;
        data.u64 = 0;
        SDOAbortCodes code = SDOAbortCode_OK;
        REQUIRE(node.od().readData(data, 0x1017, 0, code) == 0);
        return data.u16;
    }

    void tick(uint32_t delta_us) {
        hardware.advance(delta_us);
        node.update();
    }

    std::vector<Heartbeat> heartbeats() {
        std::vector<Heartbeat> out;
        for (size_t i = baseline; i < hardware.sent.size(); i++) {
            const Frame &frame = hardware.sent[i];
            if (frame.getCobID() != HB_COBID) continue;
            const hb::Message message(frame);
            REQUIRE(message.isWellFormed());
            out.push_back({message.state(), message.toggle()});
        }
        return out;
    }
};

}  // namespace

TEST_CASE("leaving initialisation sends the boot-up message") {
    HbBench b;
    b.hardware.signatureStored = false;
    node.nmt().setTransition(NMTServiceCommand_ResetCommunication);
    b.setPeriod(0);
    std::vector<Heartbeat> sent = b.heartbeats();
    REQUIRE(sent.size() == 1);
    CHECK(sent[0].state == NMTState_Initialisation);
    CHECK_FALSE(sent[0].toggle);
}

TEST_CASE("the heartbeat carries the NMT state at the producer time") {
    HbBench b;
    b.setPeriod(20);
    CHECK(b.periodFromOd() == 20);
    CHECK(canopen.hb.producerTime_ms() == 20);

    b.tick(19 * 1000);
    CHECK(b.heartbeats().empty());
    b.tick(1 * 1000);
    std::vector<Heartbeat> sent = b.heartbeats();
    REQUIRE(sent.size() == 1);
    CHECK(sent[0].state == NMTState_PreOperational);
    CHECK_FALSE(sent[0].toggle);

    node.nmt().setTransition(NMTServiceCommand_Start);
    b.tick(20 * 1000);
    node.nmt().setTransition(NMTServiceCommand_Stop);
    b.tick(20 * 1000);
    sent = b.heartbeats();
    REQUIRE(sent.size() == 3);
    CHECK(sent[1].state == NMTState_Operational);
    CHECK(sent[2].state == NMTState_Stopped);
}

TEST_CASE("writing the producer time restarts the period") {
    HbBench b;
    b.setPeriod(20);
    b.tick(15 * 1000);
    b.setPeriod(20);
    b.tick(15 * 1000);
    CHECK(b.heartbeats().empty());
    b.tick(5 * 1000);
    CHECK(b.heartbeats().size() == 1);
}

TEST_CASE("a producer time of zero stops the heartbeat") {
    HbBench b;
    b.setPeriod(10);
    b.tick(10 * 1000);
    CHECK(b.heartbeats().size() == 1);
    b.setPeriod(0);
    b.tick(100 * 1000);
    CHECK(b.heartbeats().size() == 1);
}

TEST_CASE("node guarding replies with the state and a toggling bit") {
    HbBench b;
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID));
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID));
    node.nmt().setTransition(NMTServiceCommand_Start);
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID));
    std::vector<Heartbeat> sent = b.heartbeats();
    REQUIRE(sent.size() == 3);
    CHECK(sent[0].state == NMTState_PreOperational);
    CHECK_FALSE(sent[0].toggle);
    CHECK(sent[1].state == NMTState_PreOperational);
    CHECK(sent[1].toggle);
    CHECK(sent[2].state == NMTState_Operational);
    CHECK_FALSE(sent[2].toggle);
}

TEST_CASE("requests for other nodes and foreign heartbeats are ignored") {
    HbBench b;
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID + 1));
    node.receiveFrame(
        hb::makeMessage(OD_NODE_ID + 1, NMTState_Operational, false));
    CHECK(b.heartbeats().empty());
}

TEST_CASE("a reset clears the toggle bit") {
    HbBench b;
    b.hardware.signatureStored = false;
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID));
    node.nmt().setTransition(NMTServiceCommand_ResetCommunication);
    b.setPeriod(0);
    node.receiveFrame(hb::makeGuardRequest(OD_NODE_ID));
    std::vector<Heartbeat> sent = b.heartbeats();
    REQUIRE(sent.size() == 3);
    CHECK(sent[1].state == NMTState_Initialisation);
    CHECK_FALSE(sent[2].toggle);
}
