/**
 * Contains the definitions of all of the frame classes.
 */
#include "frame.hpp"

#include "emergency.hpp"
#include "heartbeat.hpp"
#include "sync.hpp"

using namespace CANopen;

Frame::Frame(uint8_t nodeId, uint8_t functionCode)
    : nodeId(nodeId), functionCode(functionCode) {}

Frame Frame::fromCobId(uint16_t cobId) {
    return Frame(cobId & NODEID_MASK,
                 (cobId >> FUNCTION_OFFSET) & FUNCTION_MASK);
}

uint16_t Frame::getCobID() const {
    return ((uint16_t)functionCode & FUNCTION_MASK) << FUNCTION_OFFSET |
           (nodeId & NODEID_MASK);
}

HeartbeatFrame::HeartbeatFrame(uint8_t nodeId, uint8_t state)
    : Frame(nodeId, FunctionCode_HEARTBEAT) {
    dlc = HEARTBEAT_DLC;
    setNMTState(state);
}

void HeartbeatFrame::setNMTState(uint8_t state) {
    data[HEARTBEAT_STATE_OFFSET] = state;
}

EmergencyFrame::EmergencyFrame(uint8_t nodeId, uint16_t errorCode,
                               uint8_t errorRegister, uint32_t manufacturerCode)
    : Frame(nodeId, FunctionCode_EMCY) {
    dlc = EMCY_DLC;
    setErrorCode(errorCode);
    setErrorRegister(errorRegister);
    setManufacturerCode(manufacturerCode);
}

void EmergencyFrame::setErrorCode(uint16_t errorCode) {
    *(uint16_t *)(data + EMCY_ERRCODE_OFFSET) = errorCode;
}

void EmergencyFrame::setErrorRegister(uint8_t errorRegister) {
    data[EMCY_ERRREG_OFFSET] = errorRegister;
}

void EmergencyFrame::setManufacturerCode(uint32_t manufacturerCode) {
    *(uint32_t *)(data + EMCY_MANUFACTURER_OFFSET) = manufacturerCode;
}

SYNCFrame::SYNCFrame(uint8_t nodeId) : Frame(nodeId, FunctionCode_SYNC) {}

bool SYNCFrame::isCounter() const { return dlc > 0; }

uint8_t SYNCFrame::getCounter() const { return data[SYNC_COUNTER_OFFSET]; }
