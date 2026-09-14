#include <cstring>
#include <vector>

#include "doctest.h"
#include "full-harness.hpp"
#include "od.hpp"
#include "pdo/config.hpp"
#include "pdo/pdo.hpp"

using namespace CANopen;

namespace {

typedef std::vector<uint8_t> Bytes;

const uint16_t TPDO1_COBID = 0x180 + OD_NODE_ID;
const uint16_t RPDO1_COBID = 0x200 + OD_NODE_ID;
const uint16_t SYNC_COBID = 0x80;

const uint32_t MAP_STATUSWORD = pdo::mapEntry(0x6041, 0, 16);
const uint32_t MAP_CONTROLWORD = pdo::mapEntry(0x6040, 0, 16);
const uint32_t MAP_MODE = pdo::mapEntry(0x6060, 0, 16);
const uint32_t MAP_POSITION = pdo::mapEntry(0x6064, 0, 32);

/**
 * Every test starts and ends on a communication reset, so the PDO records
 * are back to their defaults and the shared node is left as found.
 */
struct PdoBench {
    FakeHardware &hardware;
    size_t baseline = 0;

    PdoBench() : hardware(initNodeOnce()) {
        hardware.signatureStored = false;
        node.nmt().setTransition(NMTServiceCommand_ResetCommunication);
        canopen.pdo.onReceive(nullptr);
        canopen.pdo.onTimeout(nullptr);
        baseline = hardware.sent.size();
    }

    ~PdoBench() {
        write(0x1017, 0, 0);
        node.nmt().setTransition(NMTServiceCommand_ResetCommunication);
        write(0x1017, 0, 0);
        canopen.pdo.onReceive(nullptr);
        canopen.pdo.onTimeout(nullptr);
    }

    void write(uint16_t index, uint8_t subindex, uint64_t value) {
        REQUIRE(tryWrite(index, subindex, value) == SDOAbortCode_OK);
    }

    SDOAbortCodes tryWrite(uint16_t index, uint8_t subindex, uint64_t value) {
        Data data;
        data.u64 = value;
        SDOAbortCodes code = SDOAbortCode_OK;
        node.od().writeData(data, index, subindex, code);
        return code;
    }

    Data read(uint16_t index, uint8_t subindex) {
        Data data;
        data.u64 = 0;
        SDOAbortCodes code = SDOAbortCode_OK;
        REQUIRE(node.od().readData(data, index, subindex, code) == 0);
        return data;
    }

    void mapTpdo1(std::vector<uint32_t> entries) {
        write(0x1A00, 0, 0);
        for (size_t i = 0; i < entries.size(); i++)
            write(0x1A00, static_cast<uint8_t>(i + 1), entries[i]);
        write(0x1A00, 0, entries.size());
    }

    void mapRpdo1(std::vector<uint32_t> entries) {
        write(0x1600, 0, 0);
        for (size_t i = 0; i < entries.size(); i++)
            write(0x1600, static_cast<uint8_t>(i + 1), entries[i]);
        write(0x1600, 0, entries.size());
    }

    void start() {
        node.nmt().setTransition(NMTServiceCommand_Start);
        baseline = hardware.sent.size();
    }

    void tick(uint32_t delta_us) {
        hardware.advance(delta_us);
        node.update();
    }

    void inject(uint16_t cobId, const Bytes &payload, bool rtr = false) {
        Frame frame = Frame::fromCobId(cobId);
        frame.dlc = static_cast<uint8_t>(payload.size());
        memcpy(frame.data, payload.data(), payload.size());
        frame.rtr = rtr;
        node.receiveFrame(frame);
    }

    std::vector<Bytes> sentOn(uint16_t cobId) {
        std::vector<Bytes> out;
        for (size_t i = baseline; i < hardware.sent.size(); i++) {
            const Frame &frame = hardware.sent[i];
            if (frame.getCobID() != cobId || frame.rtr) continue;
            out.push_back(Bytes(frame.data, frame.data + frame.dlc));
        }
        return out;
    }
};

}  // namespace

TEST_CASE("PDOs start invalid: nothing is sent in operational") {
    PdoBench b;
    b.start();
    b.tick(100 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).empty());
    CHECK(b.read(0x1800, 1).u32 == 0x80000181u);
    CHECK(b.read(0x1400, 1).u32 == 0x80000201u);
}

TEST_CASE("event timer TPDO carries the mapped local objects") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD, MAP_MODE});
    b.write(0x1800, 2, pdo::TRANSMISSION_EVENT_MANUFACTURER);
    b.write(0x1800, 5, 10);
    b.write(0x1800, 1, TPDO1_COBID);
    b.write(0x6041, 0, 0x1234);
    b.write(0x6060, 0, 0xFFFE);
    b.start();

    b.tick(5 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).empty());
    b.tick(5 * 1000);
    std::vector<Bytes> frames = b.sentOn(TPDO1_COBID);
    REQUIRE(frames.size() == 1);
    CHECK(frames[0] == Bytes({0x34, 0x12, 0xFE, 0xFF}));
    b.tick(10 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).size() == 2);
}

TEST_CASE("remote request triggers an RTR-only TPDO") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_RTR_EVENT);
    b.write(0x1800, 1, TPDO1_COBID);
    b.write(0x6041, 0, 0xABCD);
    b.start();

    b.tick(50 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).empty());
    b.inject(TPDO1_COBID, {}, true);
    std::vector<Bytes> frames = b.sentOn(TPDO1_COBID);
    REQUIRE(frames.size() == 1);
    CHECK(frames[0] == Bytes({0xCD, 0xAB}));
}

TEST_CASE("application events respect the inhibit time") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1800, 3, 5);
    b.write(0x1800, 1, TPDO1_COBID);
    b.start();
    b.tick(1000);

    canopen.pdo.transmitTPDO(0);
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
    b.tick(100);
    canopen.pdo.transmitTPDO(0);
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
    b.tick(500);
    canopen.pdo.transmitTPDO(0);
    CHECK(b.sentOn(TPDO1_COBID).size() == 2);
}

TEST_CASE("inhibit time is only writeable while the PDO is invalid") {
    PdoBench b;
    b.write(0x1800, 1, TPDO1_COBID);
    CHECK(b.tryWrite(0x1800, 3, 5) == SDOAbortCode_UnsupportedObjectAccess);
    b.write(0x1800, 1, 0x80000000u | TPDO1_COBID);
    CHECK(b.tryWrite(0x1800, 3, 5) == SDOAbortCode_OK);
    CHECK(b.read(0x1800, 3).u16 == 5);
}

TEST_CASE("cyclic synchronous TPDO is sent every third SYNC") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, 3);
    b.write(0x1800, 1, TPDO1_COBID);
    b.start();

    for (int i = 0; i < 2; i++) b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).empty());
    b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
    for (int i = 0; i < 3; i++) b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).size() == 2);
}

TEST_CASE("acyclic synchronous TPDO waits for an event then the next SYNC") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_SYNC_ACYCLIC);
    b.write(0x1800, 1, TPDO1_COBID);
    b.start();

    b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).empty());
    canopen.pdo.transmitTPDO(0);
    CHECK(b.sentOn(TPDO1_COBID).empty());
    b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
    b.inject(SYNC_COBID, {});
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
}

TEST_CASE("event RPDO writes the mapped local objects and calls back") {
    PdoBench b;
    b.mapRpdo1({MAP_CONTROLWORD, MAP_MODE});
    b.write(0x1400, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1400, 1, RPDO1_COBID);
    static unsigned received = 0;
    received = 0;
    canopen.pdo.onReceive([](unsigned number) { received = number; });
    b.start();

    b.inject(RPDO1_COBID, {0x0F, 0x00, 0x01, 0x00});
    CHECK(b.read(0x6040, 0).u16 == 0x000F);
    CHECK(b.read(0x6060, 0).i16 == 1);
    CHECK(received == 1);
}

TEST_CASE("RPDO shorter than its mapping is ignored") {
    PdoBench b;
    b.mapRpdo1({MAP_CONTROLWORD, MAP_MODE});
    b.write(0x1400, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1400, 1, RPDO1_COBID);
    b.write(0x6040, 0, 0x1111);
    b.start();

    b.inject(RPDO1_COBID, {0x22, 0x22});
    CHECK(b.read(0x6040, 0).u16 == 0x1111);
}

TEST_CASE("synchronous RPDO is applied on the next SYNC") {
    PdoBench b;
    b.mapRpdo1({MAP_CONTROLWORD});
    b.write(0x1400, 2, 1);
    b.write(0x1400, 1, RPDO1_COBID);
    b.write(0x6040, 0, 0);
    b.start();

    b.inject(RPDO1_COBID, {0x34, 0x12});
    CHECK(b.read(0x6040, 0).u16 == 0);
    b.inject(SYNC_COBID, {});
    CHECK(b.read(0x6040, 0).u16 == 0x1234);
}

TEST_CASE("PDOs are ignored outside operational") {
    PdoBench b;
    b.mapRpdo1({MAP_CONTROLWORD});
    b.write(0x1400, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1400, 1, RPDO1_COBID);
    b.write(0x6040, 0, 0);
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1800, 5, 1);
    b.write(0x1800, 1, TPDO1_COBID);

    b.inject(RPDO1_COBID, {0x34, 0x12});
    CHECK(b.read(0x6040, 0).u16 == 0);
    b.tick(10 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).empty());

    b.start();
    b.inject(RPDO1_COBID, {0x34, 0x12});
    CHECK(b.read(0x6040, 0).u16 == 0x1234);
    node.nmt().setTransition(NMTServiceCommand_Stop);
    const size_t before = b.sentOn(TPDO1_COBID).size();
    b.tick(10 * 1000);
    CHECK(b.sentOn(TPDO1_COBID).size() == before);
}

TEST_CASE("invalidating a PDO stops it") {
    PdoBench b;
    b.mapTpdo1({MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1800, 5, 1);
    b.write(0x1800, 1, TPDO1_COBID);
    b.start();

    b.tick(2000);
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
    b.write(0x1800, 1, 0x80000000u | TPDO1_COBID);
    b.tick(2000);
    CHECK(b.sentOn(TPDO1_COBID).size() == 1);
}

TEST_CASE("COB-ID bits are frozen while the PDO is valid") {
    PdoBench b;
    b.write(0x1800, 1, TPDO1_COBID);
    CHECK(b.tryWrite(0x1800, 1, TPDO1_COBID + 1) ==
          SDOAbortCode_InvalidDownloadParameterValue);
    CHECK(b.read(0x1800, 1).u32 == TPDO1_COBID);
    b.write(0x1800, 1, 0x80000000u | (TPDO1_COBID + 1));
    b.write(0x1800, 1, TPDO1_COBID + 1);
    CHECK(b.read(0x1800, 1).u32 == TPDO1_COBID + 1);
}

TEST_CASE(
    "mapping rejects unknown, non-mappable, wrong-way and oversized entries") {
    PdoBench b;
    b.write(0x1A00, 0, 0);
    CHECK(b.tryWrite(0x1A00, 1, pdo::mapEntry(0x1000, 0, 32)) ==
          SDOAbortCode_CannotMapToPDO);
    CHECK(b.tryWrite(0x1A00, 1, pdo::mapEntry(0x2FFF, 0, 8)) ==
          SDOAbortCode_ObjectNonExistent);
    b.write(0x1600, 0, 0);
    CHECK(b.tryWrite(0x1600, 1, MAP_STATUSWORD) == SDOAbortCode_CannotMapToPDO);
    for (uint8_t i = 1; i <= 5; i++) b.write(0x1A00, i, MAP_STATUSWORD);
    CHECK(b.tryWrite(0x1A00, 0, 5) == SDOAbortCode_MappedPDOLengthExceeded);
    CHECK(b.tryWrite(0x1A00, 0, 4) == SDOAbortCode_OK);
    CHECK(b.tryWrite(0x1A00, 0, 9) == SDOAbortCode_DownloadValueTooHigh);
}

TEST_CASE(
    "remote objects are fetched from the remote side and local ones read") {
    PdoBench b;
    b.mapTpdo1({MAP_POSITION, MAP_STATUSWORD});
    b.write(0x1800, 2, pdo::TRANSMISSION_EVENT_PROFILE);
    b.write(0x1800, 1, TPDO1_COBID);
    Data position;
    position.u64 = 0;
    position.i32 = 0x11223344;
    b.hardware.remote[OD_OBJECT_6064_SUB0] = position;
    b.write(0x6041, 0, 0x5566);
    b.start();

    REQUIRE(b.hardware.remoteTpdo.count(0) == 1);
    CHECK(b.hardware.remoteTpdo[0][0] == OD_OBJECT_6064_SUB0);
    CHECK(b.hardware.remoteTpdo[0][1] == OD_OBJECT_6041_SUB0);
    CHECK(b.hardware.remoteTpdo[0][2] == -1);

    b.tick(1000);
    canopen.pdo.transmitTPDO(0);
    std::vector<Bytes> frames = b.sentOn(TPDO1_COBID);
    REQUIRE(frames.size() == 1);
    CHECK(frames[0] == Bytes({0x44, 0x33, 0x22, 0x11, 0x66, 0x55}));
}
