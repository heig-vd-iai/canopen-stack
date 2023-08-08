/******************************************************************************
 * [Filename]:      od.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          Generated on August 8, 2023
 * [Description]:   This file contains the declaration of the ObjectDictionnary class.
 *                  It is auto-generated from the device's EDS file, so manually editing is not recommended.
 *****************************************************************************/
#pragma once
#include "od_include.hpp"
#define OD_OBJECTS_COUNT 63
#define OD_TPDO_COUNT 4
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
        struct 
        {
            struct
            {
                struct
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
                uint32_t x1014 = 132;
                uint8_t x1016sub0 = 1;
                uint32_t x1016[1] = {0};
                uint16_t x1017 = 0;
                struct {uint8_t sub0 = 4; uint32_t sub1 = 620; uint32_t sub2 = 70; uint32_t sub3 = 145031168; uint32_t sub4 = 0;} x1018;
                uint8_t x1019 = 0;
                uint8_t x1020sub0 = 2;
                uint32_t x1020[2] = {0, 0};
                uint8_t x1029sub0 = 2;
                uint8_t x1029[2] = {0, 1};
                struct {uint8_t sub0 = 2; uint32_t sub1 = 516; uint8_t sub2 = 255;} x1400;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 772; uint8_t sub2 = 255;} x1401;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 1028; uint8_t sub2 = 255;} x1402;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 1284; uint8_t sub2 = 255;} x1403;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 1614807056; uint32_t sub2 = 1616904200; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1600;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 1618608160; uint32_t sub2 = 1619066912; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1601;
                struct {uint8_t sub0 = 1; uint32_t sub1 = 1614938128; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1602;
                struct {uint8_t sub0 = 1; uint32_t sub1 = 1627259168; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1603;
                struct {uint8_t sub0 = 6; uint32_t sub1 = 388; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1800;
                struct {uint8_t sub0 = 6; uint32_t sub1 = 644; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1801;
                struct {uint8_t sub0 = 6; uint32_t sub1 = 900; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1802;
                struct {uint8_t sub0 = 6; uint32_t sub1 = 1156; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0; uint8_t sub6 = 0;} x1803;
                struct {uint8_t sub0 = 2; uint32_t sub1 = 1614872592; uint32_t sub2 = 1616969736; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A00;
                struct {uint8_t sub0 = 1; uint32_t sub1 = 1617166368; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A01;
                struct {uint8_t sub0 = 1; uint32_t sub1 = 1615069200; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A02;
                struct {uint8_t sub0 = 1; uint32_t sub1 = 1627193376; uint32_t sub2 = 0; uint32_t sub3 = 0; uint32_t sub4 = 0; uint32_t sub5 = 0; uint32_t sub6 = 0; uint32_t sub7 = 0; uint32_t sub8 = 0;} x1A03;
                uint8_t x1F51sub0 = 2;
                uint8_t x1F51[2] = {0, 0};
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
                } data;
                ObjectEntry x2[1] = {ObjectEntry(&data.x2, 5, 2, 1)};
                ObjectEntry x3[1] = {ObjectEntry(&data.x3, 5, 3, 2)};
                ObjectEntry x4[1] = {ObjectEntry(&data.x4, 5, 4, 4)};
                ObjectEntry x5[1] = {ObjectEntry(&data.x5, 5, 5, 1)};
                ObjectEntry x6[1] = {ObjectEntry(&data.x6, 5, 6, 2)};
                ObjectEntry x7[1] = {ObjectEntry(&data.x7, 5, 7, 4)};
                ObjectEntry x1000[1] = {ObjectEntry(&data.x1000, 1, 7, 4)};
                ObjectEntry x1001[1] = {ObjectEntry(&data.x1001, 1, 5, 1)};
                ObjectEntry x1003[9] = {ObjectEntry(&data.x1003sub0, 3, 5, 1), ObjectEntry(&data.x1003[0], 1, 7, 4), ObjectEntry(&data.x1003[1], 1, 7, 4), ObjectEntry(&data.x1003[2], 1, 7, 4), ObjectEntry(&data.x1003[3], 1, 7, 4), ObjectEntry(&data.x1003[4], 1, 7, 4), ObjectEntry(&data.x1003[5], 1, 7, 4), ObjectEntry(&data.x1003[6], 1, 7, 4), ObjectEntry(&data.x1003[7], 1, 7, 4)};
                ObjectEntry x1005[1] = {ObjectEntry(&data.x1005, 3, 7, 4)};
                ObjectEntry x1006[1] = {ObjectEntry(&data.x1006, 3, 7, 4)};
                ObjectEntry x1007[1] = {ObjectEntry(&data.x1007, 3, 7, 4)};
                ObjectEntry x1008[1] = {ObjectEntry(&data.x1008, 5, 9, 17)};
                ObjectEntry x1009[1] = {ObjectEntry(&data.x1009, 5, 9, 12)};
                ObjectEntry x100A[1] = {ObjectEntry(&data.x100A, 5, 9, 18)};
                ObjectEntry x100C[1] = {ObjectEntry(&data.x100C, 3, 6, 2)};
                ObjectEntry x100D[1] = {ObjectEntry(&data.x100D, 3, 5, 1)};
                ObjectEntry x1010[14] = {ObjectEntry(&data.x1010sub0, 1, 5, 1), ObjectEntry(&data.x1010[0], 3, 7, 4), ObjectEntry(&data.x1010[1], 3, 7, 4), ObjectEntry(&data.x1010[2], 3, 7, 4), ObjectEntry(&data.x1010[3], 3, 7, 4), ObjectEntry(&data.x1010[4], 3, 7, 4), ObjectEntry(&data.x1010[5], 3, 7, 4), ObjectEntry(&data.x1010[6], 3, 7, 4), ObjectEntry(&data.x1010[7], 3, 7, 4), ObjectEntry(&data.x1010[8], 3, 7, 4), ObjectEntry(&data.x1010[9], 3, 7, 4), ObjectEntry(&data.x1010[10], 3, 7, 4), ObjectEntry(&data.x1010[11], 3, 7, 4), ObjectEntry(&data.x1010[12], 3, 7, 4)};
                ObjectEntry x1011[14] = {ObjectEntry(&data.x1011sub0, 1, 5, 1), ObjectEntry(&data.x1011[0], 3, 7, 4), ObjectEntry(&data.x1011[1], 3, 7, 4), ObjectEntry(&data.x1011[2], 3, 7, 4), ObjectEntry(&data.x1011[3], 3, 7, 4), ObjectEntry(&data.x1011[4], 3, 7, 4), ObjectEntry(&data.x1011[5], 3, 7, 4), ObjectEntry(&data.x1011[6], 3, 7, 4), ObjectEntry(&data.x1011[7], 3, 7, 4), ObjectEntry(&data.x1011[8], 3, 7, 4), ObjectEntry(&data.x1011[9], 3, 7, 4), ObjectEntry(&data.x1011[10], 3, 7, 4), ObjectEntry(&data.x1011[11], 3, 7, 4), ObjectEntry(&data.x1011[12], 3, 7, 4)};
                ObjectEntry x1014[1] = {ObjectEntry(&data.x1014, 3, 7, 4)};
                ObjectEntry x1016[2] = {ObjectEntry(&data.x1016sub0, 1, 5, 1), ObjectEntry(&data.x1016[0], 3, 7, 4)};
                ObjectEntry x1017[1] = {ObjectEntry(&data.x1017, 3, 6, 2)};
                ObjectEntry x1018[5] = {ObjectEntry(&data.x1018.sub0, 1, 5, 1), ObjectEntry(&data.x1018.sub1, 1, 7, 4), ObjectEntry(&data.x1018.sub2, 1, 7, 4), ObjectEntry(&data.x1018.sub3, 1, 7, 4), ObjectEntry(&data.x1018.sub4, 1, 7, 4)};
                ObjectEntry x1019[1] = {ObjectEntry(&data.x1019, 3, 5, 1)};
                ObjectEntry x1020[3] = {ObjectEntry(&data.x1020sub0, 1, 5, 1), ObjectEntry(&data.x1020[0], 3, 7, 4), ObjectEntry(&data.x1020[1], 3, 7, 4)};
                ObjectEntry x1029[3] = {ObjectEntry(&data.x1029sub0, 1, 5, 1), ObjectEntry(&data.x1029[0], 3, 5, 1), ObjectEntry(&data.x1029[1], 3, 5, 1)};
                ObjectEntry x1400[3] = {ObjectEntry(&data.x1400.sub0, 1, 5, 1), ObjectEntry(&data.x1400.sub1, 3, 7, 4), ObjectEntry(&data.x1400.sub2, 3, 5, 1)};
                ObjectEntry x1401[3] = {ObjectEntry(&data.x1401.sub0, 1, 5, 1), ObjectEntry(&data.x1401.sub1, 3, 7, 4), ObjectEntry(&data.x1401.sub2, 3, 5, 1)};
                ObjectEntry x1402[3] = {ObjectEntry(&data.x1402.sub0, 1, 5, 1), ObjectEntry(&data.x1402.sub1, 3, 7, 4), ObjectEntry(&data.x1402.sub2, 3, 5, 1)};
                ObjectEntry x1403[3] = {ObjectEntry(&data.x1403.sub0, 1, 5, 1), ObjectEntry(&data.x1403.sub1, 3, 7, 4), ObjectEntry(&data.x1403.sub2, 3, 5, 1)};
                ObjectEntry x1600[9] = {ObjectEntry(&data.x1600.sub0, 3, 5, 1), ObjectEntry(&data.x1600.sub1, 3, 7, 4), ObjectEntry(&data.x1600.sub2, 3, 7, 4), ObjectEntry(&data.x1600.sub3, 3, 7, 4), ObjectEntry(&data.x1600.sub4, 3, 7, 4), ObjectEntry(&data.x1600.sub5, 3, 7, 4), ObjectEntry(&data.x1600.sub6, 3, 7, 4), ObjectEntry(&data.x1600.sub7, 3, 7, 4), ObjectEntry(&data.x1600.sub8, 3, 7, 4)};
                ObjectEntry x1601[9] = {ObjectEntry(&data.x1601.sub0, 3, 5, 1), ObjectEntry(&data.x1601.sub1, 3, 7, 4), ObjectEntry(&data.x1601.sub2, 3, 7, 4), ObjectEntry(&data.x1601.sub3, 3, 7, 4), ObjectEntry(&data.x1601.sub4, 3, 7, 4), ObjectEntry(&data.x1601.sub5, 3, 7, 4), ObjectEntry(&data.x1601.sub6, 3, 7, 4), ObjectEntry(&data.x1601.sub7, 3, 7, 4), ObjectEntry(&data.x1601.sub8, 3, 7, 4)};
                ObjectEntry x1602[9] = {ObjectEntry(&data.x1602.sub0, 3, 5, 1), ObjectEntry(&data.x1602.sub1, 3, 7, 4), ObjectEntry(&data.x1602.sub2, 3, 7, 4), ObjectEntry(&data.x1602.sub3, 3, 7, 4), ObjectEntry(&data.x1602.sub4, 3, 7, 4), ObjectEntry(&data.x1602.sub5, 3, 7, 4), ObjectEntry(&data.x1602.sub6, 3, 7, 4), ObjectEntry(&data.x1602.sub7, 3, 7, 4), ObjectEntry(&data.x1602.sub8, 3, 7, 4)};
                ObjectEntry x1603[9] = {ObjectEntry(&data.x1603.sub0, 3, 5, 1), ObjectEntry(&data.x1603.sub1, 3, 7, 4), ObjectEntry(&data.x1603.sub2, 3, 7, 4), ObjectEntry(&data.x1603.sub3, 3, 7, 4), ObjectEntry(&data.x1603.sub4, 3, 7, 4), ObjectEntry(&data.x1603.sub5, 3, 7, 4), ObjectEntry(&data.x1603.sub6, 3, 7, 4), ObjectEntry(&data.x1603.sub7, 3, 7, 4), ObjectEntry(&data.x1603.sub8, 3, 7, 4)};
                ObjectEntry x1800[7] = {ObjectEntry(&data.x1800.sub0, 1, 5, 1), ObjectEntry(&data.x1800.sub1, 5, 7, 4), ObjectEntry(&data.x1800.sub2, 3, 5, 1), ObjectEntry(&data.x1800.sub3, 3, 6, 2), ObjectEntry(&data.x1800.sub4, 3, 5, 1), ObjectEntry(&data.x1800.sub5, 3, 6, 2), ObjectEntry(&data.x1800.sub6, 3, 5, 1)};
                ObjectEntry x1801[7] = {ObjectEntry(&data.x1801.sub0, 1, 5, 1), ObjectEntry(&data.x1801.sub1, 5, 7, 4), ObjectEntry(&data.x1801.sub2, 3, 5, 1), ObjectEntry(&data.x1801.sub3, 3, 6, 2), ObjectEntry(&data.x1801.sub4, 3, 5, 1), ObjectEntry(&data.x1801.sub5, 3, 6, 2), ObjectEntry(&data.x1801.sub6, 3, 5, 1)};
                ObjectEntry x1802[7] = {ObjectEntry(&data.x1802.sub0, 1, 5, 1), ObjectEntry(&data.x1802.sub1, 5, 7, 4), ObjectEntry(&data.x1802.sub2, 3, 5, 1), ObjectEntry(&data.x1802.sub3, 3, 6, 2), ObjectEntry(&data.x1802.sub4, 3, 5, 1), ObjectEntry(&data.x1802.sub5, 3, 6, 2), ObjectEntry(&data.x1802.sub6, 3, 5, 1)};
                ObjectEntry x1803[7] = {ObjectEntry(&data.x1803.sub0, 1, 5, 1), ObjectEntry(&data.x1803.sub1, 5, 7, 4), ObjectEntry(&data.x1803.sub2, 3, 5, 1), ObjectEntry(&data.x1803.sub3, 3, 6, 2), ObjectEntry(&data.x1803.sub4, 3, 5, 1), ObjectEntry(&data.x1803.sub5, 3, 6, 2), ObjectEntry(&data.x1803.sub6, 3, 5, 1)};
                ObjectEntry x1A00[9] = {ObjectEntry(&data.x1A00.sub0, 3, 5, 1), ObjectEntry(&data.x1A00.sub1, 3, 7, 4), ObjectEntry(&data.x1A00.sub2, 3, 7, 4), ObjectEntry(&data.x1A00.sub3, 3, 7, 4), ObjectEntry(&data.x1A00.sub4, 3, 7, 4), ObjectEntry(&data.x1A00.sub5, 3, 7, 4), ObjectEntry(&data.x1A00.sub6, 3, 7, 4), ObjectEntry(&data.x1A00.sub7, 3, 7, 4), ObjectEntry(&data.x1A00.sub8, 3, 7, 4)};
                ObjectEntry x1A01[9] = {ObjectEntry(&data.x1A01.sub0, 3, 5, 1), ObjectEntry(&data.x1A01.sub1, 3, 7, 4), ObjectEntry(&data.x1A01.sub2, 3, 7, 4), ObjectEntry(&data.x1A01.sub3, 3, 7, 4), ObjectEntry(&data.x1A01.sub4, 3, 7, 4), ObjectEntry(&data.x1A01.sub5, 3, 7, 4), ObjectEntry(&data.x1A01.sub6, 3, 7, 4), ObjectEntry(&data.x1A01.sub7, 3, 7, 4), ObjectEntry(&data.x1A01.sub8, 3, 7, 4)};
                ObjectEntry x1A02[9] = {ObjectEntry(&data.x1A02.sub0, 3, 5, 1), ObjectEntry(&data.x1A02.sub1, 3, 7, 4), ObjectEntry(&data.x1A02.sub2, 3, 7, 4), ObjectEntry(&data.x1A02.sub3, 3, 7, 4), ObjectEntry(&data.x1A02.sub4, 3, 7, 4), ObjectEntry(&data.x1A02.sub5, 3, 7, 4), ObjectEntry(&data.x1A02.sub6, 3, 7, 4), ObjectEntry(&data.x1A02.sub7, 3, 7, 4), ObjectEntry(&data.x1A02.sub8, 3, 7, 4)};
                ObjectEntry x1A03[9] = {ObjectEntry(&data.x1A03.sub0, 3, 5, 1), ObjectEntry(&data.x1A03.sub1, 3, 7, 4), ObjectEntry(&data.x1A03.sub2, 3, 7, 4), ObjectEntry(&data.x1A03.sub3, 3, 7, 4), ObjectEntry(&data.x1A03.sub4, 3, 7, 4), ObjectEntry(&data.x1A03.sub5, 3, 7, 4), ObjectEntry(&data.x1A03.sub6, 3, 7, 4), ObjectEntry(&data.x1A03.sub7, 3, 7, 4), ObjectEntry(&data.x1A03.sub8, 3, 7, 4)};
                ObjectEntry x1F51[3] = {ObjectEntry(&data.x1F51sub0, 1, 5, 1), ObjectEntry(&data.x1F51[0], 3, 5, 1), ObjectEntry(&data.x1F51[1], 3, 5, 1)};
                ObjectEntry x1F57[3] = {ObjectEntry(&data.x1F57sub0, 1, 5, 1), ObjectEntry(&data.x1F57[0], 1, 7, 4), ObjectEntry(&data.x1F57[1], 1, 7, 4)};
                ObjectEntry x1F80[1] = {ObjectEntry(&data.x1F80, 3, 7, 4)};
                ObjectEntry x6007[1] = {ObjectEntry(&data.x6007, 3, 3, 2)};
                ObjectEntry x603F[1] = {ObjectEntry(&data.x603F, 1, 6, 2)};
                ObjectEntry x6040[1] = {ObjectEntry(&data.x6040, 3, 6, 2)};
                ObjectEntry x6041[1] = {ObjectEntry(&data.x6041, 1, 6, 2)};
                ObjectEntry x6042[1] = {ObjectEntry(&data.x6042, 3, 3, 2)};
                ObjectEntry x6043[1] = {ObjectEntry(&data.x6043, 1, 3, 2)};
                ObjectEntry x6044[1] = {ObjectEntry(&data.x6044, 1, 3, 2)};
                ObjectEntry x6046[3] = {ObjectEntry(&data.x6046sub0, 5, 5, 1), ObjectEntry(&data.x6046[0], 3, 7, 4), ObjectEntry(&data.x6046[1], 3, 7, 4)};
                ObjectEntry x6048[3] = {ObjectEntry(&data.x6048.sub0, 5, 5, 1), ObjectEntry(&data.x6048.sub1, 3, 17, 8), ObjectEntry(&data.x6048.sub2, 3, 8, 4)};
                ObjectEntry x6049[3] = {ObjectEntry(&data.x6049.sub0, 5, 5, 1), ObjectEntry(&data.x6049.sub1, 1, 7, 4), ObjectEntry(&data.x6049.sub2, 2, 6, 2)};
                ObjectEntry x604A[3] = {ObjectEntry(&data.x604A.sub0, 5, 5, 1), ObjectEntry(&data.x604A.sub1, 3, 27, 8), ObjectEntry(&data.x604A.sub2, 3, 27, 8)};
                ObjectEntry x6060[1] = {ObjectEntry(&data.x6060, 3, 2, 1)};
                ObjectEntry x6061[1] = {ObjectEntry(&data.x6061, 1, 2, 1)};
                ObjectEntry x6064[1] = {ObjectEntry(&data.x6064, 1, 4, 4)};
                ObjectEntry x607A[1] = {ObjectEntry(&data.x607A, 3, 4, 4)};
                ObjectEntry x6081[1] = {ObjectEntry(&data.x6081, 3, 7, 4)};
                ObjectEntry x60FD[1] = {ObjectEntry(&data.x60FD, 1, 7, 4)};
                ObjectEntry x60FE[2] = {ObjectEntry(&data.x60FEsub0, 5, 5, 1), ObjectEntry(&data.x60FE[0], 3, 7, 4)};
            } entries;
            Object x2 = Object(2, 1, 7, entries.x2);
            Object x3 = Object(3, 1, 7, entries.x3);
            Object x4 = Object(4, 1, 7, entries.x4);
            Object x5 = Object(5, 1, 7, entries.x5);
            Object x6 = Object(6, 1, 7, entries.x6);
            Object x7 = Object(7, 1, 7, entries.x7);
            Object x1000 = Object(4096, 1, 7, entries.x1000);
            Object1001 x1001 = Object1001(4097, 1, 7, entries.x1001);
            Object1003 x1003 = Object1003(4099, 9, 8, entries.x1003);
            Object x1005 = Object(4101, 1, 7, entries.x1005);
            Object x1006 = Object(4102, 1, 7, entries.x1006);
            Object x1007 = Object(4103, 1, 7, entries.x1007);
            Object x1008 = Object(4104, 1, 7, entries.x1008);
            Object x1009 = Object(4105, 1, 7, entries.x1009);
            Object x100A = Object(4106, 1, 7, entries.x100A);
            Object x100C = Object(4108, 1, 7, entries.x100C);
            Object x100D = Object(4109, 1, 7, entries.x100D);
            Object1010 x1010 = Object1010(4112, 14, 8, entries.x1010);
            Object1011 x1011 = Object1011(4113, 14, 8, entries.x1011);
            Object x1014 = Object(4116, 1, 7, entries.x1014);
            Object x1016 = Object(4118, 2, 8, entries.x1016);
            Object x1017 = Object(4119, 1, 7, entries.x1017);
            Object x1018 = Object(4120, 5, 8, entries.x1018);
            Object x1019 = Object(4121, 1, 7, entries.x1019);
            Object x1020 = Object(4128, 3, 8, entries.x1020);
            Object x1029 = Object(4137, 3, 8, entries.x1029);
            Object x1400 = Object(5120, 3, 8, entries.x1400);
            Object x1401 = Object(5121, 3, 8, entries.x1401);
            Object x1402 = Object(5122, 3, 8, entries.x1402);
            Object x1403 = Object(5123, 3, 8, entries.x1403);
            Object x1600 = Object(5632, 9, 8, entries.x1600);
            Object x1601 = Object(5633, 9, 8, entries.x1601);
            Object x1602 = Object(5634, 9, 8, entries.x1602);
            Object x1603 = Object(5635, 9, 8, entries.x1603);
            Object1800 x1800 = Object1800(6144, 7, 8, entries.x1800);
            Object1800 x1801 = Object1800(6145, 7, 8, entries.x1801);
            Object1800 x1802 = Object1800(6146, 7, 8, entries.x1802);
            Object1800 x1803 = Object1800(6147, 7, 8, entries.x1803);
            Object1A00 x1A00 = Object1A00(6656, 9, 8, entries.x1A00);
            Object1A00 x1A01 = Object1A00(6657, 9, 8, entries.x1A01);
            Object1A00 x1A02 = Object1A00(6658, 9, 8, entries.x1A02);
            Object1A00 x1A03 = Object1A00(6659, 9, 8, entries.x1A03);
            Object x1F51 = Object(8017, 3, 8, entries.x1F51);
            Object x1F57 = Object(8023, 3, 8, entries.x1F57);
            Object x1F80 = Object(8064, 1, 7, entries.x1F80);
            Object x6007 = Object(24583, 1, 7, entries.x6007);
            Object x603F = Object(24639, 1, 7, entries.x603F);
            Object x6040 = Object(24640, 1, 7, entries.x6040);
            Object x6041 = Object(24641, 1, 7, entries.x6041);
            Object x6042 = Object(24642, 1, 7, entries.x6042);
            Object x6043 = Object(24643, 1, 7, entries.x6043);
            Object x6044 = Object(24644, 1, 7, entries.x6044);
            Object x6046 = Object(24646, 3, 8, entries.x6046);
            Object x6048 = Object(24648, 3, 8, entries.x6048);
            Object x6049 = Object(24649, 3, 8, entries.x6049);
            Object x604A = Object(24650, 3, 8, entries.x604A);
            Object x6060 = Object(24672, 1, 7, entries.x6060);
            Object x6061 = Object(24673, 1, 7, entries.x6061);
            Object x6064 = Object(24676, 1, 7, entries.x6064);
            Object x607A = Object(24698, 1, 7, entries.x607A);
            Object x6081 = Object(24705, 1, 7, entries.x6081);
            Object x60FD = Object(24829, 1, 7, entries.x60FD);
            Object x60FE = Object(24830, 2, 8, entries.x60FE);
        } objects;
        const uint16_t length = OD_OBJECTS_COUNT;
        Object *objectsArray[OD_OBJECTS_COUNT] = {&objects.x2, &objects.x3, &objects.x4, &objects.x5, &objects.x6, &objects.x7, &objects.x1000, &objects.x1001, &objects.x1003, &objects.x1005, &objects.x1006, &objects.x1007, &objects.x1008, &objects.x1009, &objects.x100A, &objects.x100C, &objects.x100D, &objects.x1010, &objects.x1011, &objects.x1014, &objects.x1016, &objects.x1017, &objects.x1018, &objects.x1019, &objects.x1020, &objects.x1029, &objects.x1400, &objects.x1401, &objects.x1402, &objects.x1403, &objects.x1600, &objects.x1601, &objects.x1602, &objects.x1603, &objects.x1800, &objects.x1801, &objects.x1802, &objects.x1803, &objects.x1A00, &objects.x1A01, &objects.x1A02, &objects.x1A03, &objects.x1F51, &objects.x1F57, &objects.x1F80, &objects.x6007, &objects.x603F, &objects.x6040, &objects.x6041, &objects.x6042, &objects.x6043, &objects.x6044, &objects.x6046, &objects.x6048, &objects.x6049, &objects.x604A, &objects.x6060, &objects.x6061, &objects.x6064, &objects.x607A, &objects.x6081, &objects.x60FD, &objects.x60FE};
    public:
        Object *findObject(uint16_t index);
        Object *at(uint16_t index);
        bool saveData(uint8_t parameterGroup);
        bool loadData(uint8_t parameterGroup);
        bool restoreData(uint8_t parameterGroup);
    };
}