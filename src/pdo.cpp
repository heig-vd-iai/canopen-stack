#include "pdo.hpp"
#include "node.hpp"
#include <cstring>
#include <cstdio>

CANopen_PDO::CANopen_PDO(CANopen_Node &node) : node(node)
{
    for (unsigned i = 0; i < OD_TPDO_COUNT; i++)
    {
        // OD_ObjectEntry *communication = node.od.findEntry(OD_TPDO_COMMUNICATION_INDEX + i);
        OD_ObjectEntry *mapping = node.od.findEntry(OD_TPDO_MAPPING_INDEX + i);

        tpdos[i].count = *(uint8_t *)mapping->objects[0].valueSrc;
        printf("[PDO] TPDO[%d]\n", i + 1);
        for (unsigned j = 0; j < tpdos[i].count; j++)
        {
            uint32_t content = *(uint32_t *)mapping->objects[j + 1].valueSrc;
            uint16_t index = content >> 16;
            uint8_t subIndex = content >> 8;
            uint8_t size = content;
            tpdos[i].sources[j] = (OD_Object *)node.od.findEntry(index)->objects + subIndex;
            printf("index: 0x%04X, sub-index: %d, size(bits): %d\n", index, subIndex, size);
        }
        puts("");
    }
}

void CANopen_PDO::update(uint32_t timestamp_us)
{
}

void CANopen_PDO::receiveFrame(CANopen_Frame frame)
{
}

void CANopen_PDO::build(PDOMap tpdo, uint8_t *buffer)
{
    unsigned bytesWritten = 0;
    for (unsigned i = 0; i < tpdo.count; i++)
    {
        OD_Object *obj = tpdo.sources[i];
        memcpy(buffer + bytesWritten, obj->valueSrc, obj->size);
        bytesWritten += obj->size;
    }
}
