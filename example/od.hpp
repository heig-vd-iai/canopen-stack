#pragma once
#include <cstdint>
#include "od_data.hpp"

// struct AccessType
// {
//     uint8_t read : 1;
//     uint8_t write : 1;
//     uint8_t constant : 1;
// };
/*
class OD_AbstractObject
{
private:
    int accessType;
    int dataType;
};

template <typename T>
class OD_Object : public OD_AbstractObject
{
private:
    T &value;

public:
    inline OD_Object(T val) : value(val) {}
    inline T &get() { return value; }
    inline void set(T val) { value = val; };
};

class OD_ObjectEntry
{
private:
    uint16_t index;
    uint8_t subNumber;
    uint8_t objectType;
    OD_AbstractObject *objects[];

public:
    // inline OD_ObjectEntry(uint16_t index, uint8_t subNumber, uint8_t objectType, OD_AbstractObject objects) : index(index), subNumber(subNumber), objectType(objectType), objects(objects) {}
    // inline OD_AbstractObject &get(uint8_t subIndex) { return objects[subIndex]; }S
    friend class ObjectDictionnary;
};

class ObjectDictionnary
{
private:
    OD_Data data;

public:
    ObjectDictionnary()
    {
        data.print();
    }
};
*/