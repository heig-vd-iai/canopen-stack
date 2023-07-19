#include "pdo.hpp"
#include "node.hpp"
#include <cstring>
#include <cstdio>

CANopen_PDO::CANopen_PDO(CANopen_Node &node) : node(node)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        initTPDO(i);
    }
}

void CANopen_PDO::initTPDO(unsigned num)
{
    OD_ObjectEntry *communicationParameterEntry = node.od.findEntry(OD_TPDO_COMMUNICATION_INDEX + num);
    tpdos[num].cobId = (uint32_t *)communicationParameterEntry->objects[1].valueSrc;
    tpdos[num].transmissionType = (uint8_t *)communicationParameterEntry->objects[2].valueSrc;
    tpdos[num].inhibitTime = (uint16_t *)communicationParameterEntry->objects[3].valueSrc;
    tpdos[num].eventTimer = (uint16_t *)communicationParameterEntry->objects[5].valueSrc;
    tpdos[num].syncStartValue = (uint8_t *)communicationParameterEntry->objects[6].valueSrc;
    remapTPDO(num);
}

void CANopen_PDO::remapTPDO(unsigned num)
{
    printf("[PDO] TPDO[%d]\n", num + 1);
    OD_ObjectEntry *mappingParameterEntry = node.od.findEntry(OD_TPDO_MAPPING_INDEX + num);
    tpdos[num].mappedObjectsCount = *(uint8_t *)mappingParameterEntry->objects[0].valueSrc;
    for (unsigned i = 0; i < tpdos[num].mappedObjectsCount; i++)
    {
        uint32_t content = *(uint32_t *)mappingParameterEntry->objects[i + 1].valueSrc;
        uint16_t index = content >> 16;
        uint8_t subIndex = content >> 8;
        uint8_t size = content;
        tpdos[num].mappedObjectsSources[i] = (OD_Object *)node.od.findEntry(index)->objects + subIndex;
        printf("index: 0x%04X, sub-index: %d, size(bits): %d\n", index, subIndex, size);
    }
    puts("");
}

void CANopen_PDO::bufferizeTPDO(unsigned num, uint8_t buffer[8])
{
    unsigned bytesWritten = 0;
    for (unsigned i = 0; i < tpdos[num].mappedObjectsCount; i++)
    {
        OD_Object *obj = tpdos[num].mappedObjectsSources[i];
        memcpy(buffer + bytesWritten, obj->valueSrc, obj->size);
        bytesWritten += obj->size;
    }
}

void CANopen_PDO::update(uint32_t timestamp_us)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        tpdos[i];
    }
}

void CANopen_PDO::receiveFrame(CANopen_Frame frame)
{
}

CANopen_PDO::CANopen_TPDO::CANopen_TPDO() : mappedObjectsCount(0), mappedObjectsSources{NULL}
{
}
