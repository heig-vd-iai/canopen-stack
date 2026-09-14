#include "doctest.h"
#include "full-harness.hpp"
#include "node.hpp"
#include "od.hpp"

using namespace CANopen;

namespace {

Data readById(int32_t id) {
    Data data;
    data.u64 = 0;
    SDOAbortCodes abortCode = SDOAbortCode_OK;
    REQUIRE(node.od().readData(data, id, abortCode) == 0);
    REQUIRE(abortCode == SDOAbortCode_OK);
    return data;
}

}  // namespace

TEST_CASE("generated tables have one entry per subindex") {
    CHECK(sizeof(CANopenOD::objectIndexTable) /
              sizeof(CANopenOD::objectIndexTable[0]) ==
          OD_LENGTH);
    CHECK(node.od().length == OD_LENGTH);
    CHECK(OD_NODE_ID == node.nodeId);
    CHECK(OD_TPDO_COUNT == 1);
    CHECK(OD_RPDO_COUNT == 1);
}

TEST_CASE("findObject resolves every generated key and rejects unknown ones") {
    const int32_t length = OD_LENGTH;
    for (int32_t id = 0; id < length; id++) {
        const uint16_t index = CANopenOD::objectIndexTable[id].first;
        const uint8_t subindex = CANopenOD::objectIndexTable[id].second;
        CHECK(node.od().findObject(index, subindex) == id);
    }
    CHECK(node.od().findObject(0x1000, 0) == OD_OBJECT_1000_SUB0);
    CHECK(node.od().findObject(0x6064, 0) == OD_OBJECT_6064_SUB0);
    CHECK(node.od().findObject(0xFFFF, 0) < 0);
    CHECK(node.od().findObject(0x0000, 0) < 0);
    CHECK(node.od().findObject(0x1000, 9) < 0);
    CHECK_FALSE(node.od().isSubValid(0x2FFF, 0));
}

TEST_CASE("device type is derived from the logical device profile") {
    CHECK(readById(OD_OBJECT_1000_SUB0).u32 == 0x00000192u);
}

TEST_CASE("default values land in the right typed table") {
    CHECK(readById(OD_OBJECT_2000_SUB0).u8 == 10);
    CHECK(readById(OD_OBJECT_2000_SUB1).b == true);
    CHECK(readById(OD_OBJECT_2000_SUB2).i8 == -8);
    CHECK(readById(OD_OBJECT_2000_SUB3).i16 == -16);
    CHECK(readById(OD_OBJECT_2000_SUB4).i32 == -32);
    CHECK(readById(OD_OBJECT_2000_SUB5).i64 == -64);
    CHECK(readById(OD_OBJECT_2000_SUB6).u16 == 16);
    CHECK(readById(OD_OBJECT_2000_SUB7).u32 == 32);
    CHECK(readById(OD_OBJECT_2000_SUB8).u64 == 64);
    CHECK(readById(OD_OBJECT_2000_SUB9).f32 == doctest::Approx(1.5f));
    CHECK(readById(OD_OBJECT_2000_SUB10).f64 == doctest::Approx(2.5));
    CHECK(readById(OD_OBJECT_2004_SUB1).u32 == 1);
    CHECK(readById(OD_OBJECT_2004_SUB2).u32 == 2);
    CHECK(readById(OD_OBJECT_2004_SUB3).u32 == 3);
    CHECK(readById(OD_OBJECT_1018_SUB1).u32 == 0);
    CHECK(readById(OD_OBJECT_1018_SUB2).u32 == 1);
}

TEST_CASE("metadata carries access rights, type and limits") {
    Metadata *limited = node.od().getMetadata(0x2003, 0);
    REQUIRE(limited != nullptr);
    CHECK(bool(limited->access.bits.readable));
    CHECK(bool(limited->access.bits.writeable));
    CHECK(bool(limited->access.bits.limited));
    CHECK_FALSE(bool(limited->access.bits.remote));
    CHECK(limited->dataType == DataType::UNSIGNED16);
    CHECK(limited->getDefaultValue().u16 == 50);

    Metadata *remote = node.od().getMetadata(0x2005, 0);
    REQUIRE(remote != nullptr);
    CHECK(bool(remote->access.bits.remote));

    Metadata *mappable = node.od().getMetadata(0x6040, 0);
    REQUIRE(mappable != nullptr);
    CHECK(bool(mappable->access.bits.mappable));

    CHECK(node.od().getMetadata(0x2FFF, 0) == nullptr);
    CHECK(node.od().getMetadata(-1) == nullptr);
    CHECK(node.od().getMetadata(OD_LENGTH) == nullptr);
    CHECK(node.od().getSize(-1) == 0);
    CHECK(node.od().getSize(0x2000, 8) == sizeof(uint64_t));
    CHECK(node.od().getSize(0x2002, 0) == DOMAIN_MAX_SIZE);
}

TEST_CASE("limited setter rejects values outside its range") {
    Data data;
    data.u64 = 0;
    SDOAbortCodes abortCode = SDOAbortCode_OK;

    data.u16 = 5;
    CHECK(node.od().writeData(data, 0x2003, 0, abortCode) == -1);
    CHECK(abortCode == SDOAbortCode_DownloadValueTooLow);

    data.u16 = 200;
    CHECK(node.od().writeData(data, 0x2003, 0, abortCode) == -1);
    CHECK(abortCode == SDOAbortCode_DownloadValueTooHigh);

    data.u16 = 60;
    CHECK(node.od().writeData(data, 0x2003, 0, abortCode) == 0);
    CHECK(abortCode == SDOAbortCode_OK);
    CHECK(readById(OD_OBJECT_2003_SUB0).u16 == 60);
    data.u16 = 50;
    CHECK(node.od().writeData(data, 0x2003, 0, abortCode) == 0);
}

TEST_CASE("local write then read round-trips through the typed tables") {
    Data data;
    data.u64 = 0;
    data.i32 = -1234;
    CHECK(node.od().writeData(data, 0x2000, 4) == 0);
    CHECK(readById(OD_OBJECT_2000_SUB4).i32 == -1234);
    data.i32 = -32;
    CHECK(node.od().writeData(data, 0x2000, 4) == 0);
}

TEST_CASE("remote objects are delegated to the hardware interface") {
    FakeHardware &hardware = initNodeOnce();
    Data stored;
    stored.u64 = 0;
    stored.u32 = 0xCAFE;
    hardware.remote[OD_OBJECT_2005_SUB0] = stored;

    Data data;
    data.u64 = 0;
    SDOAbortCodes abortCode = SDOAbortCode_OK;
    CHECK(node.od().readData(data, 0x2005, 0, abortCode) == 0);
    CHECK(data.u32 == 0xCAFE);

    hardware.remotePendingTurns = 2;
    CHECK(node.od().readData(data, 0x2005, 0, abortCode) == 1);
    CHECK(node.od().readData(data, 0x2005, 0, abortCode) == 1);
    CHECK(node.od().readData(data, 0x2005, 0, abortCode) == 0);

    data.u32 = 42;
    CHECK(node.od().writeData(data, 0x2005, 0, abortCode) == 0);
    CHECK(hardware.remote[OD_OBJECT_2005_SUB0].u32 == 42);
}
