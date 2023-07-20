#include "pdo.hpp"
#include "node.hpp"
#include "enums.hpp"
#include <cstring>
#include <cstdio>

CANopen_PDO::CANopen_PDO(CANopen_Node &node) : node(node)
{
    // for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    // {
    //     initTPDO(i);
    // }
}

// void CANopen_PDO::initTPDO(unsigned num)
// {
//     OD_ObjectEntry *communicationParameterEntry = node.od.findEntry(OD_TPDO_COMMUNICATION_INDEX + num);
//     tpdos[num].cobId = (uint32_t *)communicationParameterEntry->objects[1].valueSrc;
//     tpdos[num].transmissionType = (uint8_t *)communicationParameterEntry->objects[2].valueSrc;
//     tpdos[num].inhibitTime = (uint16_t *)communicationParameterEntry->objects[3].valueSrc;
//     tpdos[num].eventTimer = (uint16_t *)communicationParameterEntry->objects[5].valueSrc;
//     tpdos[num].syncStartValue = (uint8_t *)communicationParameterEntry->objects[6].valueSrc;
//     remapTPDO(num);
// }

// void CANopen_PDO::remapTPDO(unsigned num)
// {
//     const unsigned maxMappedObjects = 8; // This CANopen implementation does not support bit-level PDO mapping
//     OD_ObjectEntry *mappingParameterEntry = node.od.findEntry(OD_TPDO_MAPPING_INDEX + num);
//     uint8_t declaredObjectsCount = *(uint8_t *)mappingParameterEntry->objects[0].valueSrc;
//     declaredObjectsCount = declaredObjectsCount > maxMappedObjects ? maxMappedObjects : declaredObjectsCount;
//     tpdos[num].mappedObjectsCount = declaredObjectsCount;
//     if (tpdos[num].mappedObjects != NULL)
//         delete[] tpdos[num].mappedObjects;
//     tpdos[num].mappedObjects = new OD_Object *[declaredObjectsCount];
//     // printf("[PDO] TPDO[%d]\n", num + 1);
//     for (unsigned i = 0; i < declaredObjectsCount; i++)
//     {
//         uint32_t content = *(uint32_t *)mappingParameterEntry->objects[i + 1].valueSrc;
//         uint16_t index = content >> 16;
//         uint8_t subIndex = content >> 8;
//         // uint8_t bitSize = content;
//         tpdos[num].mappedObjects[i] = (OD_Object *)node.od.findEntry(index)->objects + subIndex;
//         // printf("index: 0x%04X, sub-index: %d, size(bits): %d\n", index, subIndex, bitSize);
//     }
//     // puts("");
// }

// void CANopen_PDO::bufferizeTPDO(unsigned num, uint8_t buffer[8])
// {
//     unsigned bytesWritten = 0;
//     for (unsigned i = 0; i < tpdos[num].mappedObjectsCount; i++)
//     {
//         OD_Object *obj = tpdos[num].mappedObjects[i];
//         memcpy(buffer + bytesWritten, obj->valueSrc, obj->size);
//         bytesWritten += obj->size;
//     }
// }

void CANopen_PDO::update(uint32_t timestamp_us)
{
    NMTStates state = node.nmt.getState();
    if (state != NMTState_Operational)
        return;
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

// CANopen_PDO::TPDO::TPDO() : mappedObjectsCount(0), mappedObjects{NULL}, timestamp(0)
// {
// }
