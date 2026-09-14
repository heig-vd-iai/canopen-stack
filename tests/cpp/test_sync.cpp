#include <vector>

#include "doctest.h"
#include "full-harness.hpp"
#include "od.hpp"
#include "sync/config.hpp"
#include "sync/frames.hpp"

using namespace CANopen;

namespace {

/** Records the counters the application callback receives. */
struct SyncBench {
    FakeHardware &hardware;
    static std::vector<unsigned> counters;

    SyncBench() : hardware(initNodeOnce()) {
        toPreOperational();
        counters.clear();
        canopen.sync.onSync(
            [](unsigned counter) { counters.push_back(counter); });
    }

    ~SyncBench() {
        canopen.sync.onSync(nullptr);
        toPreOperational();
    }

    void toPreOperational() {
        if (node.nmt().getState() != NMTState_PreOperational)
            node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    }

    void sync() { node.receiveFrame(sync::makeSync()); }
    void sync(uint8_t counter) {
        node.receiveFrame(sync::makeSync(counter, sync::DEFAULT_COBID));
    }
};

std::vector<unsigned> SyncBench::counters;

}  // namespace

TEST_CASE("the SYNC consumer listens on the default COB-ID") {
    SyncBench b;
    CHECK(canopen.sync.cobId() == sync::DEFAULT_COBID);
    CHECK(canopen.sync.counterOverflow() == sync::COUNTER_NONE);
}

TEST_CASE("a SYNC without counter counts locally, one with counter adopts it") {
    SyncBench b;
    b.sync(10);
    b.sync();
    b.sync();
    b.sync(3);
    b.sync();
    CHECK(SyncBench::counters == std::vector<unsigned>({10, 11, 12, 3, 4}));
}

TEST_CASE("the local counter wraps at 240 without an overflow object") {
    SyncBench b;
    b.sync(239);
    b.sync();
    b.sync();
    CHECK(SyncBench::counters == std::vector<unsigned>({239, 240, 1}));
}

TEST_CASE("SYNC is ignored while stopped, from another COB-ID or malformed") {
    SyncBench b;
    node.nmt().setTransition(NMTServiceCommand_Stop);
    b.sync(1);
    CHECK(SyncBench::counters.empty());
    node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);

    node.receiveFrame(sync::makeSync(5, 0x81));
    Frame tooLong = sync::makeSync();
    tooLong.dlc = 2;
    node.receiveFrame(tooLong);
    Frame request = sync::makeSync();
    request.rtr = true;
    node.receiveFrame(request);
    CHECK(SyncBench::counters.empty());

    b.sync(1);
    CHECK(SyncBench::counters == std::vector<unsigned>({1}));
}

TEST_CASE("SYNC reaches the listener in operational") {
    SyncBench b;
    const size_t before = b.hardware.sent.size();
    node.nmt().setTransition(NMTServiceCommand_Start);
    b.sync();
    CHECK(SyncBench::counters.size() == 1);
    CHECK(b.hardware.sent.size() == before);
}
