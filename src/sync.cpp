#include "sync.hpp"
#include "node.hpp"
#include "frame.hpp"
#include "enums.hpp"
using namespace CANopen;

SYNC::SYNC(Node &node) : node(node) {}

void SYNC::enable() { enabled = true; }

void SYNC::disable() { enabled = false; }

void SYNC::receiveFrame(Frame frame, uint32_t timestamp_us)
{
    if (!enabled)
        return;
    internalCounter = frame.dlc > 0 ? frame.data[0] : internalCounter % SYNC_MAX_COUNTER + 1;
    node.pdo.onSync(internalCounter, timestamp_us);
}
