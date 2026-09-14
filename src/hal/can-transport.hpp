#pragma once

#include <cstdint>

namespace CANopen {
struct Frame;

/**
 * CAN bus access and monotonic clock.
 * The stack polls the bus from Node::update(): receiveFrame() must never
 * block.
 */
class CanTransport {
   public:
    virtual ~CanTransport() = default;
    virtual void init() = 0;
    virtual void sendFrame(const Frame &frame) = 0;
    /**
     * Pop one pending frame from the receive queue.
     * @return True when a frame was written to `frame`, false when the queue
     * is empty.
     */
    virtual bool receiveFrame(Frame &frame) = 0;
    /**
     * Relative clock in microseconds.
     * The value only needs to be monotonic and to wrap at 0xFFFFFFFF.
     */
    virtual uint32_t getTime_us() = 0;
};
}  // namespace CANopen
