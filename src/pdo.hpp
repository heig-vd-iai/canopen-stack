#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"
#include "od.hpp"
#define TPDO_MAX_MAPPED_OBJECTS 8
#define TPDO_COMMUNICATION_INDEX 0x1800
#define TPDO_MAPPING_INDEX 0x1A00

class CANopen_PDO
{
    struct TPDO
    {
        TPDOCommunicationObject *commObject;
        TPDOMappingObject *mapObject;
        uint8_t count;
        ObjectEntry **mappedEntries = NULL;
        uint32_t timestamp_us;
    };

private:
    class CANopen_Node &node;
    TPDO tpdos[OD_TPDO_COUNT];

    void initTPDO(unsigned index);
    void remapTPDO(unsigned index);
    void bufferizeTPDO(unsigned index, uint8_t *buffer);

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void update(uint32_t timestamp_us);
};
