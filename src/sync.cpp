#include "sync.hpp"
#include "node.hpp"
#include <cstdio>
using namespace CANopen;

SYNC::SYNC(Node &node) : node(node)
{
}

void SYNC::receiveFrame(Frame frame, uint32_t timestamp_us)
{
    uint8_t counter = frame.dlc > 0 ? frame.data[0] : 0;
    node.pdo.onSync(counter, timestamp_us);
}
