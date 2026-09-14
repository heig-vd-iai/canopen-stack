#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

#include "doctest.h"
#include "full-harness.hpp"
#include "node.hpp"
#include "od.hpp"
#include "od/domainHandler.hpp"
#include "sdo/config.hpp"
#include "sdo/sdoServer.hpp"
#include "utils/crc.hpp"

using namespace CANopen;

namespace {

typedef std::vector<uint8_t> Bytes;

std::string hex(const Bytes &bytes) {
    static const char digits[] = "0123456789abcdef";
    std::string out;
    for (size_t i = 0; i < bytes.size(); i++) {
        if (i > 0) out += ' ';
        out += digits[bytes[i] >> 4];
        out += digits[bytes[i] & 0x0F];
    }
    return out;
}

Bytes le(uint64_t value, size_t length) {
    Bytes out;
    for (size_t i = 0; i < length; i++)
        out.push_back(static_cast<uint8_t>(value >> (8 * i)));
    return out;
}

Bytes cat(Bytes head, const Bytes &tail) {
    head.insert(head.end(), tail.begin(), tail.end());
    return head;
}

Bytes pad(Bytes bytes) {
    bytes.resize(8, 0);
    return bytes;
}

struct MemoryDomain : public DomainHandler {
    Bytes stored;
    Bytes source;
    uint32_t announced = 0;
    uint32_t endSize = 0;
    int begun = 0;
    int ended = 0;
    int aborted = 0;

    SDOAbortCodes beginDownload(int32_t, uint32_t size) override {
        begun++;
        announced = size;
        stored.clear();
        return SDOAbortCode_OK;
    }
    SDOAbortCodes downloadChunk(int32_t, uint32_t offset, const uint8_t *bytes,
                                uint32_t length) override {
        if (offset != stored.size()) return SDOAbortCode_GeneralError;
        stored.insert(stored.end(), bytes, bytes + length);
        return SDOAbortCode_OK;
    }
    SDOAbortCodes endDownload(int32_t, uint32_t size) override {
        ended++;
        endSize = size;
        return SDOAbortCode_OK;
    }
    void abortDownload(int32_t) override { aborted++; }
    uint32_t uploadSize(int32_t) override {
        return static_cast<uint32_t>(source.size());
    }
    SDOAbortCodes uploadChunk(int32_t, uint32_t offset, uint8_t *bytes,
                              uint32_t length) override {
        memcpy(bytes, source.data() + offset, length);
        return SDOAbortCode_OK;
    }
};

struct Step {
    Bytes in;
    std::vector<Bytes> out;
    uint32_t advance_us;
};

struct Bench {
    FakeHardware bus;
    ODAccessor accessor;
    SDO sdo;
    MemoryDomain domain;
    uint32_t now = 0;

    Bench() : accessor(node.od()), sdo(accessor, bus, OD_NODE_ID) {
        initNodeOnce();
        accessor.setDomainHandler(&domain);
        sdo.enable();
    }

    void send(const Bytes &bytes) {
        Frame frame(OD_NODE_ID, FunctionCode_RSDO);
        frame.dlc = 8;
        memcpy(frame.data, pad(bytes).data(), 8);
        sdo.receiveFrame(frame, now);
    }

    void tick(uint32_t delta_us = 0) {
        now += delta_us;
        sdo.update(now);
    }

    std::vector<Bytes> drain() {
        std::vector<Bytes> out;
        for (const Frame &frame : bus.sent) {
            CHECK(frame.getCobID() == 0x580 + OD_NODE_ID);
            CHECK(frame.dlc == 8);
            out.push_back(Bytes(frame.data, frame.data + 8));
        }
        bus.sent.clear();
        return out;
    }

    Bytes single() {
        std::vector<Bytes> out = drain();
        REQUIRE(out.size() == 1);
        return out.front();
    }

    void replay(const std::vector<Step> &steps) {
        for (const Step &step : steps) {
            if (step.advance_us > 0) tick(step.advance_us);
            if (!step.in.empty()) send(step.in);
            std::vector<Bytes> out = drain();
            REQUIRE(out.size() == step.out.size());
            for (size_t i = 0; i < out.size(); i++)
                CHECK(hex(out[i]) == hex(pad(step.out[i])));
        }
    }
};

Data readValue(uint16_t index, uint8_t subindex) {
    Data data;
    data.u64 = 0;
    SDOAbortCodes code = SDOAbortCode_OK;
    REQUIRE(node.od().readData(data, index, subindex, code) == 0);
    return data;
}

void writeValue(uint16_t index, uint8_t subindex, Data data) {
    SDOAbortCodes code = SDOAbortCode_OK;
    REQUIRE(node.od().writeData(data, index, subindex, code) == 0);
}

const Bytes I64_MINUS_64 = le(0xFFFFFFFFFFFFFFC0ull, 8);
const uint16_t I64_MINUS_64_CRC = 0x39ff;

}  // namespace

TEST_CASE("expedited upload of 1, 2 and 4 byte objects") {
    Bench b;
    b.replay({
        {{0x40, 0x00, 0x20, 0x00}, {{0x4f, 0x00, 0x20, 0x00, 0x0b}}, 0},
        {{0x40, 0x03, 0x20, 0x00}, {{0x4b, 0x03, 0x20, 0x00, 0x32, 0x00}}, 0},
        {{0x40, 0x00, 0x10, 0x00},
         {{0x43, 0x00, 0x10, 0x00, 0x92, 0x01, 0x00, 0x00}},
         0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("segmented upload of an 8 byte object with toggle bits") {
    Bench b;
    b.replay({
        {{0x40, 0x00, 0x20, 0x05}, {{0x41, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{0x60},
         {cat({0x00}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7))},
         0},
        {{0x70}, {{0x1d, 0xff}}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("wrong toggle bit aborts the upload") {
    Bench b;
    b.replay({
        {{0x40, 0x00, 0x20, 0x05}, {{0x41, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{0x70}, {{0x80, 0x00, 0x20, 0x05, 0x00, 0x00, 0x03, 0x05}}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("expedited download writes the object") {
    Bench b;
    b.replay({
        {{0x23, 0x00, 0x20, 0x07, 0x78, 0x56, 0x34, 0x12},
         {{0x60, 0x00, 0x20, 0x07}},
         0},
    });
    CHECK(readValue(0x2000, 7).u32 == 0x12345678);
    Data restore;
    restore.u64 = 0;
    restore.u32 = 32;
    writeValue(0x2000, 7, restore);
}

TEST_CASE("expedited download without size uses the object size") {
    Bench b;
    b.replay({
        {{0x22, 0x03, 0x20, 0x00, 0x3c, 0x00, 0xaa, 0xbb},
         {{0x60, 0x03, 0x20, 0x00}},
         0},
    });
    CHECK(readValue(0x2003, 0).u16 == 60);
    Data restore;
    restore.u64 = 0;
    restore.u16 = 50;
    writeValue(0x2003, 0, restore);
}

TEST_CASE("expedited download with a wrong length aborts") {
    Bench b;
    b.replay({
        {{0x23, 0x03, 0x20, 0x00, 0x32, 0x00, 0x00, 0x00},
         {{0x80, 0x03, 0x20, 0x00, 0x12, 0x00, 0x07, 0x06}},
         0},
        {{0x2f, 0x00, 0x20, 0x07, 0x05},
         {{0x80, 0x00, 0x20, 0x07, 0x13, 0x00, 0x07, 0x06}},
         0},
    });
    CHECK(readValue(0x2003, 0).u16 == 50);
    CHECK(readValue(0x2000, 7).u32 == 32);
}

TEST_CASE("segmented download of an 8 byte object") {
    Bench b;
    Bytes value = le(0x1122334455667788ull, 8);
    b.replay({
        {{0x21, 0x00, 0x20, 0x08, 0x08}, {{0x60, 0x00, 0x20, 0x08}}, 0},
        {cat({0x00}, Bytes(value.begin(), value.begin() + 7)), {{0x20}}, 0},
        {{0x1d, value[7]}, {{0x30}}, 0},
    });
    CHECK(readValue(0x2000, 8).u64 == 0x1122334455667788ull);
    Data restore;
    restore.u64 = 64;
    writeValue(0x2000, 8, restore);
}

TEST_CASE("segmented download with a wrong announced size aborts") {
    Bench b;
    b.replay({
        {{0x21, 0x00, 0x20, 0x08, 0x09},
         {{0x80, 0x00, 0x20, 0x08, 0x12, 0x00, 0x07, 0x06}},
         0},
        {{0x21, 0x00, 0x20, 0x08, 0x08}, {{0x60, 0x00, 0x20, 0x08}}, 0},
        {{0x01, 1, 2, 3, 4, 5, 6, 7},
         {{0x80, 0x00, 0x20, 0x08, 0x13, 0x00, 0x07, 0x06}},
         0},
    });
    CHECK(readValue(0x2000, 8).u64 == 64);
}

TEST_CASE("unknown object, unknown subindex and invalid command abort") {
    Bench b;
    b.replay({
        {{0x40, 0xff, 0xff, 0x00},
         {{0x80, 0xff, 0xff, 0x00, 0x00, 0x00, 0x02, 0x06}},
         0},
        {{0x40, 0x00, 0x10, 0x09},
         {{0x80, 0x00, 0x10, 0x09, 0x11, 0x00, 0x09, 0x06}},
         0},
        {{0xe0, 0x00, 0x10, 0x00},
         {{0x80, 0x00, 0x10, 0x00, 0x01, 0x00, 0x04, 0x05}},
         0},
        {{0x60, 0x00, 0x10, 0x00},
         {{0x80, 0x00, 0x10, 0x00, 0x01, 0x00, 0x04, 0x05}},
         0},
    });
}

TEST_CASE("write on a read-only object aborts") {
    Metadata *metadata = node.od().getMetadata(0x1001, 0);
    REQUIRE(metadata != nullptr);
    metadata->access.bits.writeable = false;
    Bench b;
    b.replay({
        {{0x2f, 0x01, 0x10, 0x00, 0x05},
         {{0x80, 0x01, 0x10, 0x00, 0x02, 0x00, 0x01, 0x06}},
         0},
    });
    metadata->access.bits.writeable = true;
    CHECK(readValue(0x1001, 0).u8 == 0);
}

TEST_CASE("value outside the limits aborts") {
    Bench b;
    b.replay({
        {{0x2b, 0x03, 0x20, 0x00, 0x05, 0x00},
         {{0x80, 0x03, 0x20, 0x00, 0x32, 0x00, 0x09, 0x06}},
         0},
        {{0x2b, 0x03, 0x20, 0x00, 0xc8, 0x00},
         {{0x80, 0x03, 0x20, 0x00, 0x31, 0x00, 0x09, 0x06}},
         0},
    });
    CHECK(readValue(0x2003, 0).u16 == 50);
}

TEST_CASE("remote object pending for three turns then answered") {
    FakeHardware &hardware = initNodeOnce();
    Data stored;
    stored.u64 = 0;
    stored.u32 = 0xCAFE;
    hardware.remote[OD_OBJECT_2005_SUB0] = stored;
    hardware.remotePendingTurns = 3;
    Bench b;
    b.replay({
        {{0x40, 0x05, 0x20, 0x00}, {}, 0},
        {{}, {}, 1000},
        {{}, {}, 1000},
        {{}, {{0x43, 0x05, 0x20, 0x00, 0xfe, 0xca, 0x00, 0x00}}, 1000},
    });
    CHECK(hardware.remotePendingTurns == 0);
}

TEST_CASE("remote write pending then acknowledged") {
    FakeHardware &hardware = initNodeOnce();
    hardware.remotePendingTurns = 2;
    Bench b;
    b.replay({
        {{0x23, 0x05, 0x20, 0x00, 0x2a, 0x00, 0x00, 0x00}, {}, 0},
        {{}, {}, 1000},
        {{}, {{0x60, 0x05, 0x20, 0x00}}, 1000},
    });
    CHECK(hardware.remote[OD_OBJECT_2005_SUB0].u32 == 42);
}

TEST_CASE("remote object that never answers times out") {
    FakeHardware &hardware = initNodeOnce();
    hardware.remotePendingTurns = 1000000;
    Bench b;
    b.replay({
        {{0x40, 0x05, 0x20, 0x00}, {}, 0},
        {{}, {}, sdo::REMOTE_TIMEOUT_US / 2},
        {{},
         {{0x80, 0x05, 0x20, 0x00, 0x00, 0x00, 0x06, 0x06}},
         sdo::REMOTE_TIMEOUT_US / 2 + 1},
    });
    hardware.remotePendingTurns = 0;
    CHECK(b.sdo.isIdle());
}

TEST_CASE("a stalled segmented transfer times out") {
    Bench b;
    b.replay({
        {{0x40, 0x00, 0x20, 0x05}, {{0x41, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{}, {}, sdo::TIMEOUT_US - 1},
        {{}, {{0x80, 0x00, 0x20, 0x05, 0x00, 0x00, 0x04, 0x05}}, 1},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("client abort resets the server silently") {
    Bench b;
    b.replay({
        {{0x40, 0x00, 0x20, 0x05}, {{0x41, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{0x80, 0x00, 0x20, 0x05}, {}, 0},
        {{0x40, 0x00, 0x20, 0x00}, {{0x4f, 0x00, 0x20, 0x00, 0x0b}}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("block upload with CRC sends sub-blocks then the checksum") {
    Bench b;
    b.replay({
        {{0xa4, 0x00, 0x20, 0x05, 0x7f}, {{0xc6, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{0xa3}, {}, 0},
        {{},
         {cat({0x01}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7))},
         1},
        {{}, {{0x82, 0xff}}, 1},
        {{}, {}, 1},
        {{0xa2, 0x02, 0x7f}, {cat({0xd9}, le(I64_MINUS_64_CRC, 2))}, 0},
        {{0xa1}, {}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("block upload resends from the acknowledged sequence number") {
    Bench b;
    b.replay({
        {{0xa0, 0x00, 0x20, 0x05, 0x01}, {{0xc2, 0x00, 0x20, 0x05, 0x08}}, 0},
        {{0xa3}, {}, 0},
        {{},
         {cat({0x01}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7))},
         1},
        {{}, {}, 1},
        {{0xa2, 0x00, 0x02}, {}, 0},
        {{},
         {cat({0x01}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7))},
         1},
        {{}, {{0x82, 0xff}}, 1},
        {{0xa2, 0x02, 0x02}, {{0xd9, 0x00, 0x00}}, 0},
        {{0xa1}, {}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("block download with CRC writes the object") {
    Bench b;
    b.replay({
        {{0xc6, 0x00, 0x20, 0x05, 0x08}, {{0xa4, 0x00, 0x20, 0x05, 0x7f}}, 0},
        {cat({0x01}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7)),
         {},
         0},
        {{0x82, 0xff}, {{0xa2, 0x02, 0x7f}}, 0},
        {cat({0xd9}, le(I64_MINUS_64_CRC, 2)), {{0xa1}}, 0},
    });
    CHECK(readValue(0x2000, 5).i64 == -64);
    CHECK(b.sdo.isIdle());
}

TEST_CASE("block download with a missing sequence number is resumed") {
    Bench b;
    Bytes value = le(0x1122334455667788ull, 8);
    b.replay({
        {{0xc6, 0x00, 0x20, 0x08, 0x08}, {{0xa4, 0x00, 0x20, 0x08, 0x7f}}, 0},
        {cat({0x01}, Bytes(value.begin(), value.begin() + 7)), {}, 0},
        {{0x83, value[7]}, {{0xa2, 0x01, 0x7f}}, 0},
        {{0x81, value[7]}, {{0xa2, 0x01, 0x7f}}, 0},
        {{0xd9, 0xee, 0xc0}, {{0xa1}}, 0},
    });
    CHECK(readValue(0x2000, 8).u64 == 0x1122334455667788ull);
    Data restore;
    restore.u64 = 64;
    writeValue(0x2000, 8, restore);
}

TEST_CASE("block download with a wrong CRC aborts") {
    Bench b;
    Bytes value = le(0x1122334455667788ull, 8);
    b.replay({
        {{0xc6, 0x00, 0x20, 0x08, 0x08}, {{0xa4, 0x00, 0x20, 0x08, 0x7f}}, 0},
        {cat({0x01}, Bytes(value.begin(), value.begin() + 7)), {}, 0},
        {{0x82, value[7]}, {{0xa2, 0x02, 0x7f}}, 0},
        {{0xd9, 0x00, 0x00},
         {{0x80, 0x00, 0x20, 0x08, 0x04, 0x00, 0x04, 0x05}},
         0},
    });
    CHECK(readValue(0x2000, 8).u64 == 64);
}

TEST_CASE("block download without CRC support skips the check") {
    Bench b;
    b.replay({
        {{0xc2, 0x00, 0x20, 0x05, 0x08}, {{0xa0, 0x00, 0x20, 0x05, 0x7f}}, 0},
        {cat({0x01}, Bytes(I64_MINUS_64.begin(), I64_MINUS_64.begin() + 7)),
         {},
         0},
        {{0x82, 0xff}, {{0xa2, 0x02, 0x7f}}, 0},
        {{0xd9, 0x00, 0x00}, {{0xa1}}, 0},
    });
    CHECK(readValue(0x2000, 5).i64 == -64);
}

TEST_CASE("block download timeout acknowledges the received segments") {
    Bench b;
    Bytes value = le(0x1122334455667788ull, 8);
    b.replay({
        {{0xc6, 0x00, 0x20, 0x08, 0x08}, {{0xa4, 0x00, 0x20, 0x08, 0x7f}}, 0},
        {cat({0x01}, Bytes(value.begin(), value.begin() + 7)), {}, 0},
        {{}, {{0xa2, 0x01, 0x7f}}, sdo::BLOCK_TIMEOUT_US},
        {{}, {}, sdo::BLOCK_TIMEOUT_US},
        {{0x81, value[7]}, {{0xa2, 0x01, 0x7f}}, 0},
        {{0xd9, 0xee, 0xc0}, {{0xa1}}, 0},
    });
    CHECK(readValue(0x2000, 8).u64 == 0x1122334455667788ull);
    Data restore;
    restore.u64 = 64;
    writeValue(0x2000, 8, restore);
}

TEST_CASE("segmented download and upload of a DOMAIN go through the handler") {
    Bench b;
    b.replay({
        {{0x21, 0x02, 0x20, 0x00, 0x0a}, {{0x60, 0x02, 0x20, 0x00}}, 0},
        {{0x00, 1, 2, 3, 4, 5, 6, 7}, {{0x20}}, 0},
        {{0x19, 8, 9, 10}, {{0x30}}, 0},
    });
    CHECK(b.domain.begun == 1);
    CHECK(b.domain.announced == 10);
    CHECK(b.domain.ended == 1);
    CHECK(b.domain.endSize == 10);
    CHECK(hex(b.domain.stored) == "01 02 03 04 05 06 07 08 09 0a");

    b.domain.source = b.domain.stored;
    b.replay({
        {{0x40, 0x02, 0x20, 0x00}, {{0x41, 0x02, 0x20, 0x00, 0x0a}}, 0},
        {{0x60}, {{0x00, 1, 2, 3, 4, 5, 6, 7}}, 0},
        {{0x70}, {{0x19, 8, 9, 10}}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("DOMAIN without handler is refused") {
    Bench b;
    b.accessor.setDomainHandler(nullptr);
    b.replay({
        {{0x40, 0x02, 0x20, 0x00},
         {{0x80, 0x02, 0x20, 0x00, 0x00, 0x00, 0x01, 0x06}},
         0},
    });
}

TEST_CASE("block upload of a DOMAIN") {
    Bench b;
    for (uint8_t i = 1; i <= 20; i++) b.domain.source.push_back(i);
    b.replay({
        {{0xa0, 0x02, 0x20, 0x00, 0x7f}, {{0xc2, 0x02, 0x20, 0x00, 0x14}}, 0},
        {{0xa3}, {}, 0},
        {{}, {{0x01, 1, 2, 3, 4, 5, 6, 7}}, 1},
        {{}, {{0x02, 8, 9, 10, 11, 12, 13, 14}}, 1},
        {{}, {{0x83, 15, 16, 17, 18, 19, 20}}, 1},
        {{0xa2, 0x03, 0x7f}, {{0xc5, 0x00, 0x00}}, 0},
        {{0xa1}, {}, 0},
    });
    CHECK(b.sdo.isIdle());
}

TEST_CASE("client abort during a DOMAIN download notifies the handler") {
    Bench b;
    b.replay({
        {{0xc4, 0x02, 0x20, 0x00}, {{0xa4, 0x02, 0x20, 0x00, 0x7f}}, 0},
        {{0x01, 1, 2, 3, 4, 5, 6, 7}, {}, 0},
        {{0x80}, {}, 0},
    });
    CHECK(b.domain.aborted == 1);
    CHECK(b.domain.ended == 0);
    CHECK(b.sdo.isIdle());
}

TEST_CASE("block download of a 64 KiB DOMAIN by chunks with CRC") {
    Bench b;
    const uint32_t size = 64 * 1024;
    Bytes image(size);
    for (uint32_t i = 0; i < size; i++)
        image[i] = static_cast<uint8_t>((i * 7 + (i >> 8)) & 0xFF);
    Crc16Ccitt crc;
    crc.feed(image.data(), size);

    b.send(cat({0xc6, 0x02, 0x20, 0x00}, le(size, 4)));
    CHECK(hex(b.single()) == "a4 02 20 00 7f 00 00 00");
    REQUIRE(b.domain.announced == size);

    const uint32_t segments = (size + 6) / 7;
    uint8_t seqno = 1;
    uint32_t acks = 0;
    for (uint32_t segment = 0; segment < segments; segment++) {
        const uint32_t offset = segment * 7;
        const uint32_t length = size - offset < 7 ? size - offset : 7;
        const bool last = segment + 1 == segments;
        Bytes frame = {static_cast<uint8_t>(seqno | (last ? 0x80 : 0))};
        frame.insert(frame.end(), image.begin() + offset,
                     image.begin() + offset + length);
        b.send(frame);
        if (seqno == sdo::BLOCK_SIZE || last) {
            Bytes ack = b.single();
            CHECK(ack[0] == 0xa2);
            CHECK(ack[1] == seqno);
            CHECK(ack[2] == sdo::BLOCK_SIZE);
            acks++;
            seqno = 1;
        } else {
            CHECK(b.bus.sent.empty());
            seqno++;
        }
    }
    CHECK(acks == (segments + sdo::BLOCK_SIZE - 1) / sdo::BLOCK_SIZE);
    CHECK(b.domain.stored.size() == size - (size % 7 == 0 ? 7 : size % 7));

    const uint8_t unused =
        static_cast<uint8_t>(7 - (size % 7 == 0 ? 7 : size % 7));
    b.send(
        cat({static_cast<uint8_t>(0xc1 | (unused << 2))}, le(crc.result(), 2)));
    CHECK(hex(b.single()) == "a1 00 00 00 00 00 00 00");
    CHECK(b.domain.ended == 1);
    CHECK(b.domain.endSize == size);
    CHECK(b.domain.stored == image);
    CHECK(b.sdo.isIdle());
}

TEST_CASE("node routes RSDO frames to the SDO server") {
    FakeHardware &hardware = initNodeOnce();
    const size_t before = hardware.sent.size();
    Frame frame(OD_NODE_ID, FunctionCode_RSDO);
    frame.dlc = 8;
    frame.data[0] = 0x40;
    frame.data[1] = 0x00;
    frame.data[2] = 0x10;
    node.receiveFrame(frame);
    REQUIRE(hardware.sent.size() == before + 1);
    const Frame &response = hardware.sent.back();
    CHECK(response.getCobID() == 0x580 + OD_NODE_ID);
    CHECK(hex(Bytes(response.data, response.data + 8)) ==
          "43 00 10 00 92 01 00 00");
}
