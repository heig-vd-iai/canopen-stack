#pragma once

#include <cstdint>

#include "hal/can-transport.hpp"

namespace CANopen {

/**
 * SocketCAN transport for Linux, single-threaded: receiveFrame() polls the
 * socket with MSG_DONTWAIT and getTime_us() reads CLOCK_MONOTONIC.
 */
class SocketCanTransport : public CanTransport {
   public:
    explicit SocketCanTransport(const char *interface = "vcan0");
    ~SocketCanTransport() override;

    void setInterface(const char *interface);
    bool isOpen() const;

    void init() override;
    void sendFrame(const Frame &frame) override;
    bool receiveFrame(Frame &frame) override;
    uint32_t getTime_us() override;

   private:
    const char *interface;
    int sock = -1;
};
}  // namespace CANopen
