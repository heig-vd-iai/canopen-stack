#include "pdo.hpp"
#include "node.hpp"
#include "frame.hpp"
#include "enums.hpp"
#include "objects/object_1800.hpp"
#include <cstring>
using namespace CANopen;

PDO::PDO(Node &node) : node(node)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        initTPDO(i);
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++)
        initRPDO(i);
}

void PDO::initTPDO(unsigned index)
{
    tpdos[index].commObject = (Object1800 *)node.od.findObject(TPDO_COMMUNICATION_INDEX + index);
    tpdos[index].mapObject = (Object1A00 *)node.od.findObject(TPDO_MAPPING_INDEX + index);
    remapTPDO(index);
}

void PDO::initRPDO(unsigned index)
{
    rpdos[index].commObject = (Object1400 *)node.od.findObject(RPDO_COMMUNICATION_INDEX + index);
    rpdos[index].mapObject = (Object1600 *)node.od.findObject(RPDO_MAPPING_INDEX + index);
    remapRPDO(index);
}

void PDO::remapTPDO(unsigned index)
{
    TPDO *tpdo = tpdos + index;
    unsigned count = tpdo->mapObject->getCount();
    tpdo->count = 0;
    if (tpdo->mappedEntries)
        delete[] tpdo->mappedEntries;
    tpdo->mappedEntries = new PDOPair[count];
    unsigned sizeSum = 0;
    for (unsigned i = 0; i < count; i++)
    {
        PDOMapEntry content = {tpdo->mapObject->getMappedValue(i)};
        Object *object = node.od.findObject(content.bits.index);
        sizeSum += object->getSize(content.bits.subindex);
        if (sizeSum > PDO_DLC)
            break;
        tpdo->mappedEntries[i].object = object;
        tpdo->mappedEntries[i].subindex = content.bits.subindex;
        tpdo->size = sizeSum;
        tpdo->count++;
    }
}

void PDO::remapRPDO(unsigned index)
{
    RPDO *rpdo = rpdos + index;
    unsigned count = rpdo->mapObject->getCount();
    rpdo->count = 0;
    if (rpdo->mappedEntries)
        delete[] rpdo->mappedEntries;
    rpdo->mappedEntries = new PDOPair[count];
    unsigned sizeSum = 0;
    for (unsigned i = 0; i < count; i++)
    {
        PDOMapEntry content = {rpdo->mapObject->getMappedValue(i)};
        Object *object = node.od.findObject(content.bits.index);
        sizeSum += object->getSize(content.bits.subindex);
        if (sizeSum > PDO_DLC)
            break;
        rpdo->mappedEntries[i].object = object;
        rpdo->mappedEntries[i].subindex = content.bits.subindex;
        rpdo->size = sizeSum;
        rpdo->count++;
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

void PDO::unpackRPDO(unsigned index, uint8_t *buffer)
{
    RPDO *rpdo = rpdos + index;
    unsigned bytesTransferred = 0;
    for (unsigned i = 0; i < rpdo->count; i++)
    {
        Object *object = rpdo->mappedEntries[i].object;
        uint8_t subindex = rpdo->mappedEntries[i].subindex;
        uint8_t size = object->getSize(subindex);
        object->writeBytes(subindex, buffer + bytesTransferred, size, node);
        bytesTransferred += size;
    } // TODO: event-timer timeout
}

void PDO::sendTPDO(unsigned index, uint32_t timestamp_us)
{
    TPDO *tpdo = tpdos + index;
    if (!enabled || !tpdo->commObject->isEnabled())
        return;
    Frame frame(tpdo->commObject->getActualCobId());
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

void PDO::receiveTPDO(Frame &frame, uint32_t timestamp_us)
{
    if (!enabled || frame.nodeId != node.nodeId || !frame.rtr)
        return;
    uint8_t index;
    switch (frame.functionCode)
    {
    case FunctionCode_TPDO1:
        index = 0;
        break;
    case FunctionCode_TPDO2:
        index = 1;
        break;
    case FunctionCode_TPDO3:
        index = 2;
        break;
    case FunctionCode_TPDO4:
        index = 3;
        break;
    default:
        return;
    }
    TPDO *tpdo = tpdos + index;
    uint8_t transmission = tpdo->commObject->getTransmissionType();
    if (transmission == X1800_RTR_SYNC)
        tpdo->syncFlag = true;
    else if (transmission == X1800_RTR_EVENT)
        sendTPDO(index, timestamp_us);
}

void PDO::receiveRPDO(Frame &frame, uint32_t timestamp_us)
{
    if (!enabled || frame.nodeId != node.nodeId)
        return;
    uint8_t index = 42;
    switch (frame.functionCode)
    {
    case FunctionCode_RPDO1:
        index = 0;
        break;
    case FunctionCode_RPDO2:
        index = 1;
        break;
    case FunctionCode_RPDO3:
        index = 2;
        break;
    case FunctionCode_RPDO4:
        index = 3;
        break;
    default:
        return;
    }
    RPDO *rpdo = rpdos + index;
    if (rpdo->size != frame.dlc)
        return;
    uint8_t transmission = rpdo->commObject->getTransmissionType();
    if (transmission <= X1400_SYNC_MAX)
    {
        memcpy(rpdo->buffer, frame.data, frame.dlc);
        rpdo->syncFlag = true;
    }
    else
        unpackRPDO(index, frame.data);
}

void PDO::update(uint32_t timestamp_us)
{
    if (!enabled)
        return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        TPDO *tpdo = tpdos + i;
        uint8_t transmission = tpdo->commObject->getTransmissionType();
        uint32_t timer_us = tpdo->commObject->getEventTimer_us();
        // Only event-driven PDOs can be sent periodically
        if ((transmission != X1800_EVENT1 && transmission != X1800_EVENT2) || !tpdo->commObject->isTimerSupported() || timer_us == 0 || timestamp_us - tpdo->timestamp_us < timer_us)
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
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++)
    {
        RPDO *rpdo = rpdos + i;
        if (!rpdo->commObject->isSynchronous() || !rpdo->syncFlag)
            continue;
        unpackRPDO(i, rpdo->buffer);
        rpdo->syncFlag = false;
    }
}

void PDO::transmitTPDO(unsigned index)
{
    if (!enabled || index >= OD_TPDO_COUNT)
        return;
    TPDO *tpdo = tpdos + index;
    uint8_t transmission = tpdo->commObject->getTransmissionType();
    if (transmission == X1800_ACYCLIC)
    {
        tpdo->syncFlag = true;
    }
    else if (transmission >= X1800_EVENT1)
    {
        uint32_t timestamp_us = node.getTime_us();
        bool supported = tpdo->commObject->isInhibitSupported();
        if (!supported || (supported && (tpdo->commObject->getInhibitTime_us() == 0 || timestamp_us - tpdo->timestamp_us >= tpdo->commObject->getInhibitTime_us())))
        {
            sendTPDO(index, timestamp_us);
        }
    }
}

void PDO::reloadTPDO()
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        remapTPDO(i);
}

void PDO::reloadRPDO()
{
    for (unsigned i = 0; i < OD_RPDO_COUNT; i++)
        remapRPDO(i);
}
