/**
 * The stack a bootloader links: a node, an SDO server and a heartbeat.
 * Nothing here may pull the PDO, SYNC or EMCY services in.
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <cstring>
#include <vector>

#include "doctest.h"
#include "fake-hardware.hpp"
#include "heartbeat.hpp"
#include "node.hpp"
#include "od/domainHandler.hpp"
#include "sdo/sdoServer.hpp"
#include "utils/crc.hpp"

using namespace CANopen;

namespace {

typedef std::vector<uint8_t> Bytes;

Bytes pad(Bytes bytes) {
    bytes.resize(8, 0);
    return bytes;
}

/** Stands in for the flash driver of a bootloader. */
struct FlashDomain : public DomainHandler {
    Bytes stored;
    uint32_t announced = 0;
    bool ended = false;

    SDOAbortCodes beginDownload(int32_t, uint32_t size) override {
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
    SDOAbortCodes endDownload(int32_t, uint32_t) override {
        ended = true;
        return SDOAbortCode_OK;
    }
    void abortDownload(int32_t) override {}
    uint32_t uploadSize(int32_t) override {
        return static_cast<uint32_t>(stored.size());
    }
    SDOAbortCodes uploadChunk(int32_t, uint32_t offset, uint8_t *bytes,
                              uint32_t length) override {
        memcpy(bytes, stored.data() + offset, length);
        return SDOAbortCode_OK;
    }
};

/** A bootloader node: NMT and the object dictionary, SDO, heartbeat. */
struct MinimalNode {
    FakeHardware hardware;
    Node node;
    HB hb;
    SDO sdo;
    FlashDomain flash;

    MinimalNode()
        : node(hardware, hardware, hardware),
          hb(node.od(), hardware, node.nodeId),
          sdo(node.odAccessor(), hardware, node.nodeId) {
        node.attach(hb);
        node.attach(sdo);
        bindHeartbeat(hb);
        node.odAccessor().setDomainHandler(&flash);
        node.init();
    }

    void send(const Bytes &bytes) {
        Frame frame(OD_NODE_ID, FunctionCode_RSDO);
        frame.dlc = 8;
        memcpy(frame.data, pad(bytes).data(), 8);
        hardware.incoming.push_back(frame);
        node.update();
    }

    void command(NMTServiceCommands command) {
        Frame frame(0, FunctionCode_NMT);
        frame.dlc = 2;
        frame.data[0] = command;
        frame.data[1] = OD_NODE_ID;
        hardware.incoming.push_back(frame);
        node.update();
    }

    Bytes answer() {
        REQUIRE(hardware.sent.size() == 1);
        const Frame &frame = hardware.sent.front();
        CHECK(frame.getCobID() == 0x580 + OD_NODE_ID);
        Bytes out(frame.data, frame.data + 8);
        hardware.sent.clear();
        return out;
    }
};

}  // namespace

TEST_CASE("the minimal node boots up and answers NMT commands") {
    MinimalNode bootloader;
    REQUIRE(bootloader.hardware.sent.size() == 1);
    const Frame &bootUp = bootloader.hardware.sent.front();
    CHECK(bootUp.getCobID() == 0x700 + OD_NODE_ID);
    CHECK(bootUp.dlc == 1);
    CHECK(bootUp.data[0] == NMTState_Initialisation);
    CHECK(bootloader.node.nmt().getState() == NMTState_PreOperational);

    bootloader.hardware.clearSent();
    bootloader.command(NMTServiceCommand_Start);
    CHECK(bootloader.node.nmt().getState() == NMTState_Operational);
    bootloader.command(NMTServiceCommand_EnterPreOperational);
    CHECK(bootloader.node.nmt().getState() == NMTState_PreOperational);
}

TEST_CASE("the minimal node produces a heartbeat") {
    MinimalNode bootloader;
    Data period;
    period.u64 = 0;
    period.u16 = 100;
    REQUIRE(bootloader.node.od().writeData(period, 0x1017, 0) == 0);

    bootloader.hardware.clearSent();
    bootloader.hardware.advance(50 * 1000);
    bootloader.node.update();
    CHECK(bootloader.hardware.sent.empty());

    bootloader.hardware.advance(60 * 1000);
    bootloader.node.update();
    REQUIRE(bootloader.hardware.sent.size() == 1);
    CHECK(bootloader.hardware.sent.front().getCobID() == 0x700 + OD_NODE_ID);
    CHECK(bootloader.hardware.sent.front().data[0] == NMTState_PreOperational);
}

TEST_CASE("a firmware image is written to 0x1F50 by block download") {
    MinimalNode bootloader;
    bootloader.hardware.clearSent();

    Bytes image;
    for (unsigned i = 0; i < 30; i++) image.push_back(static_cast<uint8_t>(i));

    // Initiate: client CRC support, size known.
    bootloader.send({0xC6, 0x50, 0x1F, 0x01, static_cast<uint8_t>(image.size()),
                     0x00, 0x00, 0x00});
    Bytes response = bootloader.answer();
    CHECK(response[0] == 0xA4);
    CHECK(response[1] == 0x50);
    CHECK(response[2] == 0x1F);
    CHECK(response[3] == 0x01);
    const uint8_t blockSize = response[4];
    REQUIRE(blockSize >= 1);

    uint32_t offset = 0;
    uint8_t seqno = 0;
    while (offset < image.size()) {
        const uint32_t length =
            image.size() - offset > 7 ? 7 : image.size() - offset;
        const bool last = offset + length >= image.size();
        Bytes segment;
        segment.push_back(static_cast<uint8_t>((last ? 0x80 : 0x00) | ++seqno));
        for (uint32_t i = 0; i < length; i++)
            segment.push_back(image[offset + i]);
        bootloader.send(segment);
        offset += length;
        if (seqno == blockSize || last) {
            Bytes ack = bootloader.answer();
            CHECK(ack[0] == 0xA2);
            CHECK(ack[1] == seqno);
            seqno = 0;
        } else {
            CHECK(bootloader.hardware.sent.empty());
        }
    }

    Crc16Ccitt crc;
    crc.feed(image.data(), static_cast<uint32_t>(image.size()));
    const uint16_t checksum = crc.result();
    const uint8_t lastLength =
        static_cast<uint8_t>(image.size() % 7 == 0 ? 7 : image.size() % 7);
    bootloader.send({static_cast<uint8_t>(0xC1 | ((7 - lastLength) << 2)),
                     static_cast<uint8_t>(checksum & 0xFF),
                     static_cast<uint8_t>(checksum >> 8)});
    Bytes end = bootloader.answer();
    CHECK(end[0] == 0xA1);

    CHECK(bootloader.flash.ended);
    CHECK(bootloader.flash.announced == image.size());
    REQUIRE(bootloader.flash.stored.size() == image.size());
    CHECK(memcmp(bootloader.flash.stored.data(), image.data(), image.size()) ==
          0);
}

TEST_CASE("program control is readable and writable over SDO") {
    MinimalNode bootloader;
    bootloader.hardware.clearSent();

    bootloader.send({0x2F, 0x51, 0x1F, 0x01, 0x01});
    Bytes written = bootloader.answer();
    CHECK(written[0] == 0x60);

    bootloader.send({0x40, 0x51, 0x1F, 0x01});
    Bytes read = bootloader.answer();
    CHECK(read[0] == 0x4F);
    CHECK(read[4] == 0x01);
}
