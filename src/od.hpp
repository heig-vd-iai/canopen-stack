/******************************************************************************
 * [Filename]:      od.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          Generated on August 22, 2023
 * [Description]:   This file contains the declaration of the ObjectDictionnary class.
 *                  It is auto-generated from the device's EDS file, so manually editing is not recommended.
 *****************************************************************************/
#pragma once
#include "od_include.hpp"
#define OD_OBJECTS_COUNT 63
#define OD_GRANULARITY 8
#define OD_PDO_MAPPING_MAX 8
#define OD_ENTRY_SIZE_MAX 1024
#define OD_TPDO_COUNT 4
#define OD_RPDO_COUNT 4
#define OD_OBJECT_2 0
#define OD_OBJECT_3 1
#define OD_OBJECT_4 2
#define OD_OBJECT_5 3
#define OD_OBJECT_6 4
#define OD_OBJECT_7 5
#define OD_OBJECT_1000 6
#define OD_OBJECT_1001 7
#define OD_OBJECT_1003 8
#define OD_OBJECT_1005 9
#define OD_OBJECT_1006 10
#define OD_OBJECT_1007 11
#define OD_OBJECT_1008 12
#define OD_OBJECT_1009 13
#define OD_OBJECT_100A 14
#define OD_OBJECT_100C 15
#define OD_OBJECT_100D 16
#define OD_OBJECT_1010 17
#define OD_OBJECT_1011 18
#define OD_OBJECT_1014 19
#define OD_OBJECT_1016 20
#define OD_OBJECT_1017 21
#define OD_OBJECT_1018 22
#define OD_OBJECT_1019 23
#define OD_OBJECT_1020 24
#define OD_OBJECT_1029 25
#define OD_OBJECT_1400 26
#define OD_OBJECT_1401 27
#define OD_OBJECT_1402 28
#define OD_OBJECT_1403 29
#define OD_OBJECT_1600 30
#define OD_OBJECT_1601 31
#define OD_OBJECT_1602 32
#define OD_OBJECT_1603 33
#define OD_OBJECT_1800 34
#define OD_OBJECT_1801 35
#define OD_OBJECT_1802 36
#define OD_OBJECT_1803 37
#define OD_OBJECT_1A00 38
#define OD_OBJECT_1A01 39
#define OD_OBJECT_1A02 40
#define OD_OBJECT_1A03 41
#define OD_OBJECT_1F51 42
#define OD_OBJECT_1F57 43
#define OD_OBJECT_1F80 44
#define OD_OBJECT_6007 45
#define OD_OBJECT_603F 46
#define OD_OBJECT_6040 47
#define OD_OBJECT_6041 48
#define OD_OBJECT_6042 49
#define OD_OBJECT_6043 50
#define OD_OBJECT_6044 51
#define OD_OBJECT_6046 52
#define OD_OBJECT_6048 53
#define OD_OBJECT_6049 54
#define OD_OBJECT_604A 55
#define OD_OBJECT_6060 56
#define OD_OBJECT_6061 57
#define OD_OBJECT_6064 58
#define OD_OBJECT_607A 59
#define OD_OBJECT_6081 60
#define OD_OBJECT_60FD 61
#define OD_OBJECT_60FE 62

namespace CANopen
{
    class ObjectDictionnary
    {
    private:
        struct ObjectDictionnaryData
        {
            int8_t x2 = 0;
            int16_t x3 = 0;
            int32_t x4 = 0;
            uint8_t x5 = 0;
            uint16_t x6 = 0;
            uint32_t x7 = 0;
            uint32_t x1000 = 131474;
            uint8_t x1001 = 0;
            uint8_t x1003sub0 = 0;
            uint32_t x1003[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            uint32_t x1005 = 128;
            uint32_t x1006 = 0;
            uint32_t x1007 = 0;
            uint8_t x1008[17] = {80, 68, 52, 45, 69, 66, 53, 57, 67, 68, 45, 69, 45, 54, 53, 45, 50};
            uint8_t x1009[12] = {97, 109, 111, 110, 103, 32, 117, 115, 32, 224, 182, 158};
            uint8_t x100A[18] = {70, 73, 82, 45, 118, 50, 50, 49, 51, 45, 66, 49, 48, 51, 49, 49, 51, 52};
            uint16_t x100C = 0;
            uint8_t x100D = 0;
            uint8_t x1010sub0 = 13;
            uint32_t x1010[13] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1};
            uint8_t x1011sub0 = 13;
            uint32_t x1011[13] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1};
            uint32_t x1014 = 129;
            uint8_t x1016sub0 = 1;
            uint32_t x1016[1] = {0};
            uint16_t x1017 = 0;
            struct {uint8_t sub0 = 4; uint32_t sub1 = 620; uint32_t sub2 = 70; uint32_t sub3 = 145031168; uint32_t sub4 = 0;} x1018;
            uint8_t x1019 = 0;
            uint8_t x1020sub0 = 2;
            uint32_t x1020[2] = {0, 0};
            uint8_t x1029sub0 = 2;
            uint8_t x1029[2] = {0, 1};
            struct {uint8_t sub0 = 5; uint32_t sub1 = 513; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0;} x1400;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 769; uint8_t sub2 = 255;} x1401;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 1025; uint8_t sub2 = 255;} x1402;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 1281; uint8_t sub2 = 255;} x1403;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 1614807056; uint32_t sub2 = 1616904200; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1600;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 1618608160; uint32_t sub2 = 1619066912; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1601;
            struct {uint8_t sub0 = 1; uint32_t sub1 = 1614938128; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1602;
            struct {uint8_t sub0 = 1; uint32_t sub1 = 1627259168; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1603;
            struct {uint8_t sub0 = 6; uint32_t sub1 = 385; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1800;
            struct {uint8_t sub0 = 6; uint32_t sub1 = 641; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1801;
            struct {uint8_t sub0 = 6; uint32_t sub1 = 897; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1802;
            struct {uint8_t sub0 = 6; uint32_t sub1 = 1153; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1803;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 1614872592; uint32_t sub2 = 1616969736; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A00;
            struct {uint8_t sub0 = 1; uint32_t sub1 = 1617166368; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A01;
            struct {uint8_t sub0 = 1; uint32_t sub1 = 1615069200; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A02;
            struct {uint8_t sub0 = 1; uint32_t sub1 = 1627193376; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A03;
            uint8_t x1F51[974] = {76, 111, 114, 101, 109, 32, 105, 112, 115, 117, 109, 32, 100, 111, 108, 111, 114, 32, 115, 105, 116, 32, 97, 109, 101, 116, 44, 32, 99, 111, 110, 115, 101, 99, 116, 101, 116, 117, 114, 32, 97, 100, 105, 112, 105, 115, 99, 105, 110, 103, 32, 101, 108, 105, 116, 44, 32, 115, 101, 100, 32, 100, 111, 32, 101, 105, 117, 115, 109, 111, 100, 32, 116, 101, 109, 112, 111, 114, 32, 105, 110, 99, 105, 100, 105, 100, 117, 110, 116, 32, 117, 116, 32, 108, 97, 98, 111, 114, 101, 32, 101, 116, 32, 100, 111, 108, 111, 114, 101, 32, 109, 97, 103, 110, 97, 32, 97, 108, 105, 113, 117, 97, 46, 32, 67, 111, 110, 118, 97, 108, 108, 105, 115, 32, 97, 32, 99, 114, 97, 115, 32, 115, 101, 109, 112, 101, 114, 32, 97, 117, 99, 116, 111, 114, 32, 110, 101, 113, 117, 101, 32, 118, 105, 116, 97, 101, 32, 116, 101, 109, 112, 117, 115, 32, 113, 117, 97, 109, 32, 112, 101, 108, 108, 101, 110, 116, 101, 115, 113, 117, 101, 46, 32, 78, 101, 116, 117, 115, 32, 101, 116, 32, 109, 97, 108, 101, 115, 117, 97, 100, 97, 32, 102, 97, 109, 101, 115, 32, 97, 99, 32, 116, 117, 114, 112, 105, 115, 32, 101, 103, 101, 115, 116, 97, 115, 32, 109, 97, 101, 99, 101, 110, 97, 115, 46, 32, 70, 97, 117, 99, 105, 98, 117, 115, 32, 112, 117, 108, 118, 105, 110, 97, 114, 32, 101, 108, 101, 109, 101, 110, 116, 117, 109, 32, 105, 110, 116, 101, 103, 101, 114, 32, 101, 110, 105, 109, 32, 110, 101, 113, 117, 101, 46, 32, 79, 100, 105, 111, 32, 117, 116, 32, 101, 110, 105, 109, 32, 98, 108, 97, 110, 100, 105, 116, 32, 118, 111, 108, 117, 116, 112, 97, 116, 32, 109, 97, 101, 99, 101, 110, 97, 115, 46, 32, 65, 108, 105, 113, 117, 97, 109, 32, 102, 97, 117, 99, 105, 98, 117, 115, 32, 112, 117, 114, 117, 115, 32, 105, 110, 32, 109, 97, 115, 115, 97, 32, 116, 101, 109, 112, 111, 114, 46, 32, 67, 111, 110, 115, 101, 113, 117, 97, 116, 32, 115, 101, 109, 112, 101, 114, 32, 118, 105, 118, 101, 114, 114, 97, 32, 110, 97, 109, 32, 108, 105, 98, 101, 114, 111, 32, 106, 117, 115, 116, 111, 46, 32, 69, 103, 101, 116, 32, 101, 115, 116, 32, 108, 111, 114, 101, 109, 32, 105, 112, 115, 117, 109, 32, 100, 111, 108, 111, 114, 32, 115, 105, 116, 32, 97, 109, 101, 116, 32, 99, 111, 110, 115, 101, 99, 116, 101, 116, 117, 114, 32, 97, 100, 105, 112, 105, 115, 99, 105, 110, 103, 46, 32, 73, 112, 115, 117, 109, 32, 97, 32, 97, 114, 99, 117, 32, 99, 117, 114, 115, 117, 115, 32, 118, 105, 116, 97, 101, 32, 99, 111, 110, 103, 117, 101, 32, 109, 97, 117, 114, 105, 115, 32, 114, 104, 111, 110, 99, 117, 115, 32, 97, 101, 110, 101, 97, 110, 46, 32, 67, 111, 110, 115, 101, 99, 116, 101, 116, 117, 114, 32, 108, 111, 114, 101, 109, 32, 100, 111, 110, 101, 99, 32, 109, 97, 115, 115, 97, 32, 115, 97, 112, 105, 101, 110, 32, 102, 97, 117, 99, 105, 98, 117, 115, 32, 101, 116, 46, 32, 73, 110, 32, 109, 101, 116, 117, 115, 32, 118, 117, 108, 112, 117, 116, 97, 116, 101, 32, 101, 117, 32, 115, 99, 101, 108, 101, 114, 105, 115, 113, 117, 101, 32, 102, 101, 108, 105, 115, 32, 105, 109, 112, 101, 114, 100, 105, 101, 116, 46, 32, 78, 117, 110, 99, 32, 112, 117, 108, 118, 105, 110, 97, 114, 32, 115, 97, 112, 105, 101, 110, 32, 101, 116, 32, 108, 105, 103, 117, 108, 97, 32, 117, 108, 108, 97, 109, 99, 111, 114, 112, 101, 114, 32, 109, 97, 108, 101, 115, 117, 97, 100, 97, 32, 112, 114, 111, 105, 110, 46, 32, 72, 97, 99, 32, 104, 97, 98, 105, 116, 97, 115, 115, 101, 32, 112, 108, 97, 116, 101, 97, 32, 100, 105, 99, 116, 117, 109, 115, 116, 32, 113, 117, 105, 115, 113, 117, 101, 32, 115, 97, 103, 105, 116, 116, 105, 115, 32, 112, 117, 114, 117, 115, 32, 115, 105, 116, 32, 97, 109, 101, 116, 46, 32, 78, 117, 110, 99, 32, 99, 111, 110, 103, 117, 101, 32, 110, 105, 115, 105, 32, 118, 105, 116, 97, 101, 32, 115, 117, 115, 99, 105, 112, 105, 116, 32, 116, 101, 108, 108, 117, 115, 32, 109, 97, 117, 114, 105, 115, 46, 32, 83, 101, 100, 32, 98, 108, 97, 110, 100, 105, 116, 32, 108, 105, 98, 101, 114, 111, 32, 118, 111, 108, 117, 116, 112, 97, 116, 32, 115, 101, 100, 32, 99, 114, 97, 115, 32, 111, 114, 110, 97, 114, 101, 32, 97, 114, 99, 117, 32, 100, 117, 105, 46, 32, 78, 105, 98, 104, 32, 116, 111, 114, 116, 111, 114, 32, 105, 100, 32, 97, 108, 105, 113, 117, 101, 116, 32, 108, 101, 99, 116, 117, 115, 46, 32, 83, 117, 115, 112, 101, 110, 100, 105, 115, 115, 101, 32, 115, 101, 100, 32, 110, 105, 115, 105, 32, 108, 97, 99, 117, 115, 32, 115, 101, 100, 46, 32, 86, 105, 116, 97, 101, 32, 117, 108, 116, 114, 105, 99, 105, 101, 115, 32, 108, 101, 111, 32, 105, 110, 116, 101, 103, 101, 114, 32, 109, 97, 108, 101, 115, 117, 97, 100, 97, 32, 110, 117, 110, 99, 32, 118, 101, 108, 32, 114, 105, 115, 117, 115, 46};
            uint8_t x1F57sub0 = 2;
            uint32_t x1F57[2] = {0, 0};
            uint32_t x1F80 = 0;
            int16_t x6007 = 1;
            uint16_t x603F = 0;
            uint16_t x6040 = 0;
            uint16_t x6041 = 0;
            int16_t x6042 = 200;
            int16_t x6043 = 0;
            int16_t x6044 = 0;
            uint8_t x6046sub0 = 2;
            uint32_t x6046[2] = {0, 30000};
            struct {uint8_t sub0 = 2; double sub1 = 500.0; float sub2 = 1.0;} x6048;
            struct {uint8_t sub0 = 2; uint32_t sub1 = 500; uint16_t sub2 = 1;} x6049;
            struct {uint8_t sub0 = 2; uint64_t sub1 = 5000; uint64_t sub2 = 1;} x604A;
            int8_t x6060 = 0;
            int8_t x6061 = 0;
            int32_t x6064 = 420;
            int32_t x607A = 4000;
            uint32_t x6081 = 500;
            uint32_t x60FD = 0;
            uint8_t x60FEsub0 = 1;
            uint32_t x60FE[1] = {0};
        };
        struct 
        {
            struct
            {
                ObjectDictionnaryData data;
                ObjectEntry<int8_t> x2sub0 = ObjectEntry<int8_t>(&data.x2, 1);
                const ObjectEntryBase *x2[1] = {&x2sub0};
                ObjectEntry<int16_t> x3sub0 = ObjectEntry<int16_t>(&data.x3, 1);
                const ObjectEntryBase *x3[1] = {&x3sub0};
                ObjectEntry<int32_t> x4sub0 = ObjectEntry<int32_t>(&data.x4, 1);
                const ObjectEntryBase *x4[1] = {&x4sub0};
                ObjectEntry<uint8_t> x5sub0 = ObjectEntry<uint8_t>(&data.x5, 1);
                const ObjectEntryBase *x5[1] = {&x5sub0};
                ObjectEntry<uint16_t> x6sub0 = ObjectEntry<uint16_t>(&data.x6, 1);
                const ObjectEntryBase *x6[1] = {&x6sub0};
                ObjectEntry<uint32_t> x7sub0 = ObjectEntry<uint32_t>(&data.x7, 1);
                const ObjectEntryBase *x7[1] = {&x7sub0};
                ObjectEntry<uint32_t> x1000sub0 = ObjectEntry<uint32_t>(&data.x1000, 1);
                const ObjectEntryBase *x1000[1] = {&x1000sub0};
                ObjectEntry<uint8_t> x1001sub0 = ObjectEntry<uint8_t>(&data.x1001, 5);
                const ObjectEntryBase *x1001[1] = {&x1001sub0};
                ObjectEntry<uint8_t> x1003sub0 = ObjectEntry<uint8_t>(&data.x1003sub0, 3);
                ObjectEntry<uint32_t> x1003sub1 = ObjectEntry<uint32_t>(&data.x1003[0], 1);
                ObjectEntry<uint32_t> x1003sub2 = ObjectEntry<uint32_t>(&data.x1003[1], 1);
                ObjectEntry<uint32_t> x1003sub3 = ObjectEntry<uint32_t>(&data.x1003[2], 1);
                ObjectEntry<uint32_t> x1003sub4 = ObjectEntry<uint32_t>(&data.x1003[3], 1);
                ObjectEntry<uint32_t> x1003sub5 = ObjectEntry<uint32_t>(&data.x1003[4], 1);
                ObjectEntry<uint32_t> x1003sub6 = ObjectEntry<uint32_t>(&data.x1003[5], 1);
                ObjectEntry<uint32_t> x1003sub7 = ObjectEntry<uint32_t>(&data.x1003[6], 1);
                ObjectEntry<uint32_t> x1003sub8 = ObjectEntry<uint32_t>(&data.x1003[7], 1);
                const ObjectEntryBase *x1003[9] = {&x1003sub0, &x1003sub1, &x1003sub2, &x1003sub3, &x1003sub4, &x1003sub5, &x1003sub6, &x1003sub7, &x1003sub8};
                ObjectEntry<uint32_t> x1005sub0 = ObjectEntry<uint32_t>(&data.x1005, 3);
                const ObjectEntryBase *x1005[1] = {&x1005sub0};
                ObjectEntry<uint32_t> x1006sub0 = ObjectEntry<uint32_t>(&data.x1006, 3);
                const ObjectEntryBase *x1006[1] = {&x1006sub0};
                ObjectEntry<uint32_t> x1007sub0 = ObjectEntry<uint32_t>(&data.x1007, 3);
                const ObjectEntryBase *x1007[1] = {&x1007sub0};
                ObjectEntry<uint8_t[17]> x1008sub0 = ObjectEntry<uint8_t[17]>(&data.x1008, 1);
                const ObjectEntryBase *x1008[1] = {&x1008sub0};
                ObjectEntry<uint8_t[12]> x1009sub0 = ObjectEntry<uint8_t[12]>(&data.x1009, 1);
                const ObjectEntryBase *x1009[1] = {&x1009sub0};
                ObjectEntry<uint8_t[18]> x100Asub0 = ObjectEntry<uint8_t[18]>(&data.x100A, 1);
                const ObjectEntryBase *x100A[1] = {&x100Asub0};
                ObjectEntry<uint16_t> x100Csub0 = ObjectEntry<uint16_t>(&data.x100C, 3);
                const ObjectEntryBase *x100C[1] = {&x100Csub0};
                ObjectEntry<uint8_t> x100Dsub0 = ObjectEntry<uint8_t>(&data.x100D, 3);
                const ObjectEntryBase *x100D[1] = {&x100Dsub0};
                ObjectEntry<uint8_t> x1010sub0 = ObjectEntry<uint8_t>(&data.x1010sub0, 1);
                ObjectEntry<uint32_t> x1010sub1 = ObjectEntry<uint32_t>(&data.x1010[0], 3);
                ObjectEntry<uint32_t> x1010sub2 = ObjectEntry<uint32_t>(&data.x1010[1], 3);
                ObjectEntry<uint32_t> x1010sub3 = ObjectEntry<uint32_t>(&data.x1010[2], 3);
                ObjectEntry<uint32_t> x1010sub4 = ObjectEntry<uint32_t>(&data.x1010[3], 3);
                ObjectEntry<uint32_t> x1010sub5 = ObjectEntry<uint32_t>(&data.x1010[4], 3);
                ObjectEntry<uint32_t> x1010sub6 = ObjectEntry<uint32_t>(&data.x1010[5], 3);
                ObjectEntry<uint32_t> x1010sub7 = ObjectEntry<uint32_t>(&data.x1010[6], 3);
                ObjectEntry<uint32_t> x1010sub8 = ObjectEntry<uint32_t>(&data.x1010[7], 3);
                ObjectEntry<uint32_t> x1010sub9 = ObjectEntry<uint32_t>(&data.x1010[8], 3);
                ObjectEntry<uint32_t> x1010sub10 = ObjectEntry<uint32_t>(&data.x1010[9], 3);
                ObjectEntry<uint32_t> x1010sub11 = ObjectEntry<uint32_t>(&data.x1010[10], 3);
                ObjectEntry<uint32_t> x1010sub12 = ObjectEntry<uint32_t>(&data.x1010[11], 3);
                ObjectEntry<uint32_t> x1010sub13 = ObjectEntry<uint32_t>(&data.x1010[12], 3);
                const ObjectEntryBase *x1010[14] = {&x1010sub0, &x1010sub1, &x1010sub2, &x1010sub3, &x1010sub4, &x1010sub5, &x1010sub6, &x1010sub7, &x1010sub8, &x1010sub9, &x1010sub10, &x1010sub11, &x1010sub12, &x1010sub13};
                ObjectEntry<uint8_t> x1011sub0 = ObjectEntry<uint8_t>(&data.x1011sub0, 1);
                ObjectEntry<uint32_t> x1011sub1 = ObjectEntry<uint32_t>(&data.x1011[0], 3);
                ObjectEntry<uint32_t> x1011sub2 = ObjectEntry<uint32_t>(&data.x1011[1], 3);
                ObjectEntry<uint32_t> x1011sub3 = ObjectEntry<uint32_t>(&data.x1011[2], 3);
                ObjectEntry<uint32_t> x1011sub4 = ObjectEntry<uint32_t>(&data.x1011[3], 3);
                ObjectEntry<uint32_t> x1011sub5 = ObjectEntry<uint32_t>(&data.x1011[4], 3);
                ObjectEntry<uint32_t> x1011sub6 = ObjectEntry<uint32_t>(&data.x1011[5], 3);
                ObjectEntry<uint32_t> x1011sub7 = ObjectEntry<uint32_t>(&data.x1011[6], 3);
                ObjectEntry<uint32_t> x1011sub8 = ObjectEntry<uint32_t>(&data.x1011[7], 3);
                ObjectEntry<uint32_t> x1011sub9 = ObjectEntry<uint32_t>(&data.x1011[8], 3);
                ObjectEntry<uint32_t> x1011sub10 = ObjectEntry<uint32_t>(&data.x1011[9], 3);
                ObjectEntry<uint32_t> x1011sub11 = ObjectEntry<uint32_t>(&data.x1011[10], 3);
                ObjectEntry<uint32_t> x1011sub12 = ObjectEntry<uint32_t>(&data.x1011[11], 3);
                ObjectEntry<uint32_t> x1011sub13 = ObjectEntry<uint32_t>(&data.x1011[12], 3);
                const ObjectEntryBase *x1011[14] = {&x1011sub0, &x1011sub1, &x1011sub2, &x1011sub3, &x1011sub4, &x1011sub5, &x1011sub6, &x1011sub7, &x1011sub8, &x1011sub9, &x1011sub10, &x1011sub11, &x1011sub12, &x1011sub13};
                ObjectEntry<uint32_t> x1014sub0 = ObjectEntry<uint32_t>(&data.x1014, 3);
                const ObjectEntryBase *x1014[1] = {&x1014sub0};
                ObjectEntry<uint8_t> x1016sub0 = ObjectEntry<uint8_t>(&data.x1016sub0, 1);
                ObjectEntry<uint32_t> x1016sub1 = ObjectEntry<uint32_t>(&data.x1016[0], 3);
                const ObjectEntryBase *x1016[2] = {&x1016sub0, &x1016sub1};
                ObjectEntry<uint16_t> x1017sub0 = ObjectEntry<uint16_t>(&data.x1017, 3);
                const ObjectEntryBase *x1017[1] = {&x1017sub0};
                ObjectEntry<uint8_t> x1018sub0 = ObjectEntry<uint8_t>(&data.x1018.sub0, 1);
                ObjectEntry<uint32_t> x1018sub1 = ObjectEntry<uint32_t>(&data.x1018.sub1, 1);
                ObjectEntry<uint32_t> x1018sub2 = ObjectEntry<uint32_t>(&data.x1018.sub2, 1);
                ObjectEntry<uint32_t> x1018sub3 = ObjectEntry<uint32_t>(&data.x1018.sub3, 1);
                ObjectEntry<uint32_t> x1018sub4 = ObjectEntry<uint32_t>(&data.x1018.sub4, 1);
                const ObjectEntryBase *x1018[5] = {&x1018sub0, &x1018sub1, &x1018sub2, &x1018sub3, &x1018sub4};
                ObjectEntry<uint8_t> x1019sub0 = ObjectEntry<uint8_t>(&data.x1019, 3);
                const ObjectEntryBase *x1019[1] = {&x1019sub0};
                ObjectEntry<uint8_t> x1020sub0 = ObjectEntry<uint8_t>(&data.x1020sub0, 1);
                ObjectEntry<uint32_t> x1020sub1 = ObjectEntry<uint32_t>(&data.x1020[0], 3);
                ObjectEntry<uint32_t> x1020sub2 = ObjectEntry<uint32_t>(&data.x1020[1], 3);
                const ObjectEntryBase *x1020[3] = {&x1020sub0, &x1020sub1, &x1020sub2};
                ObjectEntry<uint8_t> x1029sub0 = ObjectEntry<uint8_t>(&data.x1029sub0, 1);
                ObjectEntry<uint8_t> x1029sub1 = ObjectEntry<uint8_t>(&data.x1029[0], 3);
                ObjectEntry<uint8_t> x1029sub2 = ObjectEntry<uint8_t>(&data.x1029[1], 3);
                const ObjectEntryBase *x1029[3] = {&x1029sub0, &x1029sub1, &x1029sub2};
                ObjectEntry<uint8_t> x1400sub0 = ObjectEntry<uint8_t>(&data.x1400.sub0, 1);
                ObjectEntry<uint32_t> x1400sub1 = ObjectEntry<uint32_t>(&data.x1400.sub1, 3);
                ObjectEntry<uint8_t> x1400sub2 = ObjectEntry<uint8_t>(&data.x1400.sub2, 3);
                ObjectEntry<uint16_t> x1400sub3 = ObjectEntry<uint16_t>(&data.x1400.sub3, 3);
                ObjectEntry<uint8_t> x1400sub4 = ObjectEntry<uint8_t>(&data.x1400.sub4, 3);
                ObjectEntry<uint16_t> x1400sub5 = ObjectEntry<uint16_t>(&data.x1400.sub5, 3);
                const ObjectEntryBase *x1400[6] = {&x1400sub0, &x1400sub1, &x1400sub2, &x1400sub3, &x1400sub4, &x1400sub5};
                ObjectEntry<uint8_t> x1401sub0 = ObjectEntry<uint8_t>(&data.x1401.sub0, 1);
                ObjectEntry<uint32_t> x1401sub1 = ObjectEntry<uint32_t>(&data.x1401.sub1, 3);
                ObjectEntry<uint8_t> x1401sub2 = ObjectEntry<uint8_t>(&data.x1401.sub2, 3);
                const ObjectEntryBase *x1401[3] = {&x1401sub0, &x1401sub1, &x1401sub2};
                ObjectEntry<uint8_t> x1402sub0 = ObjectEntry<uint8_t>(&data.x1402.sub0, 1);
                ObjectEntry<uint32_t> x1402sub1 = ObjectEntry<uint32_t>(&data.x1402.sub1, 3);
                ObjectEntry<uint8_t> x1402sub2 = ObjectEntry<uint8_t>(&data.x1402.sub2, 3);
                const ObjectEntryBase *x1402[3] = {&x1402sub0, &x1402sub1, &x1402sub2};
                ObjectEntry<uint8_t> x1403sub0 = ObjectEntry<uint8_t>(&data.x1403.sub0, 1);
                ObjectEntry<uint32_t> x1403sub1 = ObjectEntry<uint32_t>(&data.x1403.sub1, 3);
                ObjectEntry<uint8_t> x1403sub2 = ObjectEntry<uint8_t>(&data.x1403.sub2, 3);
                const ObjectEntryBase *x1403[3] = {&x1403sub0, &x1403sub1, &x1403sub2};
                ObjectEntry<uint8_t> x1600sub0 = ObjectEntry<uint8_t>(&data.x1600.sub0, 3);
                ObjectEntry<uint32_t> x1600sub1 = ObjectEntry<uint32_t>(&data.x1600.sub1, 3);
                ObjectEntry<uint32_t> x1600sub2 = ObjectEntry<uint32_t>(&data.x1600.sub2, 3);
                ObjectEntry<uint32_t> x1600sub3 = ObjectEntry<uint32_t>(&data.x1600.sub3, 3);
                ObjectEntry<uint32_t> x1600sub4 = ObjectEntry<uint32_t>(&data.x1600.sub4, 3);
                ObjectEntry<uint32_t> x1600sub5 = ObjectEntry<uint32_t>(&data.x1600.sub5, 3);
                ObjectEntry<uint32_t> x1600sub6 = ObjectEntry<uint32_t>(&data.x1600.sub6, 3);
                ObjectEntry<uint32_t> x1600sub7 = ObjectEntry<uint32_t>(&data.x1600.sub7, 3);
                ObjectEntry<uint32_t> x1600sub8 = ObjectEntry<uint32_t>(&data.x1600.sub8, 3);
                const ObjectEntryBase *x1600[9] = {&x1600sub0, &x1600sub1, &x1600sub2, &x1600sub3, &x1600sub4, &x1600sub5, &x1600sub6, &x1600sub7, &x1600sub8};
                ObjectEntry<uint8_t> x1601sub0 = ObjectEntry<uint8_t>(&data.x1601.sub0, 3);
                ObjectEntry<uint32_t> x1601sub1 = ObjectEntry<uint32_t>(&data.x1601.sub1, 3);
                ObjectEntry<uint32_t> x1601sub2 = ObjectEntry<uint32_t>(&data.x1601.sub2, 3);
                ObjectEntry<uint32_t> x1601sub3 = ObjectEntry<uint32_t>(&data.x1601.sub3, 3);
                ObjectEntry<uint32_t> x1601sub4 = ObjectEntry<uint32_t>(&data.x1601.sub4, 3);
                ObjectEntry<uint32_t> x1601sub5 = ObjectEntry<uint32_t>(&data.x1601.sub5, 3);
                ObjectEntry<uint32_t> x1601sub6 = ObjectEntry<uint32_t>(&data.x1601.sub6, 3);
                ObjectEntry<uint32_t> x1601sub7 = ObjectEntry<uint32_t>(&data.x1601.sub7, 3);
                ObjectEntry<uint32_t> x1601sub8 = ObjectEntry<uint32_t>(&data.x1601.sub8, 3);
                const ObjectEntryBase *x1601[9] = {&x1601sub0, &x1601sub1, &x1601sub2, &x1601sub3, &x1601sub4, &x1601sub5, &x1601sub6, &x1601sub7, &x1601sub8};
                ObjectEntry<uint8_t> x1602sub0 = ObjectEntry<uint8_t>(&data.x1602.sub0, 3);
                ObjectEntry<uint32_t> x1602sub1 = ObjectEntry<uint32_t>(&data.x1602.sub1, 3);
                ObjectEntry<uint32_t> x1602sub2 = ObjectEntry<uint32_t>(&data.x1602.sub2, 3);
                ObjectEntry<uint32_t> x1602sub3 = ObjectEntry<uint32_t>(&data.x1602.sub3, 3);
                ObjectEntry<uint32_t> x1602sub4 = ObjectEntry<uint32_t>(&data.x1602.sub4, 3);
                ObjectEntry<uint32_t> x1602sub5 = ObjectEntry<uint32_t>(&data.x1602.sub5, 3);
                ObjectEntry<uint32_t> x1602sub6 = ObjectEntry<uint32_t>(&data.x1602.sub6, 3);
                ObjectEntry<uint32_t> x1602sub7 = ObjectEntry<uint32_t>(&data.x1602.sub7, 3);
                ObjectEntry<uint32_t> x1602sub8 = ObjectEntry<uint32_t>(&data.x1602.sub8, 3);
                const ObjectEntryBase *x1602[9] = {&x1602sub0, &x1602sub1, &x1602sub2, &x1602sub3, &x1602sub4, &x1602sub5, &x1602sub6, &x1602sub7, &x1602sub8};
                ObjectEntry<uint8_t> x1603sub0 = ObjectEntry<uint8_t>(&data.x1603.sub0, 3);
                ObjectEntry<uint32_t> x1603sub1 = ObjectEntry<uint32_t>(&data.x1603.sub1, 3);
                ObjectEntry<uint32_t> x1603sub2 = ObjectEntry<uint32_t>(&data.x1603.sub2, 3);
                ObjectEntry<uint32_t> x1603sub3 = ObjectEntry<uint32_t>(&data.x1603.sub3, 3);
                ObjectEntry<uint32_t> x1603sub4 = ObjectEntry<uint32_t>(&data.x1603.sub4, 3);
                ObjectEntry<uint32_t> x1603sub5 = ObjectEntry<uint32_t>(&data.x1603.sub5, 3);
                ObjectEntry<uint32_t> x1603sub6 = ObjectEntry<uint32_t>(&data.x1603.sub6, 3);
                ObjectEntry<uint32_t> x1603sub7 = ObjectEntry<uint32_t>(&data.x1603.sub7, 3);
                ObjectEntry<uint32_t> x1603sub8 = ObjectEntry<uint32_t>(&data.x1603.sub8, 3);
                const ObjectEntryBase *x1603[9] = {&x1603sub0, &x1603sub1, &x1603sub2, &x1603sub3, &x1603sub4, &x1603sub5, &x1603sub6, &x1603sub7, &x1603sub8};
                ObjectEntry<uint8_t> x1800sub0 = ObjectEntry<uint8_t>(&data.x1800.sub0, 1);
                ObjectEntry<uint32_t> x1800sub1 = ObjectEntry<uint32_t>(&data.x1800.sub1, 3);
                ObjectEntry<uint8_t> x1800sub2 = ObjectEntry<uint8_t>(&data.x1800.sub2, 3);
                ObjectEntry<uint16_t> x1800sub3 = ObjectEntry<uint16_t>(&data.x1800.sub3, 3);
                ObjectEntry<uint8_t> x1800sub4 = ObjectEntry<uint8_t>(&data.x1800.sub4, 3);
                ObjectEntry<uint16_t> x1800sub5 = ObjectEntry<uint16_t>(&data.x1800.sub5, 3);
                ObjectEntry<uint8_t> x1800sub6 = ObjectEntry<uint8_t>(&data.x1800.sub6, 3);
                const ObjectEntryBase *x1800[7] = {&x1800sub0, &x1800sub1, &x1800sub2, &x1800sub3, &x1800sub4, &x1800sub5, &x1800sub6};
                ObjectEntry<uint8_t> x1801sub0 = ObjectEntry<uint8_t>(&data.x1801.sub0, 1);
                ObjectEntry<uint32_t> x1801sub1 = ObjectEntry<uint32_t>(&data.x1801.sub1, 3);
                ObjectEntry<uint8_t> x1801sub2 = ObjectEntry<uint8_t>(&data.x1801.sub2, 3);
                ObjectEntry<uint16_t> x1801sub3 = ObjectEntry<uint16_t>(&data.x1801.sub3, 3);
                ObjectEntry<uint8_t> x1801sub4 = ObjectEntry<uint8_t>(&data.x1801.sub4, 3);
                ObjectEntry<uint16_t> x1801sub5 = ObjectEntry<uint16_t>(&data.x1801.sub5, 3);
                ObjectEntry<uint8_t> x1801sub6 = ObjectEntry<uint8_t>(&data.x1801.sub6, 3);
                const ObjectEntryBase *x1801[7] = {&x1801sub0, &x1801sub1, &x1801sub2, &x1801sub3, &x1801sub4, &x1801sub5, &x1801sub6};
                ObjectEntry<uint8_t> x1802sub0 = ObjectEntry<uint8_t>(&data.x1802.sub0, 1);
                ObjectEntry<uint32_t> x1802sub1 = ObjectEntry<uint32_t>(&data.x1802.sub1, 3);
                ObjectEntry<uint8_t> x1802sub2 = ObjectEntry<uint8_t>(&data.x1802.sub2, 3);
                ObjectEntry<uint16_t> x1802sub3 = ObjectEntry<uint16_t>(&data.x1802.sub3, 3);
                ObjectEntry<uint8_t> x1802sub4 = ObjectEntry<uint8_t>(&data.x1802.sub4, 3);
                ObjectEntry<uint16_t> x1802sub5 = ObjectEntry<uint16_t>(&data.x1802.sub5, 3);
                ObjectEntry<uint8_t> x1802sub6 = ObjectEntry<uint8_t>(&data.x1802.sub6, 3);
                const ObjectEntryBase *x1802[7] = {&x1802sub0, &x1802sub1, &x1802sub2, &x1802sub3, &x1802sub4, &x1802sub5, &x1802sub6};
                ObjectEntry<uint8_t> x1803sub0 = ObjectEntry<uint8_t>(&data.x1803.sub0, 1);
                ObjectEntry<uint32_t> x1803sub1 = ObjectEntry<uint32_t>(&data.x1803.sub1, 3);
                ObjectEntry<uint8_t> x1803sub2 = ObjectEntry<uint8_t>(&data.x1803.sub2, 3);
                ObjectEntry<uint16_t> x1803sub3 = ObjectEntry<uint16_t>(&data.x1803.sub3, 3);
                ObjectEntry<uint8_t> x1803sub4 = ObjectEntry<uint8_t>(&data.x1803.sub4, 3);
                ObjectEntry<uint16_t> x1803sub5 = ObjectEntry<uint16_t>(&data.x1803.sub5, 3);
                ObjectEntry<uint8_t> x1803sub6 = ObjectEntry<uint8_t>(&data.x1803.sub6, 3);
                const ObjectEntryBase *x1803[7] = {&x1803sub0, &x1803sub1, &x1803sub2, &x1803sub3, &x1803sub4, &x1803sub5, &x1803sub6};
                ObjectEntry<uint8_t> x1A00sub0 = ObjectEntry<uint8_t>(&data.x1A00.sub0, 3);
                ObjectEntry<uint32_t> x1A00sub1 = ObjectEntry<uint32_t>(&data.x1A00.sub1, 3);
                ObjectEntry<uint32_t> x1A00sub2 = ObjectEntry<uint32_t>(&data.x1A00.sub2, 3);
                ObjectEntry<uint32_t> x1A00sub3 = ObjectEntry<uint32_t>(&data.x1A00.sub3, 3);
                ObjectEntry<uint32_t> x1A00sub4 = ObjectEntry<uint32_t>(&data.x1A00.sub4, 3);
                ObjectEntry<uint32_t> x1A00sub5 = ObjectEntry<uint32_t>(&data.x1A00.sub5, 3);
                ObjectEntry<uint32_t> x1A00sub6 = ObjectEntry<uint32_t>(&data.x1A00.sub6, 3);
                ObjectEntry<uint32_t> x1A00sub7 = ObjectEntry<uint32_t>(&data.x1A00.sub7, 3);
                ObjectEntry<uint32_t> x1A00sub8 = ObjectEntry<uint32_t>(&data.x1A00.sub8, 3);
                const ObjectEntryBase *x1A00[9] = {&x1A00sub0, &x1A00sub1, &x1A00sub2, &x1A00sub3, &x1A00sub4, &x1A00sub5, &x1A00sub6, &x1A00sub7, &x1A00sub8};
                ObjectEntry<uint8_t> x1A01sub0 = ObjectEntry<uint8_t>(&data.x1A01.sub0, 3);
                ObjectEntry<uint32_t> x1A01sub1 = ObjectEntry<uint32_t>(&data.x1A01.sub1, 3);
                ObjectEntry<uint32_t> x1A01sub2 = ObjectEntry<uint32_t>(&data.x1A01.sub2, 3);
                ObjectEntry<uint32_t> x1A01sub3 = ObjectEntry<uint32_t>(&data.x1A01.sub3, 3);
                ObjectEntry<uint32_t> x1A01sub4 = ObjectEntry<uint32_t>(&data.x1A01.sub4, 3);
                ObjectEntry<uint32_t> x1A01sub5 = ObjectEntry<uint32_t>(&data.x1A01.sub5, 3);
                ObjectEntry<uint32_t> x1A01sub6 = ObjectEntry<uint32_t>(&data.x1A01.sub6, 3);
                ObjectEntry<uint32_t> x1A01sub7 = ObjectEntry<uint32_t>(&data.x1A01.sub7, 3);
                ObjectEntry<uint32_t> x1A01sub8 = ObjectEntry<uint32_t>(&data.x1A01.sub8, 3);
                const ObjectEntryBase *x1A01[9] = {&x1A01sub0, &x1A01sub1, &x1A01sub2, &x1A01sub3, &x1A01sub4, &x1A01sub5, &x1A01sub6, &x1A01sub7, &x1A01sub8};
                ObjectEntry<uint8_t> x1A02sub0 = ObjectEntry<uint8_t>(&data.x1A02.sub0, 3);
                ObjectEntry<uint32_t> x1A02sub1 = ObjectEntry<uint32_t>(&data.x1A02.sub1, 3);
                ObjectEntry<uint32_t> x1A02sub2 = ObjectEntry<uint32_t>(&data.x1A02.sub2, 3);
                ObjectEntry<uint32_t> x1A02sub3 = ObjectEntry<uint32_t>(&data.x1A02.sub3, 3);
                ObjectEntry<uint32_t> x1A02sub4 = ObjectEntry<uint32_t>(&data.x1A02.sub4, 3);
                ObjectEntry<uint32_t> x1A02sub5 = ObjectEntry<uint32_t>(&data.x1A02.sub5, 3);
                ObjectEntry<uint32_t> x1A02sub6 = ObjectEntry<uint32_t>(&data.x1A02.sub6, 3);
                ObjectEntry<uint32_t> x1A02sub7 = ObjectEntry<uint32_t>(&data.x1A02.sub7, 3);
                ObjectEntry<uint32_t> x1A02sub8 = ObjectEntry<uint32_t>(&data.x1A02.sub8, 3);
                const ObjectEntryBase *x1A02[9] = {&x1A02sub0, &x1A02sub1, &x1A02sub2, &x1A02sub3, &x1A02sub4, &x1A02sub5, &x1A02sub6, &x1A02sub7, &x1A02sub8};
                ObjectEntry<uint8_t> x1A03sub0 = ObjectEntry<uint8_t>(&data.x1A03.sub0, 3);
                ObjectEntry<uint32_t> x1A03sub1 = ObjectEntry<uint32_t>(&data.x1A03.sub1, 3);
                ObjectEntry<uint32_t> x1A03sub2 = ObjectEntry<uint32_t>(&data.x1A03.sub2, 3);
                ObjectEntry<uint32_t> x1A03sub3 = ObjectEntry<uint32_t>(&data.x1A03.sub3, 3);
                ObjectEntry<uint32_t> x1A03sub4 = ObjectEntry<uint32_t>(&data.x1A03.sub4, 3);
                ObjectEntry<uint32_t> x1A03sub5 = ObjectEntry<uint32_t>(&data.x1A03.sub5, 3);
                ObjectEntry<uint32_t> x1A03sub6 = ObjectEntry<uint32_t>(&data.x1A03.sub6, 3);
                ObjectEntry<uint32_t> x1A03sub7 = ObjectEntry<uint32_t>(&data.x1A03.sub7, 3);
                ObjectEntry<uint32_t> x1A03sub8 = ObjectEntry<uint32_t>(&data.x1A03.sub8, 3);
                const ObjectEntryBase *x1A03[9] = {&x1A03sub0, &x1A03sub1, &x1A03sub2, &x1A03sub3, &x1A03sub4, &x1A03sub5, &x1A03sub6, &x1A03sub7, &x1A03sub8};
                ObjectEntry<uint8_t[974]> x1F51sub0 = ObjectEntry<uint8_t[974]>(&data.x1F51, 3);
                const ObjectEntryBase *x1F51[1] = {&x1F51sub0};
                ObjectEntry<uint8_t> x1F57sub0 = ObjectEntry<uint8_t>(&data.x1F57sub0, 1);
                ObjectEntry<uint32_t> x1F57sub1 = ObjectEntry<uint32_t>(&data.x1F57[0], 1);
                ObjectEntry<uint32_t> x1F57sub2 = ObjectEntry<uint32_t>(&data.x1F57[1], 1);
                const ObjectEntryBase *x1F57[3] = {&x1F57sub0, &x1F57sub1, &x1F57sub2};
                ObjectEntry<uint32_t> x1F80sub0 = ObjectEntry<uint32_t>(&data.x1F80, 3);
                const ObjectEntryBase *x1F80[1] = {&x1F80sub0};
                ObjectEntry<int16_t> x6007sub0 = ObjectEntry<int16_t>(&data.x6007, 3);
                const ObjectEntryBase *x6007[1] = {&x6007sub0};
                ObjectEntry<uint16_t> x603Fsub0 = ObjectEntry<uint16_t>(&data.x603F, 5);
                const ObjectEntryBase *x603F[1] = {&x603Fsub0};
                ObjectEntry<uint16_t> x6040sub0 = ObjectEntry<uint16_t>(&data.x6040, 7);
                const ObjectEntryBase *x6040[1] = {&x6040sub0};
                ObjectEntry<uint16_t> x6041sub0 = ObjectEntry<uint16_t>(&data.x6041, 5);
                const ObjectEntryBase *x6041[1] = {&x6041sub0};
                ObjectEntry<int16_t> x6042sub0 = ObjectEntry<int16_t>(&data.x6042, 7);
                const ObjectEntryBase *x6042[1] = {&x6042sub0};
                ObjectEntry<int16_t> x6043sub0 = ObjectEntry<int16_t>(&data.x6043, 5);
                const ObjectEntryBase *x6043[1] = {&x6043sub0};
                ObjectEntry<int16_t> x6044sub0 = ObjectEntry<int16_t>(&data.x6044, 5);
                const ObjectEntryBase *x6044[1] = {&x6044sub0};
                ObjectEntry<uint8_t> x6046sub0 = ObjectEntry<uint8_t>(&data.x6046sub0, 1);
                ObjectEntry<uint32_t> x6046sub1 = ObjectEntry<uint32_t>(&data.x6046[0], 7);
                ObjectEntry<uint32_t> x6046sub2 = ObjectEntry<uint32_t>(&data.x6046[1], 7);
                const ObjectEntryBase *x6046[3] = {&x6046sub0, &x6046sub1, &x6046sub2};
                ObjectEntry<uint8_t> x6048sub0 = ObjectEntry<uint8_t>(&data.x6048.sub0, 1);
                ObjectEntry<double> x6048sub1 = ObjectEntry<double>(&data.x6048.sub1, 7);
                ObjectEntry<float> x6048sub2 = ObjectEntry<float>(&data.x6048.sub2, 7);
                const ObjectEntryBase *x6048[3] = {&x6048sub0, &x6048sub1, &x6048sub2};
                ObjectEntry<uint8_t> x6049sub0 = ObjectEntry<uint8_t>(&data.x6049.sub0, 1);
                ObjectEntry<uint32_t> x6049sub1 = ObjectEntry<uint32_t>(&data.x6049.sub1, 5);
                ObjectEntry<uint16_t> x6049sub2 = ObjectEntry<uint16_t>(&data.x6049.sub2, 6);
                const ObjectEntryBase *x6049[3] = {&x6049sub0, &x6049sub1, &x6049sub2};
                ObjectEntry<uint8_t> x604Asub0 = ObjectEntry<uint8_t>(&data.x604A.sub0, 1);
                ObjectEntry<uint64_t> x604Asub1 = ObjectEntry<uint64_t>(&data.x604A.sub1, 7);
                ObjectEntry<uint64_t> x604Asub2 = ObjectEntry<uint64_t>(&data.x604A.sub2, 7);
                const ObjectEntryBase *x604A[3] = {&x604Asub0, &x604Asub1, &x604Asub2};
                ObjectEntry<int8_t> x6060sub0 = ObjectEntry<int8_t>(&data.x6060, 7);
                const ObjectEntryBase *x6060[1] = {&x6060sub0};
                ObjectEntry<int8_t> x6061sub0 = ObjectEntry<int8_t>(&data.x6061, 5);
                const ObjectEntryBase *x6061[1] = {&x6061sub0};
                ObjectEntry<int32_t> x6064sub0 = ObjectEntry<int32_t>(&data.x6064, 5);
                const ObjectEntryBase *x6064[1] = {&x6064sub0};
                LimitedObjectEntry<int32_t> x607Asub0 = LimitedObjectEntry<int32_t>(&data.x607A, 7, -5000, 5000);
                const ObjectEntryBase *x607A[1] = {&x607Asub0};
                ObjectEntry<uint32_t> x6081sub0 = ObjectEntry<uint32_t>(&data.x6081, 7);
                const ObjectEntryBase *x6081[1] = {&x6081sub0};
                ObjectEntry<uint32_t> x60FDsub0 = ObjectEntry<uint32_t>(&data.x60FD, 5);
                const ObjectEntryBase *x60FD[1] = {&x60FDsub0};
                ObjectEntry<uint8_t> x60FEsub0 = ObjectEntry<uint8_t>(&data.x60FEsub0, 1);
                ObjectEntry<uint32_t> x60FEsub1 = ObjectEntry<uint32_t>(&data.x60FE[0], 7);
                const ObjectEntryBase *x60FE[2] = {&x60FEsub0, &x60FEsub1};
            } entries;
            Object x2 = Object(2, 1, entries.x2);
            Object x3 = Object(3, 1, entries.x3);
            Object x4 = Object(4, 1, entries.x4);
            Object x5 = Object(5, 1, entries.x5);
            Object x6 = Object(6, 1, entries.x6);
            Object x7 = Object(7, 1, entries.x7);
            Object x1000 = Object(4096, 1, entries.x1000);
            Object1001 x1001 = Object1001(4097, 1, entries.x1001);
            Object1003 x1003 = Object1003(4099, 9, entries.x1003);
            Object x1005 = Object(4101, 1, entries.x1005);
            Object x1006 = Object(4102, 1, entries.x1006);
            Object x1007 = Object(4103, 1, entries.x1007);
            Object x1008 = Object(4104, 1, entries.x1008);
            Object x1009 = Object(4105, 1, entries.x1009);
            Object x100A = Object(4106, 1, entries.x100A);
            Object x100C = Object(4108, 1, entries.x100C);
            Object x100D = Object(4109, 1, entries.x100D);
            Object1010 x1010 = Object1010(4112, 14, entries.x1010);
            Object1011 x1011 = Object1011(4113, 14, entries.x1011);
            Object x1014 = Object(4116, 1, entries.x1014);
            Object x1016 = Object(4118, 2, entries.x1016);
            Object x1017 = Object(4119, 1, entries.x1017);
            Object x1018 = Object(4120, 5, entries.x1018);
            Object1019 x1019 = Object1019(4121, 1, entries.x1019);
            Object x1020 = Object(4128, 3, entries.x1020);
            Object x1029 = Object(4137, 3, entries.x1029);
            Object1400 x1400 = Object1400(5120, 6, entries.x1400);
            Object1400 x1401 = Object1400(5121, 3, entries.x1401);
            Object1400 x1402 = Object1400(5122, 3, entries.x1402);
            Object1400 x1403 = Object1400(5123, 3, entries.x1403);
            Object1600 x1600 = Object1600(5632, 9, entries.x1600);
            Object1600 x1601 = Object1600(5633, 9, entries.x1601);
            Object1600 x1602 = Object1600(5634, 9, entries.x1602);
            Object1600 x1603 = Object1600(5635, 9, entries.x1603);
            Object1800 x1800 = Object1800(6144, 7, entries.x1800);
            Object1800 x1801 = Object1800(6145, 7, entries.x1801);
            Object1800 x1802 = Object1800(6146, 7, entries.x1802);
            Object1800 x1803 = Object1800(6147, 7, entries.x1803);
            Object1A00 x1A00 = Object1A00(6656, 9, entries.x1A00);
            Object1A00 x1A01 = Object1A00(6657, 9, entries.x1A01);
            Object1A00 x1A02 = Object1A00(6658, 9, entries.x1A02);
            Object1A00 x1A03 = Object1A00(6659, 9, entries.x1A03);
            Object x1F51 = Object(8017, 1, entries.x1F51);
            Object x1F57 = Object(8023, 3, entries.x1F57);
            Object x1F80 = Object(8064, 1, entries.x1F80);
            Object x6007 = Object(24583, 1, entries.x6007);
            Object x603F = Object(24639, 1, entries.x603F);
            Object x6040 = Object(24640, 1, entries.x6040);
            Object x6041 = Object(24641, 1, entries.x6041);
            Object x6042 = Object(24642, 1, entries.x6042);
            Object x6043 = Object(24643, 1, entries.x6043);
            Object x6044 = Object(24644, 1, entries.x6044);
            Object x6046 = Object(24646, 3, entries.x6046);
            Object x6048 = Object(24648, 3, entries.x6048);
            Object x6049 = Object(24649, 3, entries.x6049);
            Object x604A = Object(24650, 3, entries.x604A);
            Object x6060 = Object(24672, 1, entries.x6060);
            Object x6061 = Object(24673, 1, entries.x6061);
            Object x6064 = Object(24676, 1, entries.x6064);
            Object x607A = Object(24698, 1, entries.x607A);
            Object x6081 = Object(24705, 1, entries.x6081);
            Object x60FD = Object(24829, 1, entries.x60FD);
            Object x60FE = Object(24830, 2, entries.x60FE);
        } objects;
        Object *objectsArray[OD_OBJECTS_COUNT] = {&objects.x2, &objects.x3, &objects.x4, &objects.x5, &objects.x6, &objects.x7, &objects.x1000, &objects.x1001, &objects.x1003, &objects.x1005, &objects.x1006, &objects.x1007, &objects.x1008, &objects.x1009, &objects.x100A, &objects.x100C, &objects.x100D, &objects.x1010, &objects.x1011, &objects.x1014, &objects.x1016, &objects.x1017, &objects.x1018, &objects.x1019, &objects.x1020, &objects.x1029, &objects.x1400, &objects.x1401, &objects.x1402, &objects.x1403, &objects.x1600, &objects.x1601, &objects.x1602, &objects.x1603, &objects.x1800, &objects.x1801, &objects.x1802, &objects.x1803, &objects.x1A00, &objects.x1A01, &objects.x1A02, &objects.x1A03, &objects.x1F51, &objects.x1F57, &objects.x1F80, &objects.x6007, &objects.x603F, &objects.x6040, &objects.x6041, &objects.x6042, &objects.x6043, &objects.x6044, &objects.x6046, &objects.x6048, &objects.x6049, &objects.x604A, &objects.x6060, &objects.x6061, &objects.x6064, &objects.x607A, &objects.x6081, &objects.x60FD, &objects.x60FE};
        class Node &node;
#ifdef OD_DEFAULT_DATA
        const ObjectDictionnaryData defaultData;
#endif

    public:
        const uint16_t length = OD_OBJECTS_COUNT;

        ObjectDictionnary(class Node &node);
        Object *findObject(uint16_t index);
        Object *at(uint16_t index);
        Object *operator[](uint16_t index);
        bool saveData(uint8_t parameterGroup);
        bool loadData(uint8_t parameterGroup);
        bool restoreData(uint8_t parameterGroup);
    };
}
