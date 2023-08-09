#include "pdo.hpp"
#include "node.hpp"
#include "frame.hpp"
#include "enums.hpp"
#include "objects/object_1800.hpp"
#include <cstring>
#include <cstdlib>
using namespace CANopen;

PDO::TPDO::TPDO() : mappedEntries(NULL) {}

PDO::PDO(Node &node) : node(node)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        initTPDO(i);
}

void PDO::initTPDO(unsigned index)
{
    tpdos[index].commObject = (Object1800 *)node.od.findObject(TPDO_COMMUNICATION_INDEX + index);
    tpdos[index].mapObject = (Object1A00 *)node.od.findObject(TPDO_MAPPING_INDEX + index);
    remapTPDO(index);
}

void PDO::remapTPDO(unsigned index)
{
    TPDO *tpdo = tpdos + index;
    unsigned count = tpdo->mapObject->getCount();
    tpdo->count = 0;
    if (tpdo->mappedEntries != NULL)
        delete[] tpdo->mappedEntries;
    tpdo->mappedEntries = new TPDOPair[count];
    unsigned sizeSum = 0;
    for (unsigned i = 0; i < count; i++)
    {
        TPDOMapEntry content = {tpdo->mapObject->getMappedValue(i)};
        Object *object = node.od.findObject(content.bits.index);
        sizeSum += object->getSize(content.bits.subindex);
        if (sizeSum > PDO_DATA_LENGTH)
        {
            break;
        }
        tpdo->mappedEntries[i].object = object;
        tpdo->mappedEntries[i].subindex = content.bits.subindex;
        tpdo->size = sizeSum;
        tpdo->count++;
    }
}

void PDO::bufferizeTPDO(unsigned index, uint8_t *buffer)
{
    TPDO *tpdo = tpdos + index;
    unsigned bytesTransferred = 0;
    for (unsigned i = 0; i < tpdo->count; i++)
    {
        Object *object = tpdo->mappedEntries[i].object;
        uint8_t subindex = tpdo->mappedEntries[i].subindex;
        uint8_t size = object->getSize(subindex);
        object->readBytes(subindex, buffer, size, bytesTransferred);
        bytesTransferred += size;
    }
}

void PDO::sendTPDO(unsigned index, uint32_t timestamp_us)
{
    TPDO *tpdo = tpdos + index;
    if (!enabled || !tpdo->commObject->isEnabled())
        return;
    TPDOCobidEntry cobid = {tpdo->commObject->getCobId()};
    Frame frame;
    frame.cobId.value = cobid.value & 0x07FF;
    frame.dlc = tpdo->size;
    bufferizeTPDO(index, frame.data);
    tpdo->syncFlag = false;
    node.sendFrame(frame);
    tpdo->timestamp_us = timestamp_us;
}

uint32_t PDO::getSyncWindow_us()
{
    uint32_t value = 0;
#ifdef OD_OBJECT_1007
    node.at(OD_OBJECT_1007)->getValue(0, &value);
#endif
    return value;
}

void PDO::enable() { enabled = true; }

void PDO::disable() { enabled = false; }

void PDO::receiveFrame(Frame frame, uint32_t timestamp_us)
{
    if (!enabled || !frame.rtr)
        return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        TPDO *tpdo = tpdos + i;
        uint8_t transmission = tpdo->commObject->getTransmissionType();
        if (transmission == X1800_RTR_SYNC) // RTR sync
        {
            tpdo->syncFlag = true;
        }
        else if (transmission == X1800_RTR_EVENT) // RTR event-driven
        {
            sendTPDO(i, timestamp_us);
        }
    }
}

void PDO::update(uint32_t timestamp_us)
{
    if (!enabled)
        return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        TPDO *tpdo = tpdos + i;
        uint8_t transmission = tpdo->commObject->getTransmissionType();
        uint16_t timer_ms = tpdo->commObject->getEventTimer();
        // Only event-driven PDOs can be sent periodically
        if ((transmission != X1800_EVENT1 && transmission != X1800_EVENT2) || !tpdo->commObject->isTimerSupported() || timer_ms == 0 || timestamp_us - tpdo->timestamp_us < timer_ms * 1000)
            continue;
        sendTPDO(i, timestamp_us);
    }
}

void PDO::onSync(uint8_t counter, uint32_t timestamp_us)
{
    if (!enabled)
        return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        TPDO *tpdo = tpdos + i;
        if (!tpdo->commObject->isSynchronous())
            continue;
        uint8_t transmission = tpdo->commObject->getTransmissionType();
        // synchronous acyclic || synchronous cyclic || RTR synchronous
        bool send = transmission == 0 || (transmission <= X1800_SYNC_MAX && (counter && !(counter % transmission))) || (transmission == X1800_RTR_SYNC && tpdo->syncFlag);
        if (send)
        {
            uint32_t syncWindow = getSyncWindow_us();
            if (syncWindow != 0 && node.getTime_us() - timestamp_us > syncWindow)
                break;
            sendTPDO(i, timestamp_us);
        }
    }
}

void PDO::transmitTPDO(unsigned index)
{
    if (!enabled || index >= OD_TPDO_COUNT)
        return;
    TPDO *tpdo = tpdos + index;
    uint8_t transmission = tpdo->commObject->getTransmissionType();
    if (transmission == X1800_ACYCLIC) // synchronous acyclic
    {
        tpdo->syncFlag = true;
    }
    else if (transmission >= X1800_EVENT1) // event-driven
    {
        uint32_t timestamp_us = node.getTime_us();
        bool supported = tpdo->commObject->isInhibitSupported();
        if (!supported || (supported && (tpdo->commObject->getInhibitTime_us() == 0 || timestamp_us - tpdo->timestamp_us >= tpdo->commObject->getInhibitTime_us())))
        {
            sendTPDO(index, timestamp_us);
        }
    }
}

void PDO::reload()
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        remapTPDO(i);
}
