#pragma once

#include <cstdint>

namespace CANopen {

constexpr uint8_t CAN_DATA_LENGTH = 8;
constexpr unsigned FUNCTION_OFFSET = 7;
constexpr uint16_t COBID_MASK = 0x7FF;
constexpr uint8_t FUNCTION_MASK = 0xF;
constexpr uint8_t NODEID_MASK = 0x7F;

/** Function codes of the pre-defined connection set. CiA301:2011§7.3.3 */
enum FunctionCodes {
    FunctionCode_NMT = 0b0000,
    FunctionCode_SYNC = 0b0001,
    FunctionCode_EMCY = 0b0001,
    FunctionCode_TIME = 0b0010,
    FunctionCode_TPDO1 = 0b0011,
    FunctionCode_RPDO1 = 0b0100,
    FunctionCode_TPDO2 = 0b0101,
    FunctionCode_RPDO2 = 0b0110,
    FunctionCode_TPDO3 = 0b0111,
    FunctionCode_RPDO3 = 0b1000,
    FunctionCode_TPDO4 = 0b1001,
    FunctionCode_RPDO4 = 0b1010,
    FunctionCode_TSDO = 0b1011,
    FunctionCode_RSDO = 0b1100,
    FunctionCode_HEARTBEAT = 0b1110,
};

/**
 * A classic CAN frame with an 11 bit identifier split into its CANopen
 * function code and node id. Each service views the data through its own
 * frames.hpp.
 */
struct Frame {
    uint8_t nodeId = 0;
    uint8_t functionCode = 0;
    uint8_t data[CAN_DATA_LENGTH] = {0};
    uint8_t dlc = 0;
    bool rtr = false;

    Frame(uint8_t nodeId = 0, uint8_t functionCode = 0);

    /** Build a frame from its 11 bit identifier. */
    static Frame fromCobId(uint16_t cobId);

    /** The 11 bit identifier of this frame. */
    uint16_t getCobID() const;
};
}  // namespace CANopen
