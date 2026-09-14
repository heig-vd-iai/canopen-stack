#pragma once

#include <cstdint>

#include "hal/can-transport.hpp"

namespace CANopen {

/**
 * MCAN0 in classic CAN mode with one RX FIFO filtered on the node's COB-IDs,
 * and CPUTimer0 as a free-running microsecond clock.
 */
class C2000CanTransport : public CanTransport {
   public:
    explicit C2000CanTransport(uint8_t nodeId);

    void init() override;
    void sendFrame(const Frame &frame) override;
    bool receiveFrame(Frame &frame) override;
    uint32_t getTime_us() override;

   private:
    void initCan();
    void addFilters();
    void initTimer();

    const uint8_t nodeId;
};
}  // namespace CANopen
