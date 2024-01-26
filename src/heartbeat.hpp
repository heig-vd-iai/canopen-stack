/**
 * Contains the declaration of the HB class.
 */
#pragma once
#include "enums.hpp"
#include <cstdint>
#define HEARTBEAT_DLC 1
#define HEARTBEAT_STATE_OFFSET 0
#define TOGGLE_OFFSET 7

namespace CANopen
{
/**
 * Heartbeat object.
 * It automatically handles the following protocols: node guarding, heartbeat, boot-up.
 * See CiA301:2011§7.2.8.3.2.2 (p. 76)
 */
class HB
{
private:
    uint32_t lastPublish = 0;
    uint8_t toggleBit = 0;

    /**
     * Internal method to publish the NMT state.
     * @param state NMT state to publish.
     * @param toggleBit Toggle bit value for node guarding (defaults to 0).
     */
    void publishState(NMTStates state, uint8_t toggleBit = 0);

    /**
     * Update this object.
     * If producer heartbeat time (0x1017) is configured, the state will be published at said rate.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void update(uint32_t timestamp_us);

    /**
     * Receive and process a node guarding RTR frame.
     * @param frame Frame to be processed.
     */
    void receiveFrame(class Frame &frame);

    /**
     * Reset the toggle bit value to 0.
     * This method should only be called by the NMT state machine.
     */
    void resetToggleBit();

public:
    friend class NMT;
    friend class Node;
    /**
     * Constructor for the HB class.
     * @param node The parent Node reference.
     */
    HB(class Node &node);
};
}
