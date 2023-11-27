/**
 * SYNC consumer.
 *
 * The SYNC producer broadcasts the synchronization object periodically.
 * This SYNC provides the basic network synchronization mechanism.
 * The time period between the SYNCs is specified by the standard
 * parameter communication cycle period.
 */
#pragma once

#include <cstdint>
#include <functional>

#define SYNC_COUNTER_OFFSET 0

namespace CANopen
{
/**
 * SYNC object.
 * It automatically handles the reception of SYNC events and calls associated callback for PDO class.
 * See CiA301:2011§7.2.5 (p. 62)
 */
class SYNC
{
private:
    bool enabled = false;
    class Node &node;
    uint8_t internalCounter = 1;
    uint8_t maxCounter;
    std::function<void(unsigned)> onSyncFunc;

    /**
     * Enable EMCY functionality, should be used by NMT only.
     * TODO: Why enabling SYNC would enable EMCY and why should be used by NMT only? This is confusing.
     */
    void enable();

    /**
     * Disable EMCY functionality, should be used by NMT only.
     */
    void disable();

    /**
     * Receive and process a SYNC frame.
     * This method receives and processes a SYNC frame, updating internal state and performing actions as needed.
     * @param frame SYNCFrame to be processed.
     * @param timestamp_us Timestamp in microseconds of the frame reception.
     */
    void receiveFrame(class SYNCFrame &frame, uint32_t timestamp_us);

public:
    friend class NMT;
    friend class Node;

    /**
     * Constructor for the SYNC class.
     * @param node The parent Node reference.
     */
    SYNC(class Node &node);

    /**
     * Set a callback function to be called when a SYNC message is received.
     * The function will receive the counter value.
     * This callback is called before any PDO action.
     * **DO NOT use time consuming calls in the provided callback.**
     * @param callback Callback function to be called on SYNC event.
     */
    void onSync(std::function<void(unsigned)> callback);
};
}
