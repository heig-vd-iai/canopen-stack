#include "sync.hpp"
#include "node.hpp"
#include <cstdlib>
using namespace CANopen;

SYNC::SYNC(Node &node) : node(node), syncCounterOverflow(NULL)
{
    syncCounterOverflow = node.findObject(SYNC_OBJECT_1019);
}

void SYNC::receiveFrame(Frame frame, uint32_t timestamp_us)
{
    internalCounter = frame.dlc > 0 ? frame.data[0] : internalCounter % SYNC_MAX_COUNTER + 1;
    node.pdo.onSync(internalCounter, timestamp_us);
}
