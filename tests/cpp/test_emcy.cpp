#include <vector>

#include "doctest.h"
#include "emcy/config.hpp"
#include "emcy/frames.hpp"
#include "full-harness.hpp"
#include "od.hpp"

using namespace CANopen;

namespace {

const uint16_t EMCY_COBID = 0x80 + OD_NODE_ID;

struct Emergency {
    uint16_t code;
    uint8_t reg;
    uint32_t manufacturer;
};

/** Starts error-free in Pre-Operational and leaves the node that way. */
struct EmcyBench {
    FakeHardware &hardware;
    size_t baseline;

    EmcyBench() : hardware(initNodeOnce()) {
        toPreOperational();
        canopen.emcy.reset();
        canopen.emcy.clearHistory();
        canopen.emcy.setErrorBehavior(ErrorBehaviorValue_PreOperational,
                                      ErrorBehaviorValue_PreOperational);
        baseline = hardware.sent.size();
    }

    ~EmcyBench() {
        toPreOperational();
        canopen.emcy.reset();
        canopen.emcy.clearHistory();
        canopen.emcy.setErrorBehavior(ErrorBehaviorValue_PreOperational,
                                      ErrorBehaviorValue_PreOperational);
    }

    void toPreOperational() {
        if (node.nmt().getState() != NMTState_PreOperational)
            node.nmt().setTransition(NMTServiceCommand_EnterPreOperational);
    }

    std::vector<Emergency> emergencies() {
        std::vector<Emergency> out;
        for (size_t i = baseline; i < hardware.sent.size(); i++) {
            const Frame &frame = hardware.sent[i];
            if (frame.getCobID() != EMCY_COBID) continue;
            const emcy::Message message(frame);
            REQUIRE(message.isWellFormed());
            out.push_back({message.errorCode(), message.errorRegister(),
                           message.manufacturerCode()});
        }
        return out;
    }

    uint8_t registerFromOd() {
        Data data;
        data.u64 = 0;
        SDOAbortCodes code = SDOAbortCode_OK;
        REQUIRE(node.od().readData(data, 0x1001, 0, code) == 0);
        return data.u8;
    }
};

}  // namespace

TEST_CASE("raising an error sets the register, records it and sends an EMCY") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide, 0x1234);

    std::vector<Emergency> sent = b.emergencies();
    REQUIRE(sent.size() == 1);
    CHECK(sent[0].code == 0x2100);
    CHECK(sent[0].reg == 0x03);
    CHECK(sent[0].manufacturer == 0x1234);
    CHECK(canopen.emcy.getErrorRegister() == 0x03);
    CHECK(b.registerFromOd() == 0x03);
    REQUIRE(canopen.emcy.history().count() == 1);
    CHECK(canopen.emcy.history().at(0) == 0x12342100u);
}

TEST_CASE("each error class sets its own register bit") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Voltage_Main);
    canopen.emcy.raiseError(EMCYErrorCode_Temperature_Device);
    canopen.emcy.raiseError(EMCYErrorCode_Communication_CANOverrun);
    canopen.emcy.raiseError(EMCYErrorCode_DeviceSpecific);
    canopen.emcy.raiseError(EMCYErrorCode_AdditionalFunctions);
    canopen.emcy.raiseError(EMCYErrorCode_Software_User);
    CHECK(canopen.emcy.getErrorRegister() == 0xBD);
    CHECK(b.emergencies().size() == 6);
}

TEST_CASE("a repeated error is recorded once, a new one shifts the history") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide);
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide);
    CHECK(canopen.emcy.history().count() == 1);
    CHECK(b.emergencies().size() == 2);

    canopen.emcy.raiseError(EMCYErrorCode_Voltage_Main);
    REQUIRE(canopen.emcy.history().count() == 2);
    CHECK(canopen.emcy.history().at(0) == 0x3100);
    CHECK(canopen.emcy.history().at(1) == 0x2100);
    CHECK(canopen.emcy.getErrorRegister() == 0x07);
}

TEST_CASE("the history keeps the eight most recent errors") {
    EmcyBench b;
    for (uint16_t i = 1; i <= 10; i++)
        canopen.emcy.raiseError(static_cast<uint16_t>(0x6000 + i));
    REQUIRE(canopen.emcy.history().count() == emcy::HISTORY_SIZE);
    CHECK(canopen.emcy.history().at(0) == 0x600A);
    CHECK(canopen.emcy.history().at(7) == 0x6003);
    canopen.emcy.clearHistory();
    CHECK(canopen.emcy.history().count() == 0);
}

TEST_CASE("clearing the last error bit sends an error reset once") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide);
    canopen.emcy.clearErrorBit(ErrorRegisterBit_Current);
    CHECK(canopen.emcy.getErrorRegister() == 0);
    std::vector<Emergency> sent = b.emergencies();
    REQUIRE(sent.size() == 2);
    CHECK(sent[1].code == EMCYErrorCode_Reset);
    CHECK(sent[1].reg == 0);

    canopen.emcy.clearErrorBit(ErrorRegisterBit_Current);
    CHECK(b.emergencies().size() == 2);
}

TEST_CASE("the generic bit stays while a specific bit is set") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide);
    canopen.emcy.raiseError(EMCYErrorCode_Voltage_Main);
    CHECK(canopen.emcy.getErrorRegister() == 0x07);
    canopen.emcy.clearErrorBit(ErrorRegisterBit_Generic);
    CHECK(canopen.emcy.getErrorRegister() == 0x07);
    canopen.emcy.clearErrorBit(EMCYErrorCode_Current);
    CHECK(canopen.emcy.getErrorRegister() == 0x05);
    canopen.emcy.clearErrorBit(EMCYErrorCode_Voltage);
    CHECK(canopen.emcy.getErrorRegister() == 0x00);
    CHECK(b.emergencies().size() == 3);
}

TEST_CASE("a generic error clears with the generic bit") {
    EmcyBench b;
    canopen.emcy.raiseError(EMCYErrorCode_Software_Internal);
    CHECK(canopen.emcy.getErrorRegister() == 0x01);
    canopen.emcy.clearErrorBit(ErrorRegisterBit_Generic);
    CHECK(canopen.emcy.getErrorRegister() == 0x00);
    CHECK(b.emergencies().size() == 2);
}

TEST_CASE("the error behavior drives the NMT state") {
    EmcyBench b;
    node.nmt().setTransition(NMTServiceCommand_Start);
    canopen.emcy.raiseError(EMCYErrorCode_DeviceHardware);
    CHECK(node.nmt().getState() == NMTState_PreOperational);

    canopen.emcy.setErrorBehavior(ErrorBehaviorValue_Stop,
                                  ErrorBehaviorValue_None);
    node.nmt().setTransition(NMTServiceCommand_Start);
    canopen.emcy.raiseError(EMCYErrorCode_DeviceHardware);
    CHECK(node.nmt().getState() == NMTState_Operational);
    canopen.emcy.raiseError(EMCYErrorCode_Communication_HeartbeatError);
    CHECK(node.nmt().getState() == NMTState_Stopped);
}

TEST_CASE("errors are not reported while stopped") {
    EmcyBench b;
    node.nmt().setTransition(NMTServiceCommand_Stop);
    canopen.emcy.raiseError(EMCYErrorCode_Current_InputSide);
    CHECK(b.emergencies().empty());
    CHECK(canopen.emcy.getErrorRegister() == 0);
    CHECK(canopen.emcy.history().count() == 0);
}
