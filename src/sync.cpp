/******************************************************************************
 * [Filename]:      sync.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the definitions of the SYNC class.
 *****************************************************************************/
#include "sync.hpp"
#include "enums.hpp"
#include "frame.hpp"
#include "node.hpp"
#include "objects/object_1019.hpp"
using namespace CANopen;

SYNC::SYNC(Node &node) : node(node)
{
#ifdef OD_OBJECT_1019
    uint8_t tmp;
    node._od.at(OD_OBJECT_1019)->getValue(0, &tmp);
    maxCounter = tmp < X1019_MIN_COUNTER ? X1019_MAX_COUNTER : tmp;
#else
    maxCounter = X1019_MAX_COUNTER;
#endif
}

void SYNC::enable() { enabled = true; }

void SYNC::disable() { enabled = false; }

void SYNC::receiveFrame(SYNCFrame &frame, uint32_t timestamp_us)
{
    if (!enabled || frame.nodeId != 0)
        return;
    internalCounter = frame.isCounter() ? frame.getCounter() : internalCounter % maxCounter + 1;
    if (onSyncFunc)
        onSyncFunc(internalCounter);
    node._pdo.onSync(internalCounter, timestamp_us);
}

void SYNC::onSync(std::function<void(unsigned)> callback)
{
    onSyncFunc = callback;
}
