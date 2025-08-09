#pragma once

#include <cstdint>

#define CAN_DATA_LENGTH 8


/**
 * CANopen generic frame that is used to send and receive messages.
 * It has to be converted to a CAN frame before it can be sent on the bus.
 */
struct Frame {
    alignas(4) uint8_t data[CAN_DATA_LENGTH] = {0};
    uint8_t nodeId = 0;
    uint8_t functionCode = 0;
    uint8_t dlc = 0;
    bool rtr = false;  // Remote transmission request

    Frame(uint8_t nodeId = 0, uint8_t functionCode = 0)
        : nodeId(nodeId), functionCode(functionCode) {}

    static Frame fromCobId(uint16_t cobId) {
        return Frame(cobId & 0x7F, (cobId >> 7) & 0x0F);
    }

    uint16_t Frame::getCobID() const {
        return ((uint16_t)functionCode & 0x7F) << 7 | (nodeId & 0x7F);
    }

    const uint8_t* bytes() const { return data; }
    uint8_t* bytes() { return data; }
};

/**
 * This is a specialized Frame used by the SDO class.
 * It is the base class for SDOFrame.
 * It contains common accessors for both its subclasses.
 *
 * CiA301:2011 §7.3.3 (p. 80)
 *
 * SDOTFrame: SDO Client -> SDO Server
 * SDORFrame: SDO Server -> SDO Client
 */
struct SDOTFrame : public Frame {
    SDOTFrame(uint8_t nodeId) : Frame(nodeId, 0b1011) { dlc = 8; }
};

struct SDORFrame : public Frame {
    SDORFrame(uint8_t nodeId) : Frame(nodeId, 0b1100) { dlc = 8; }
};