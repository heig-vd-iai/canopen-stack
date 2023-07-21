#include "pdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstring>
#include <cstdio>

CANopen_PDO::CANopen_PDO(CANopen_Node &node) : node(node)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
        initTPDO(i);
}

void CANopen_PDO::initTPDO(unsigned index)
{
    tpdos[index].commObject = (TPDOCommunicationObject *)node.od.findObject(TPDO_COMMUNICATION_INDEX + index);
    tpdos[index].mapObject = (TPDOMappingObject *)node.od.findObject(TPDO_MAPPING_INDEX + index);
    remapTPDO(index);
}

void CANopen_PDO::remapTPDO(unsigned index)
{
    TPDO *tpdo = tpdos + index;
    tpdo->count = tpdo->mapObject->getCount();
    tpdo->count = tpdo->count > TPDO_MAX_MAPPED_OBJECTS ? TPDO_MAX_MAPPED_OBJECTS : tpdo->count;
    if (tpdo->mappedEntries != NULL)
        delete[] tpdo->mappedEntries;
    tpdo->mappedEntries = new ObjectEntry *[tpdo->count]; // TODO: check for NULL?
    printf("[PDO] TPDO[%d]\n", index + 1);
    for (unsigned i = 0; i < tpdo->count; i++)
    {
        TPDOMapEntry content = {tpdo->mapObject->getMappedValue(i)};
        tpdo->mappedEntries[i] = (ObjectEntry *)node.od.findObject(content.bits.index)->entries + content.bits.subindex;
        printf("index: 0x%04X, sub-index: %d, size(bits): %d\n", content.bits.index, content.bits.subindex, content.bits.length);
    }
    puts("");
}

void CANopen_PDO::bufferizeTPDO(unsigned index, uint8_t *buffer)
{
    TPDO *tpdo = tpdos + index;
    unsigned bytesTransferred = 0;
    for (unsigned i = 0; i < tpdo->count; i++)
    {
        ObjectEntry *entry = tpdo->mappedEntries[i];
        entry->readBytes(buffer, entry->size, bytesTransferred);
        bytesTransferred += entry->size;
    }
}

void CANopen_PDO::update(uint32_t timestamp_us)
{
    NMTStates state = node.nmt.getState();
    if (state != NMTState_Operational)
        return;
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        TPDO *tpdo = tpdos + i;
        uint8_t transmission = tpdo->commObject->getTransmissionType();
        uint16_t timer_ms = tpdo->commObject->getEventTimer();
        // Only event-driven PDOs can be sent periodically
        if (!(timer_ms != 0 && (transmission == X1800_EVENT1 || transmission == X1800_EVENT2)))
            continue;
        if (timestamp_us - tpdo->timestamp_us < timer_ms * 1000)
            continue;
        CANopen_Frame frame;
        TPDOCobidEntry cobid = {tpdo->commObject->getCobId()};
        frame.functionCode = (cobid.bits.canId >> 7) & 0x0F;
        frame.nodeId = cobid.bits.canId & 0x7F;
        frame.dlc = 8;
        bufferizeTPDO(i, frame.data);
        node.sendFrame(frame);
        tpdo->timestamp_us = timestamp_us;
    }

    // for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    // {
    //     uint8_t transmission = *tpdos[i].transmissionType;
    //     uint16_t timer = *tpdos[i].eventTimer;
    //     // Only event-driven PDOs can be sent periodically
    //     if (timer != 0 && (transmission == 0xFE || transmission == 0xFF)) // TODO
    //     {
    //         if (timestamp_us - tpdos[i].timestamp >= timer * 1000)
    //         {
    //             CANopen_Frame frame;
    //             CobId cobId = {*tpdos[i].cobId};
    //             frame.functionCode = (cobId.bits.canId >> 7) & 0x0F;
    //             frame.nodeId = cobId.bits.canId & 0x7F;
    //             // printf("[PDO] send PDO, function code: %d, node id: %d (%X)\n", frame.functionCode, frame.nodeId, cobId.value);
    //             frame.dlc = 8;
    //             bufferizeTPDO(i, frame.data);
    //             node.sendFrame(frame);
    //             tpdos[i].timestamp = timestamp_us;
    //         }
    //     }
    // }
}

void CANopen_PDO::receiveFrame(CANopen_Frame frame)
{
}
