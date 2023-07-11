#pragma once
#include <cstdint>

#define OD_ENTRIES_COUNT 4
#define OD_x1007_COUNT 2
#define OD_x1008_COUNT 2

class OD_Data
{
    uint32_t x1005;
    uint32_t x1006;
    uint32_t x1007[2];
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
    } x1008;

public:
    OD_Data() : x1005(80),
                x1006(6),
                x1007{10, 11},
                x1008{.sub0 = 3, .sub1 = 1, .sub2 = 2} {}
    friend class OD_Objects;

    void print();
    void save();
    void load();
};

class OD_Object
{
    const int accessType;
    const int dataType;
    const void *valueSrc;

public:
    OD_Object(int accessType, int dataType, void *valueSrc) : accessType(accessType), dataType(dataType), valueSrc(valueSrc) {}
};

class OD_Objects
{
    OD_Object obj_x1005[1];
    OD_Object obj_x1006[1];
    OD_Object obj_x1007[2];
    OD_Object obj_x1008[2];

public:
    OD_Objects(OD_Data &data) : obj_x1005{OD_Object(1, 1, &data.x1005)},
                                obj_x1006{OD_Object(1, 1, &data.x1006)},
                                obj_x1007{OD_Object(1, 2, &data.x1007[0]), OD_Object(1, 2, &data.x1007[1])},
                                obj_x1008{OD_Object(1, 2, &data.x1008.sub0), OD_Object(1, 2, &data.x1008.sub1)}
    {
    }
    friend class OD_ObjectDictionnary;
};

struct OD_ObjectEntry
{
    const int index;
    const int objectType;
    const int subNumber;
    const OD_Object *objects;

public:
    OD_ObjectEntry(int index, int objectType, int subNumber, OD_Object *objects) : index(index), objectType(objectType), subNumber(subNumber), objects(objects) {}
};

class OD_ObjectDictionnary
{
    int size;
    OD_Data data;
    OD_Objects objects;
    OD_ObjectEntry entries[1];

public:
    OD_ObjectDictionnary() : objects(data), entries{OD_ObjectEntry(1, 2, 3, objects.obj_x1005)}
    {
    }
};
