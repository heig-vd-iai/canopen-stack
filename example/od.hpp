/**
 * This file contains the declaration of the ObjectDictionnary class.
 * Generated on December 12, 2023
 *
 * NOTICE: Auto-generated from the device's EDS file, do not edit manually!
 */
#pragma once
#define OD_GRANULARITY 8
#define OD_PDO_MAPPING_MAX 8
#define OD_NODE_ID 1
#define OD_OBJECTS_COUNT 63
#define OD_ENTRY_SIZE_MAX 1024
#define OD_TPDO_COUNT 4
#define OD_RPDO_COUNT 4
/* These defines map an object to its dictionnary array index */
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
/* These defines map an entry to its multiplexed address (parent object index and entry subindex) */
#define OD_MUX_2_SUB_0 0x200
#define OD_MUX_3_SUB_0 0x300
#define OD_MUX_4_SUB_0 0x400
#define OD_MUX_5_SUB_0 0x500
#define OD_MUX_6_SUB_0 0x600
#define OD_MUX_7_SUB_0 0x700
#define OD_MUX_1000_SUB_0 0x100000
#define OD_MUX_1001_SUB_0 0x100100
#define OD_MUX_1003_SUB_0 0x100300
#define OD_MUX_1003_SUB_1 0x100301
#define OD_MUX_1003_SUB_2 0x100302
#define OD_MUX_1003_SUB_3 0x100303
#define OD_MUX_1003_SUB_4 0x100304
#define OD_MUX_1003_SUB_5 0x100305
#define OD_MUX_1003_SUB_6 0x100306
#define OD_MUX_1003_SUB_7 0x100307
#define OD_MUX_1003_SUB_8 0x100308
#define OD_MUX_1005_SUB_0 0x100500
#define OD_MUX_1006_SUB_0 0x100600
#define OD_MUX_1007_SUB_0 0x100700
#define OD_MUX_1008_SUB_0 0x100800
#define OD_MUX_1009_SUB_0 0x100900
#define OD_MUX_100A_SUB_0 0x100A00
#define OD_MUX_100C_SUB_0 0x100C00
#define OD_MUX_100D_SUB_0 0x100D00
#define OD_MUX_1010_SUB_0 0x101000
#define OD_MUX_1010_SUB_1 0x101001
#define OD_MUX_1010_SUB_2 0x101002
#define OD_MUX_1010_SUB_3 0x101003
#define OD_MUX_1010_SUB_4 0x101004
#define OD_MUX_1010_SUB_5 0x101005
#define OD_MUX_1010_SUB_6 0x101006
#define OD_MUX_1010_SUB_7 0x101007
#define OD_MUX_1010_SUB_8 0x101008
#define OD_MUX_1010_SUB_9 0x101009
#define OD_MUX_1010_SUB_10 0x10100A
#define OD_MUX_1010_SUB_11 0x10100B
#define OD_MUX_1010_SUB_12 0x10100C
#define OD_MUX_1010_SUB_13 0x10100D
#define OD_MUX_1011_SUB_0 0x101100
#define OD_MUX_1011_SUB_1 0x101101
#define OD_MUX_1011_SUB_2 0x101102
#define OD_MUX_1011_SUB_3 0x101103
#define OD_MUX_1011_SUB_4 0x101104
#define OD_MUX_1011_SUB_5 0x101105
#define OD_MUX_1011_SUB_6 0x101106
#define OD_MUX_1011_SUB_7 0x101107
#define OD_MUX_1011_SUB_8 0x101108
#define OD_MUX_1011_SUB_9 0x101109
#define OD_MUX_1011_SUB_10 0x10110A
#define OD_MUX_1011_SUB_11 0x10110B
#define OD_MUX_1011_SUB_12 0x10110C
#define OD_MUX_1011_SUB_13 0x10110D
#define OD_MUX_1014_SUB_0 0x101400
#define OD_MUX_1016_SUB_0 0x101600
#define OD_MUX_1016_SUB_1 0x101601
#define OD_MUX_1017_SUB_0 0x101700
#define OD_MUX_1018_SUB_0 0x101800
#define OD_MUX_1018_SUB_1 0x101801
#define OD_MUX_1018_SUB_2 0x101802
#define OD_MUX_1018_SUB_3 0x101803
#define OD_MUX_1018_SUB_4 0x101804
#define OD_MUX_1019_SUB_0 0x101900
#define OD_MUX_1020_SUB_0 0x102000
#define OD_MUX_1020_SUB_1 0x102001
#define OD_MUX_1020_SUB_2 0x102002
#define OD_MUX_1029_SUB_0 0x102900
#define OD_MUX_1029_SUB_1 0x102901
#define OD_MUX_1029_SUB_2 0x102902
#define OD_MUX_1400_SUB_0 0x140000
#define OD_MUX_1400_SUB_1 0x140001
#define OD_MUX_1400_SUB_2 0x140002
#define OD_MUX_1400_SUB_3 0x140003
#define OD_MUX_1400_SUB_4 0x140004
#define OD_MUX_1400_SUB_5 0x140005
#define OD_MUX_1401_SUB_0 0x140100
#define OD_MUX_1401_SUB_1 0x140101
#define OD_MUX_1401_SUB_2 0x140102
#define OD_MUX_1401_SUB_3 0x140103
#define OD_MUX_1401_SUB_4 0x140104
#define OD_MUX_1401_SUB_5 0x140105
#define OD_MUX_1402_SUB_0 0x140200
#define OD_MUX_1402_SUB_1 0x140201
#define OD_MUX_1402_SUB_2 0x140202
#define OD_MUX_1402_SUB_3 0x140203
#define OD_MUX_1402_SUB_4 0x140204
#define OD_MUX_1402_SUB_5 0x140205
#define OD_MUX_1403_SUB_0 0x140300
#define OD_MUX_1403_SUB_1 0x140301
#define OD_MUX_1403_SUB_2 0x140302
#define OD_MUX_1403_SUB_3 0x140303
#define OD_MUX_1403_SUB_4 0x140304
#define OD_MUX_1403_SUB_5 0x140305
#define OD_MUX_1600_SUB_0 0x160000
#define OD_MUX_1600_SUB_1 0x160001
#define OD_MUX_1600_SUB_2 0x160002
#define OD_MUX_1600_SUB_3 0x160003
#define OD_MUX_1600_SUB_4 0x160004
#define OD_MUX_1600_SUB_5 0x160005
#define OD_MUX_1600_SUB_6 0x160006
#define OD_MUX_1600_SUB_7 0x160007
#define OD_MUX_1600_SUB_8 0x160008
#define OD_MUX_1601_SUB_0 0x160100
#define OD_MUX_1601_SUB_1 0x160101
#define OD_MUX_1601_SUB_2 0x160102
#define OD_MUX_1601_SUB_3 0x160103
#define OD_MUX_1601_SUB_4 0x160104
#define OD_MUX_1601_SUB_5 0x160105
#define OD_MUX_1601_SUB_6 0x160106
#define OD_MUX_1601_SUB_7 0x160107
#define OD_MUX_1601_SUB_8 0x160108
#define OD_MUX_1602_SUB_0 0x160200
#define OD_MUX_1602_SUB_1 0x160201
#define OD_MUX_1602_SUB_2 0x160202
#define OD_MUX_1602_SUB_3 0x160203
#define OD_MUX_1602_SUB_4 0x160204
#define OD_MUX_1602_SUB_5 0x160205
#define OD_MUX_1602_SUB_6 0x160206
#define OD_MUX_1602_SUB_7 0x160207
#define OD_MUX_1602_SUB_8 0x160208
#define OD_MUX_1603_SUB_0 0x160300
#define OD_MUX_1603_SUB_1 0x160301
#define OD_MUX_1603_SUB_2 0x160302
#define OD_MUX_1603_SUB_3 0x160303
#define OD_MUX_1603_SUB_4 0x160304
#define OD_MUX_1603_SUB_5 0x160305
#define OD_MUX_1603_SUB_6 0x160306
#define OD_MUX_1603_SUB_7 0x160307
#define OD_MUX_1603_SUB_8 0x160308
#define OD_MUX_1800_SUB_0 0x180000
#define OD_MUX_1800_SUB_1 0x180001
#define OD_MUX_1800_SUB_2 0x180002
#define OD_MUX_1800_SUB_3 0x180003
#define OD_MUX_1800_SUB_4 0x180004
#define OD_MUX_1800_SUB_5 0x180005
#define OD_MUX_1800_SUB_6 0x180006
#define OD_MUX_1801_SUB_0 0x180100
#define OD_MUX_1801_SUB_1 0x180101
#define OD_MUX_1801_SUB_2 0x180102
#define OD_MUX_1801_SUB_3 0x180103
#define OD_MUX_1801_SUB_4 0x180104
#define OD_MUX_1801_SUB_5 0x180105
#define OD_MUX_1801_SUB_6 0x180106
#define OD_MUX_1802_SUB_0 0x180200
#define OD_MUX_1802_SUB_1 0x180201
#define OD_MUX_1802_SUB_2 0x180202
#define OD_MUX_1802_SUB_3 0x180203
#define OD_MUX_1802_SUB_4 0x180204
#define OD_MUX_1802_SUB_5 0x180205
#define OD_MUX_1802_SUB_6 0x180206
#define OD_MUX_1803_SUB_0 0x180300
#define OD_MUX_1803_SUB_1 0x180301
#define OD_MUX_1803_SUB_2 0x180302
#define OD_MUX_1803_SUB_3 0x180303
#define OD_MUX_1803_SUB_4 0x180304
#define OD_MUX_1803_SUB_5 0x180305
#define OD_MUX_1803_SUB_6 0x180306
#define OD_MUX_1A00_SUB_0 0x1A0000
#define OD_MUX_1A00_SUB_1 0x1A0001
#define OD_MUX_1A00_SUB_2 0x1A0002
#define OD_MUX_1A00_SUB_3 0x1A0003
#define OD_MUX_1A00_SUB_4 0x1A0004
#define OD_MUX_1A00_SUB_5 0x1A0005
#define OD_MUX_1A00_SUB_6 0x1A0006
#define OD_MUX_1A00_SUB_7 0x1A0007
#define OD_MUX_1A00_SUB_8 0x1A0008
#define OD_MUX_1A01_SUB_0 0x1A0100
#define OD_MUX_1A01_SUB_1 0x1A0101
#define OD_MUX_1A01_SUB_2 0x1A0102
#define OD_MUX_1A01_SUB_3 0x1A0103
#define OD_MUX_1A01_SUB_4 0x1A0104
#define OD_MUX_1A01_SUB_5 0x1A0105
#define OD_MUX_1A01_SUB_6 0x1A0106
#define OD_MUX_1A01_SUB_7 0x1A0107
#define OD_MUX_1A01_SUB_8 0x1A0108
#define OD_MUX_1A02_SUB_0 0x1A0200
#define OD_MUX_1A02_SUB_1 0x1A0201
#define OD_MUX_1A02_SUB_2 0x1A0202
#define OD_MUX_1A02_SUB_3 0x1A0203
#define OD_MUX_1A02_SUB_4 0x1A0204
#define OD_MUX_1A02_SUB_5 0x1A0205
#define OD_MUX_1A02_SUB_6 0x1A0206
#define OD_MUX_1A02_SUB_7 0x1A0207
#define OD_MUX_1A02_SUB_8 0x1A0208
#define OD_MUX_1A03_SUB_0 0x1A0300
#define OD_MUX_1A03_SUB_1 0x1A0301
#define OD_MUX_1A03_SUB_2 0x1A0302
#define OD_MUX_1A03_SUB_3 0x1A0303
#define OD_MUX_1A03_SUB_4 0x1A0304
#define OD_MUX_1A03_SUB_5 0x1A0305
#define OD_MUX_1A03_SUB_6 0x1A0306
#define OD_MUX_1A03_SUB_7 0x1A0307
#define OD_MUX_1A03_SUB_8 0x1A0308
#define OD_MUX_1F51_SUB_0 0x1F5100
#define OD_MUX_1F57_SUB_0 0x1F5700
#define OD_MUX_1F57_SUB_1 0x1F5701
#define OD_MUX_1F57_SUB_2 0x1F5702
#define OD_MUX_1F80_SUB_0 0x1F8000
#define OD_MUX_6007_SUB_0 0x600700
#define OD_MUX_603F_SUB_0 0x603F00
#define OD_MUX_6040_SUB_0 0x604000
#define OD_MUX_6041_SUB_0 0x604100
#define OD_MUX_6042_SUB_0 0x604200
#define OD_MUX_6043_SUB_0 0x604300
#define OD_MUX_6044_SUB_0 0x604400
#define OD_MUX_6046_SUB_0 0x604600
#define OD_MUX_6046_SUB_1 0x604601
#define OD_MUX_6046_SUB_2 0x604602
#define OD_MUX_6048_SUB_0 0x604800
#define OD_MUX_6048_SUB_1 0x604801
#define OD_MUX_6048_SUB_2 0x604802
#define OD_MUX_6049_SUB_0 0x604900
#define OD_MUX_6049_SUB_1 0x604901
#define OD_MUX_6049_SUB_2 0x604902
#define OD_MUX_604A_SUB_0 0x604A00
#define OD_MUX_604A_SUB_1 0x604A01
#define OD_MUX_604A_SUB_2 0x604A02
#define OD_MUX_6060_SUB_0 0x606000
#define OD_MUX_6061_SUB_0 0x606100
#define OD_MUX_6064_SUB_0 0x606400
#define OD_MUX_607A_SUB_0 0x607A00
#define OD_MUX_6081_SUB_0 0x608100
#define OD_MUX_60FD_SUB_0 0x60FD00
#define OD_MUX_60FE_SUB_0 0x60FE00
#define OD_MUX_60FE_SUB_1 0x60FE01

/* Useful if only the defines from this file are needed */ 
#ifndef _OD_DEFINES_ONLY
#include "od_include.hpp"

namespace CANopen
{
/**
 * This struct represents the object dictionary raw data.
 * It is an auto-generated struct that should not be manually edited.
 */
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
    struct {uint8_t sub0 = 5; uint32_t sub1 = 769; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0;} x1401;
    struct {uint8_t sub0 = 5; uint32_t sub1 = 1025; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0;} x1402;
    struct {uint8_t sub0 = 5; uint32_t sub1 = 1281; uint8_t sub2 = 255; uint16_t sub3 = 100; uint8_t sub4 = 0; uint16_t sub5 = 0;} x1403;
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

/**
 * Object Dictionary.
 * It is an auto-generated class that should not be manually edited.
 */
class ObjectDictionnary
{
// @cond
private:
    ObjectDictionnaryData data;
    ObjectEntry<int8_t> entry_x2sub0 = ObjectEntry<int8_t>(&data.x2, 1);
    ObjectEntryBase *entries_x2[1] = {&entry_x2sub0};
    ObjectEntry<int16_t> entry_x3sub0 = ObjectEntry<int16_t>(&data.x3, 1);
    ObjectEntryBase *entries_x3[1] = {&entry_x3sub0};
    ObjectEntry<int32_t> entry_x4sub0 = ObjectEntry<int32_t>(&data.x4, 1);
    ObjectEntryBase *entries_x4[1] = {&entry_x4sub0};
    ObjectEntry<uint8_t> entry_x5sub0 = ObjectEntry<uint8_t>(&data.x5, 1);
    ObjectEntryBase *entries_x5[1] = {&entry_x5sub0};
    ObjectEntry<uint16_t> entry_x6sub0 = ObjectEntry<uint16_t>(&data.x6, 1);
    ObjectEntryBase *entries_x6[1] = {&entry_x6sub0};
    ObjectEntry<uint32_t> entry_x7sub0 = ObjectEntry<uint32_t>(&data.x7, 1);
    ObjectEntryBase *entries_x7[1] = {&entry_x7sub0};
    ObjectEntry<uint32_t> entry_x1000sub0 = ObjectEntry<uint32_t>(&data.x1000, 1);
    ObjectEntryBase *entries_x1000[1] = {&entry_x1000sub0};
    ObjectEntry<uint8_t> entry_x1001sub0 = ObjectEntry<uint8_t>(&data.x1001, 5);
    ObjectEntryBase *entries_x1001[1] = {&entry_x1001sub0};
    ObjectEntry<uint8_t> entry_x1003sub0 = ObjectEntry<uint8_t>(&data.x1003sub0, 3);
    ObjectEntry<uint32_t> entry_x1003sub1 = ObjectEntry<uint32_t>(&data.x1003[0], 1);
    ObjectEntry<uint32_t> entry_x1003sub2 = ObjectEntry<uint32_t>(&data.x1003[1], 1);
    ObjectEntry<uint32_t> entry_x1003sub3 = ObjectEntry<uint32_t>(&data.x1003[2], 1);
    ObjectEntry<uint32_t> entry_x1003sub4 = ObjectEntry<uint32_t>(&data.x1003[3], 1);
    ObjectEntry<uint32_t> entry_x1003sub5 = ObjectEntry<uint32_t>(&data.x1003[4], 1);
    ObjectEntry<uint32_t> entry_x1003sub6 = ObjectEntry<uint32_t>(&data.x1003[5], 1);
    ObjectEntry<uint32_t> entry_x1003sub7 = ObjectEntry<uint32_t>(&data.x1003[6], 1);
    ObjectEntry<uint32_t> entry_x1003sub8 = ObjectEntry<uint32_t>(&data.x1003[7], 1);
    ObjectEntryBase *entries_x1003[9] = {&entry_x1003sub0, &entry_x1003sub1, &entry_x1003sub2, &entry_x1003sub3, &entry_x1003sub4, &entry_x1003sub5, &entry_x1003sub6, &entry_x1003sub7, &entry_x1003sub8};
    ObjectEntry<uint32_t> entry_x1005sub0 = ObjectEntry<uint32_t>(&data.x1005, 3);
    ObjectEntryBase *entries_x1005[1] = {&entry_x1005sub0};
    ObjectEntry<uint32_t> entry_x1006sub0 = ObjectEntry<uint32_t>(&data.x1006, 3);
    ObjectEntryBase *entries_x1006[1] = {&entry_x1006sub0};
    ObjectEntry<uint32_t> entry_x1007sub0 = ObjectEntry<uint32_t>(&data.x1007, 3);
    ObjectEntryBase *entries_x1007[1] = {&entry_x1007sub0};
    ObjectEntry<uint8_t[17]> entry_x1008sub0 = ObjectEntry<uint8_t[17]>(&data.x1008, 1);
    ObjectEntryBase *entries_x1008[1] = {&entry_x1008sub0};
    ObjectEntry<uint8_t[12]> entry_x1009sub0 = ObjectEntry<uint8_t[12]>(&data.x1009, 1);
    ObjectEntryBase *entries_x1009[1] = {&entry_x1009sub0};
    ObjectEntry<uint8_t[18]> entry_x100Asub0 = ObjectEntry<uint8_t[18]>(&data.x100A, 1);
    ObjectEntryBase *entries_x100A[1] = {&entry_x100Asub0};
    ObjectEntry<uint16_t> entry_x100Csub0 = ObjectEntry<uint16_t>(&data.x100C, 3);
    ObjectEntryBase *entries_x100C[1] = {&entry_x100Csub0};
    ObjectEntry<uint8_t> entry_x100Dsub0 = ObjectEntry<uint8_t>(&data.x100D, 3);
    ObjectEntryBase *entries_x100D[1] = {&entry_x100Dsub0};
    ObjectEntry<uint8_t> entry_x1010sub0 = ObjectEntry<uint8_t>(&data.x1010sub0, 1);
    ObjectEntry<uint32_t> entry_x1010sub1 = ObjectEntry<uint32_t>(&data.x1010[0], 3);
    ObjectEntry<uint32_t> entry_x1010sub2 = ObjectEntry<uint32_t>(&data.x1010[1], 3);
    ObjectEntry<uint32_t> entry_x1010sub3 = ObjectEntry<uint32_t>(&data.x1010[2], 3);
    ObjectEntry<uint32_t> entry_x1010sub4 = ObjectEntry<uint32_t>(&data.x1010[3], 3);
    ObjectEntry<uint32_t> entry_x1010sub5 = ObjectEntry<uint32_t>(&data.x1010[4], 3);
    ObjectEntry<uint32_t> entry_x1010sub6 = ObjectEntry<uint32_t>(&data.x1010[5], 3);
    ObjectEntry<uint32_t> entry_x1010sub7 = ObjectEntry<uint32_t>(&data.x1010[6], 3);
    ObjectEntry<uint32_t> entry_x1010sub8 = ObjectEntry<uint32_t>(&data.x1010[7], 3);
    ObjectEntry<uint32_t> entry_x1010sub9 = ObjectEntry<uint32_t>(&data.x1010[8], 3);
    ObjectEntry<uint32_t> entry_x1010sub10 = ObjectEntry<uint32_t>(&data.x1010[9], 3);
    ObjectEntry<uint32_t> entry_x1010sub11 = ObjectEntry<uint32_t>(&data.x1010[10], 3);
    ObjectEntry<uint32_t> entry_x1010sub12 = ObjectEntry<uint32_t>(&data.x1010[11], 3);
    ObjectEntry<uint32_t> entry_x1010sub13 = ObjectEntry<uint32_t>(&data.x1010[12], 3);
    ObjectEntryBase *entries_x1010[14] = {&entry_x1010sub0, &entry_x1010sub1, &entry_x1010sub2, &entry_x1010sub3, &entry_x1010sub4, &entry_x1010sub5, &entry_x1010sub6, &entry_x1010sub7, &entry_x1010sub8, &entry_x1010sub9, &entry_x1010sub10, &entry_x1010sub11, &entry_x1010sub12, &entry_x1010sub13};
    ObjectEntry<uint8_t> entry_x1011sub0 = ObjectEntry<uint8_t>(&data.x1011sub0, 1);
    ObjectEntry<uint32_t> entry_x1011sub1 = ObjectEntry<uint32_t>(&data.x1011[0], 3);
    ObjectEntry<uint32_t> entry_x1011sub2 = ObjectEntry<uint32_t>(&data.x1011[1], 3);
    ObjectEntry<uint32_t> entry_x1011sub3 = ObjectEntry<uint32_t>(&data.x1011[2], 3);
    ObjectEntry<uint32_t> entry_x1011sub4 = ObjectEntry<uint32_t>(&data.x1011[3], 3);
    ObjectEntry<uint32_t> entry_x1011sub5 = ObjectEntry<uint32_t>(&data.x1011[4], 3);
    ObjectEntry<uint32_t> entry_x1011sub6 = ObjectEntry<uint32_t>(&data.x1011[5], 3);
    ObjectEntry<uint32_t> entry_x1011sub7 = ObjectEntry<uint32_t>(&data.x1011[6], 3);
    ObjectEntry<uint32_t> entry_x1011sub8 = ObjectEntry<uint32_t>(&data.x1011[7], 3);
    ObjectEntry<uint32_t> entry_x1011sub9 = ObjectEntry<uint32_t>(&data.x1011[8], 3);
    ObjectEntry<uint32_t> entry_x1011sub10 = ObjectEntry<uint32_t>(&data.x1011[9], 3);
    ObjectEntry<uint32_t> entry_x1011sub11 = ObjectEntry<uint32_t>(&data.x1011[10], 3);
    ObjectEntry<uint32_t> entry_x1011sub12 = ObjectEntry<uint32_t>(&data.x1011[11], 3);
    ObjectEntry<uint32_t> entry_x1011sub13 = ObjectEntry<uint32_t>(&data.x1011[12], 3);
    ObjectEntryBase *entries_x1011[14] = {&entry_x1011sub0, &entry_x1011sub1, &entry_x1011sub2, &entry_x1011sub3, &entry_x1011sub4, &entry_x1011sub5, &entry_x1011sub6, &entry_x1011sub7, &entry_x1011sub8, &entry_x1011sub9, &entry_x1011sub10, &entry_x1011sub11, &entry_x1011sub12, &entry_x1011sub13};
    ObjectEntry<uint32_t> entry_x1014sub0 = ObjectEntry<uint32_t>(&data.x1014, 3);
    ObjectEntryBase *entries_x1014[1] = {&entry_x1014sub0};
    ObjectEntry<uint8_t> entry_x1016sub0 = ObjectEntry<uint8_t>(&data.x1016sub0, 1);
    ObjectEntry<uint32_t> entry_x1016sub1 = ObjectEntry<uint32_t>(&data.x1016[0], 3);
    ObjectEntryBase *entries_x1016[2] = {&entry_x1016sub0, &entry_x1016sub1};
    ObjectEntry<uint16_t> entry_x1017sub0 = ObjectEntry<uint16_t>(&data.x1017, 3);
    ObjectEntryBase *entries_x1017[1] = {&entry_x1017sub0};
    ObjectEntry<uint8_t> entry_x1018sub0 = ObjectEntry<uint8_t>(&data.x1018.sub0, 1);
    ObjectEntry<uint32_t> entry_x1018sub1 = ObjectEntry<uint32_t>(&data.x1018.sub1, 1);
    ObjectEntry<uint32_t> entry_x1018sub2 = ObjectEntry<uint32_t>(&data.x1018.sub2, 1);
    ObjectEntry<uint32_t> entry_x1018sub3 = ObjectEntry<uint32_t>(&data.x1018.sub3, 1);
    ObjectEntry<uint32_t> entry_x1018sub4 = ObjectEntry<uint32_t>(&data.x1018.sub4, 1);
    ObjectEntryBase *entries_x1018[5] = {&entry_x1018sub0, &entry_x1018sub1, &entry_x1018sub2, &entry_x1018sub3, &entry_x1018sub4};
    ObjectEntry<uint8_t> entry_x1019sub0 = ObjectEntry<uint8_t>(&data.x1019, 3);
    ObjectEntryBase *entries_x1019[1] = {&entry_x1019sub0};
    ObjectEntry<uint8_t> entry_x1020sub0 = ObjectEntry<uint8_t>(&data.x1020sub0, 1);
    ObjectEntry<uint32_t> entry_x1020sub1 = ObjectEntry<uint32_t>(&data.x1020[0], 3);
    ObjectEntry<uint32_t> entry_x1020sub2 = ObjectEntry<uint32_t>(&data.x1020[1], 3);
    ObjectEntryBase *entries_x1020[3] = {&entry_x1020sub0, &entry_x1020sub1, &entry_x1020sub2};
    ObjectEntry<uint8_t> entry_x1029sub0 = ObjectEntry<uint8_t>(&data.x1029sub0, 1);
    ObjectEntry<uint8_t> entry_x1029sub1 = ObjectEntry<uint8_t>(&data.x1029[0], 3);
    ObjectEntry<uint8_t> entry_x1029sub2 = ObjectEntry<uint8_t>(&data.x1029[1], 3);
    ObjectEntryBase *entries_x1029[3] = {&entry_x1029sub0, &entry_x1029sub1, &entry_x1029sub2};
    ObjectEntry<uint8_t> entry_x1400sub0 = ObjectEntry<uint8_t>(&data.x1400.sub0, 1);
    ObjectEntry<uint32_t> entry_x1400sub1 = ObjectEntry<uint32_t>(&data.x1400.sub1, 3);
    ObjectEntry<uint8_t> entry_x1400sub2 = ObjectEntry<uint8_t>(&data.x1400.sub2, 3);
    ObjectEntry<uint16_t> entry_x1400sub3 = ObjectEntry<uint16_t>(&data.x1400.sub3, 3);
    ObjectEntry<uint8_t> entry_x1400sub4 = ObjectEntry<uint8_t>(&data.x1400.sub4, 3);
    ObjectEntry<uint16_t> entry_x1400sub5 = ObjectEntry<uint16_t>(&data.x1400.sub5, 3);
    ObjectEntryBase *entries_x1400[6] = {&entry_x1400sub0, &entry_x1400sub1, &entry_x1400sub2, &entry_x1400sub3, &entry_x1400sub4, &entry_x1400sub5};
    ObjectEntry<uint8_t> entry_x1401sub0 = ObjectEntry<uint8_t>(&data.x1401.sub0, 1);
    ObjectEntry<uint32_t> entry_x1401sub1 = ObjectEntry<uint32_t>(&data.x1401.sub1, 3);
    ObjectEntry<uint8_t> entry_x1401sub2 = ObjectEntry<uint8_t>(&data.x1401.sub2, 3);
    ObjectEntry<uint16_t> entry_x1401sub3 = ObjectEntry<uint16_t>(&data.x1401.sub3, 3);
    ObjectEntry<uint8_t> entry_x1401sub4 = ObjectEntry<uint8_t>(&data.x1401.sub4, 3);
    ObjectEntry<uint16_t> entry_x1401sub5 = ObjectEntry<uint16_t>(&data.x1401.sub5, 3);
    ObjectEntryBase *entries_x1401[6] = {&entry_x1401sub0, &entry_x1401sub1, &entry_x1401sub2, &entry_x1401sub3, &entry_x1401sub4, &entry_x1401sub5};
    ObjectEntry<uint8_t> entry_x1402sub0 = ObjectEntry<uint8_t>(&data.x1402.sub0, 1);
    ObjectEntry<uint32_t> entry_x1402sub1 = ObjectEntry<uint32_t>(&data.x1402.sub1, 3);
    ObjectEntry<uint8_t> entry_x1402sub2 = ObjectEntry<uint8_t>(&data.x1402.sub2, 3);
    ObjectEntry<uint16_t> entry_x1402sub3 = ObjectEntry<uint16_t>(&data.x1402.sub3, 3);
    ObjectEntry<uint8_t> entry_x1402sub4 = ObjectEntry<uint8_t>(&data.x1402.sub4, 3);
    ObjectEntry<uint16_t> entry_x1402sub5 = ObjectEntry<uint16_t>(&data.x1402.sub5, 3);
    ObjectEntryBase *entries_x1402[6] = {&entry_x1402sub0, &entry_x1402sub1, &entry_x1402sub2, &entry_x1402sub3, &entry_x1402sub4, &entry_x1402sub5};
    ObjectEntry<uint8_t> entry_x1403sub0 = ObjectEntry<uint8_t>(&data.x1403.sub0, 1);
    ObjectEntry<uint32_t> entry_x1403sub1 = ObjectEntry<uint32_t>(&data.x1403.sub1, 3);
    ObjectEntry<uint8_t> entry_x1403sub2 = ObjectEntry<uint8_t>(&data.x1403.sub2, 3);
    ObjectEntry<uint16_t> entry_x1403sub3 = ObjectEntry<uint16_t>(&data.x1403.sub3, 3);
    ObjectEntry<uint8_t> entry_x1403sub4 = ObjectEntry<uint8_t>(&data.x1403.sub4, 3);
    ObjectEntry<uint16_t> entry_x1403sub5 = ObjectEntry<uint16_t>(&data.x1403.sub5, 3);
    ObjectEntryBase *entries_x1403[6] = {&entry_x1403sub0, &entry_x1403sub1, &entry_x1403sub2, &entry_x1403sub3, &entry_x1403sub4, &entry_x1403sub5};
    ObjectEntry<uint8_t> entry_x1600sub0 = ObjectEntry<uint8_t>(&data.x1600.sub0, 3);
    ObjectEntry<uint32_t> entry_x1600sub1 = ObjectEntry<uint32_t>(&data.x1600.sub1, 3);
    ObjectEntry<uint32_t> entry_x1600sub2 = ObjectEntry<uint32_t>(&data.x1600.sub2, 3);
    ObjectEntry<uint32_t> entry_x1600sub3 = ObjectEntry<uint32_t>(&data.x1600.sub3, 3);
    ObjectEntry<uint32_t> entry_x1600sub4 = ObjectEntry<uint32_t>(&data.x1600.sub4, 3);
    ObjectEntry<uint32_t> entry_x1600sub5 = ObjectEntry<uint32_t>(&data.x1600.sub5, 3);
    ObjectEntry<uint32_t> entry_x1600sub6 = ObjectEntry<uint32_t>(&data.x1600.sub6, 3);
    ObjectEntry<uint32_t> entry_x1600sub7 = ObjectEntry<uint32_t>(&data.x1600.sub7, 3);
    ObjectEntry<uint32_t> entry_x1600sub8 = ObjectEntry<uint32_t>(&data.x1600.sub8, 3);
    ObjectEntryBase *entries_x1600[9] = {&entry_x1600sub0, &entry_x1600sub1, &entry_x1600sub2, &entry_x1600sub3, &entry_x1600sub4, &entry_x1600sub5, &entry_x1600sub6, &entry_x1600sub7, &entry_x1600sub8};
    ObjectEntry<uint8_t> entry_x1601sub0 = ObjectEntry<uint8_t>(&data.x1601.sub0, 3);
    ObjectEntry<uint32_t> entry_x1601sub1 = ObjectEntry<uint32_t>(&data.x1601.sub1, 3);
    ObjectEntry<uint32_t> entry_x1601sub2 = ObjectEntry<uint32_t>(&data.x1601.sub2, 3);
    ObjectEntry<uint32_t> entry_x1601sub3 = ObjectEntry<uint32_t>(&data.x1601.sub3, 3);
    ObjectEntry<uint32_t> entry_x1601sub4 = ObjectEntry<uint32_t>(&data.x1601.sub4, 3);
    ObjectEntry<uint32_t> entry_x1601sub5 = ObjectEntry<uint32_t>(&data.x1601.sub5, 3);
    ObjectEntry<uint32_t> entry_x1601sub6 = ObjectEntry<uint32_t>(&data.x1601.sub6, 3);
    ObjectEntry<uint32_t> entry_x1601sub7 = ObjectEntry<uint32_t>(&data.x1601.sub7, 3);
    ObjectEntry<uint32_t> entry_x1601sub8 = ObjectEntry<uint32_t>(&data.x1601.sub8, 3);
    ObjectEntryBase *entries_x1601[9] = {&entry_x1601sub0, &entry_x1601sub1, &entry_x1601sub2, &entry_x1601sub3, &entry_x1601sub4, &entry_x1601sub5, &entry_x1601sub6, &entry_x1601sub7, &entry_x1601sub8};
    ObjectEntry<uint8_t> entry_x1602sub0 = ObjectEntry<uint8_t>(&data.x1602.sub0, 3);
    ObjectEntry<uint32_t> entry_x1602sub1 = ObjectEntry<uint32_t>(&data.x1602.sub1, 3);
    ObjectEntry<uint32_t> entry_x1602sub2 = ObjectEntry<uint32_t>(&data.x1602.sub2, 3);
    ObjectEntry<uint32_t> entry_x1602sub3 = ObjectEntry<uint32_t>(&data.x1602.sub3, 3);
    ObjectEntry<uint32_t> entry_x1602sub4 = ObjectEntry<uint32_t>(&data.x1602.sub4, 3);
    ObjectEntry<uint32_t> entry_x1602sub5 = ObjectEntry<uint32_t>(&data.x1602.sub5, 3);
    ObjectEntry<uint32_t> entry_x1602sub6 = ObjectEntry<uint32_t>(&data.x1602.sub6, 3);
    ObjectEntry<uint32_t> entry_x1602sub7 = ObjectEntry<uint32_t>(&data.x1602.sub7, 3);
    ObjectEntry<uint32_t> entry_x1602sub8 = ObjectEntry<uint32_t>(&data.x1602.sub8, 3);
    ObjectEntryBase *entries_x1602[9] = {&entry_x1602sub0, &entry_x1602sub1, &entry_x1602sub2, &entry_x1602sub3, &entry_x1602sub4, &entry_x1602sub5, &entry_x1602sub6, &entry_x1602sub7, &entry_x1602sub8};
    ObjectEntry<uint8_t> entry_x1603sub0 = ObjectEntry<uint8_t>(&data.x1603.sub0, 3);
    ObjectEntry<uint32_t> entry_x1603sub1 = ObjectEntry<uint32_t>(&data.x1603.sub1, 3);
    ObjectEntry<uint32_t> entry_x1603sub2 = ObjectEntry<uint32_t>(&data.x1603.sub2, 3);
    ObjectEntry<uint32_t> entry_x1603sub3 = ObjectEntry<uint32_t>(&data.x1603.sub3, 3);
    ObjectEntry<uint32_t> entry_x1603sub4 = ObjectEntry<uint32_t>(&data.x1603.sub4, 3);
    ObjectEntry<uint32_t> entry_x1603sub5 = ObjectEntry<uint32_t>(&data.x1603.sub5, 3);
    ObjectEntry<uint32_t> entry_x1603sub6 = ObjectEntry<uint32_t>(&data.x1603.sub6, 3);
    ObjectEntry<uint32_t> entry_x1603sub7 = ObjectEntry<uint32_t>(&data.x1603.sub7, 3);
    ObjectEntry<uint32_t> entry_x1603sub8 = ObjectEntry<uint32_t>(&data.x1603.sub8, 3);
    ObjectEntryBase *entries_x1603[9] = {&entry_x1603sub0, &entry_x1603sub1, &entry_x1603sub2, &entry_x1603sub3, &entry_x1603sub4, &entry_x1603sub5, &entry_x1603sub6, &entry_x1603sub7, &entry_x1603sub8};
    ObjectEntry<uint8_t> entry_x1800sub0 = ObjectEntry<uint8_t>(&data.x1800.sub0, 1);
    ObjectEntry<uint32_t> entry_x1800sub1 = ObjectEntry<uint32_t>(&data.x1800.sub1, 3);
    ObjectEntry<uint8_t> entry_x1800sub2 = ObjectEntry<uint8_t>(&data.x1800.sub2, 3);
    ObjectEntry<uint16_t> entry_x1800sub3 = ObjectEntry<uint16_t>(&data.x1800.sub3, 3);
    ObjectEntry<uint8_t> entry_x1800sub4 = ObjectEntry<uint8_t>(&data.x1800.sub4, 3);
    ObjectEntry<uint16_t> entry_x1800sub5 = ObjectEntry<uint16_t>(&data.x1800.sub5, 3);
    ObjectEntry<uint8_t> entry_x1800sub6 = ObjectEntry<uint8_t>(&data.x1800.sub6, 3);
    ObjectEntryBase *entries_x1800[7] = {&entry_x1800sub0, &entry_x1800sub1, &entry_x1800sub2, &entry_x1800sub3, &entry_x1800sub4, &entry_x1800sub5, &entry_x1800sub6};
    ObjectEntry<uint8_t> entry_x1801sub0 = ObjectEntry<uint8_t>(&data.x1801.sub0, 1);
    ObjectEntry<uint32_t> entry_x1801sub1 = ObjectEntry<uint32_t>(&data.x1801.sub1, 3);
    ObjectEntry<uint8_t> entry_x1801sub2 = ObjectEntry<uint8_t>(&data.x1801.sub2, 3);
    ObjectEntry<uint16_t> entry_x1801sub3 = ObjectEntry<uint16_t>(&data.x1801.sub3, 3);
    ObjectEntry<uint8_t> entry_x1801sub4 = ObjectEntry<uint8_t>(&data.x1801.sub4, 3);
    ObjectEntry<uint16_t> entry_x1801sub5 = ObjectEntry<uint16_t>(&data.x1801.sub5, 3);
    ObjectEntry<uint8_t> entry_x1801sub6 = ObjectEntry<uint8_t>(&data.x1801.sub6, 3);
    ObjectEntryBase *entries_x1801[7] = {&entry_x1801sub0, &entry_x1801sub1, &entry_x1801sub2, &entry_x1801sub3, &entry_x1801sub4, &entry_x1801sub5, &entry_x1801sub6};
    ObjectEntry<uint8_t> entry_x1802sub0 = ObjectEntry<uint8_t>(&data.x1802.sub0, 1);
    ObjectEntry<uint32_t> entry_x1802sub1 = ObjectEntry<uint32_t>(&data.x1802.sub1, 3);
    ObjectEntry<uint8_t> entry_x1802sub2 = ObjectEntry<uint8_t>(&data.x1802.sub2, 3);
    ObjectEntry<uint16_t> entry_x1802sub3 = ObjectEntry<uint16_t>(&data.x1802.sub3, 3);
    ObjectEntry<uint8_t> entry_x1802sub4 = ObjectEntry<uint8_t>(&data.x1802.sub4, 3);
    ObjectEntry<uint16_t> entry_x1802sub5 = ObjectEntry<uint16_t>(&data.x1802.sub5, 3);
    ObjectEntry<uint8_t> entry_x1802sub6 = ObjectEntry<uint8_t>(&data.x1802.sub6, 3);
    ObjectEntryBase *entries_x1802[7] = {&entry_x1802sub0, &entry_x1802sub1, &entry_x1802sub2, &entry_x1802sub3, &entry_x1802sub4, &entry_x1802sub5, &entry_x1802sub6};
    ObjectEntry<uint8_t> entry_x1803sub0 = ObjectEntry<uint8_t>(&data.x1803.sub0, 1);
    ObjectEntry<uint32_t> entry_x1803sub1 = ObjectEntry<uint32_t>(&data.x1803.sub1, 3);
    ObjectEntry<uint8_t> entry_x1803sub2 = ObjectEntry<uint8_t>(&data.x1803.sub2, 3);
    ObjectEntry<uint16_t> entry_x1803sub3 = ObjectEntry<uint16_t>(&data.x1803.sub3, 3);
    ObjectEntry<uint8_t> entry_x1803sub4 = ObjectEntry<uint8_t>(&data.x1803.sub4, 3);
    ObjectEntry<uint16_t> entry_x1803sub5 = ObjectEntry<uint16_t>(&data.x1803.sub5, 3);
    ObjectEntry<uint8_t> entry_x1803sub6 = ObjectEntry<uint8_t>(&data.x1803.sub6, 3);
    ObjectEntryBase *entries_x1803[7] = {&entry_x1803sub0, &entry_x1803sub1, &entry_x1803sub2, &entry_x1803sub3, &entry_x1803sub4, &entry_x1803sub5, &entry_x1803sub6};
    ObjectEntry<uint8_t> entry_x1A00sub0 = ObjectEntry<uint8_t>(&data.x1A00.sub0, 3);
    ObjectEntry<uint32_t> entry_x1A00sub1 = ObjectEntry<uint32_t>(&data.x1A00.sub1, 3);
    ObjectEntry<uint32_t> entry_x1A00sub2 = ObjectEntry<uint32_t>(&data.x1A00.sub2, 3);
    ObjectEntry<uint32_t> entry_x1A00sub3 = ObjectEntry<uint32_t>(&data.x1A00.sub3, 3);
    ObjectEntry<uint32_t> entry_x1A00sub4 = ObjectEntry<uint32_t>(&data.x1A00.sub4, 3);
    ObjectEntry<uint32_t> entry_x1A00sub5 = ObjectEntry<uint32_t>(&data.x1A00.sub5, 3);
    ObjectEntry<uint32_t> entry_x1A00sub6 = ObjectEntry<uint32_t>(&data.x1A00.sub6, 3);
    ObjectEntry<uint32_t> entry_x1A00sub7 = ObjectEntry<uint32_t>(&data.x1A00.sub7, 3);
    ObjectEntry<uint32_t> entry_x1A00sub8 = ObjectEntry<uint32_t>(&data.x1A00.sub8, 3);
    ObjectEntryBase *entries_x1A00[9] = {&entry_x1A00sub0, &entry_x1A00sub1, &entry_x1A00sub2, &entry_x1A00sub3, &entry_x1A00sub4, &entry_x1A00sub5, &entry_x1A00sub6, &entry_x1A00sub7, &entry_x1A00sub8};
    ObjectEntry<uint8_t> entry_x1A01sub0 = ObjectEntry<uint8_t>(&data.x1A01.sub0, 3);
    ObjectEntry<uint32_t> entry_x1A01sub1 = ObjectEntry<uint32_t>(&data.x1A01.sub1, 3);
    ObjectEntry<uint32_t> entry_x1A01sub2 = ObjectEntry<uint32_t>(&data.x1A01.sub2, 3);
    ObjectEntry<uint32_t> entry_x1A01sub3 = ObjectEntry<uint32_t>(&data.x1A01.sub3, 3);
    ObjectEntry<uint32_t> entry_x1A01sub4 = ObjectEntry<uint32_t>(&data.x1A01.sub4, 3);
    ObjectEntry<uint32_t> entry_x1A01sub5 = ObjectEntry<uint32_t>(&data.x1A01.sub5, 3);
    ObjectEntry<uint32_t> entry_x1A01sub6 = ObjectEntry<uint32_t>(&data.x1A01.sub6, 3);
    ObjectEntry<uint32_t> entry_x1A01sub7 = ObjectEntry<uint32_t>(&data.x1A01.sub7, 3);
    ObjectEntry<uint32_t> entry_x1A01sub8 = ObjectEntry<uint32_t>(&data.x1A01.sub8, 3);
    ObjectEntryBase *entries_x1A01[9] = {&entry_x1A01sub0, &entry_x1A01sub1, &entry_x1A01sub2, &entry_x1A01sub3, &entry_x1A01sub4, &entry_x1A01sub5, &entry_x1A01sub6, &entry_x1A01sub7, &entry_x1A01sub8};
    ObjectEntry<uint8_t> entry_x1A02sub0 = ObjectEntry<uint8_t>(&data.x1A02.sub0, 3);
    ObjectEntry<uint32_t> entry_x1A02sub1 = ObjectEntry<uint32_t>(&data.x1A02.sub1, 3);
    ObjectEntry<uint32_t> entry_x1A02sub2 = ObjectEntry<uint32_t>(&data.x1A02.sub2, 3);
    ObjectEntry<uint32_t> entry_x1A02sub3 = ObjectEntry<uint32_t>(&data.x1A02.sub3, 3);
    ObjectEntry<uint32_t> entry_x1A02sub4 = ObjectEntry<uint32_t>(&data.x1A02.sub4, 3);
    ObjectEntry<uint32_t> entry_x1A02sub5 = ObjectEntry<uint32_t>(&data.x1A02.sub5, 3);
    ObjectEntry<uint32_t> entry_x1A02sub6 = ObjectEntry<uint32_t>(&data.x1A02.sub6, 3);
    ObjectEntry<uint32_t> entry_x1A02sub7 = ObjectEntry<uint32_t>(&data.x1A02.sub7, 3);
    ObjectEntry<uint32_t> entry_x1A02sub8 = ObjectEntry<uint32_t>(&data.x1A02.sub8, 3);
    ObjectEntryBase *entries_x1A02[9] = {&entry_x1A02sub0, &entry_x1A02sub1, &entry_x1A02sub2, &entry_x1A02sub3, &entry_x1A02sub4, &entry_x1A02sub5, &entry_x1A02sub6, &entry_x1A02sub7, &entry_x1A02sub8};
    ObjectEntry<uint8_t> entry_x1A03sub0 = ObjectEntry<uint8_t>(&data.x1A03.sub0, 3);
    ObjectEntry<uint32_t> entry_x1A03sub1 = ObjectEntry<uint32_t>(&data.x1A03.sub1, 3);
    ObjectEntry<uint32_t> entry_x1A03sub2 = ObjectEntry<uint32_t>(&data.x1A03.sub2, 3);
    ObjectEntry<uint32_t> entry_x1A03sub3 = ObjectEntry<uint32_t>(&data.x1A03.sub3, 3);
    ObjectEntry<uint32_t> entry_x1A03sub4 = ObjectEntry<uint32_t>(&data.x1A03.sub4, 3);
    ObjectEntry<uint32_t> entry_x1A03sub5 = ObjectEntry<uint32_t>(&data.x1A03.sub5, 3);
    ObjectEntry<uint32_t> entry_x1A03sub6 = ObjectEntry<uint32_t>(&data.x1A03.sub6, 3);
    ObjectEntry<uint32_t> entry_x1A03sub7 = ObjectEntry<uint32_t>(&data.x1A03.sub7, 3);
    ObjectEntry<uint32_t> entry_x1A03sub8 = ObjectEntry<uint32_t>(&data.x1A03.sub8, 3);
    ObjectEntryBase *entries_x1A03[9] = {&entry_x1A03sub0, &entry_x1A03sub1, &entry_x1A03sub2, &entry_x1A03sub3, &entry_x1A03sub4, &entry_x1A03sub5, &entry_x1A03sub6, &entry_x1A03sub7, &entry_x1A03sub8};
    ObjectEntry<uint8_t[974]> entry_x1F51sub0 = ObjectEntry<uint8_t[974]>(&data.x1F51, 3);
    ObjectEntryBase *entries_x1F51[1] = {&entry_x1F51sub0};
    ObjectEntry<uint8_t> entry_x1F57sub0 = ObjectEntry<uint8_t>(&data.x1F57sub0, 1);
    ObjectEntry<uint32_t> entry_x1F57sub1 = ObjectEntry<uint32_t>(&data.x1F57[0], 1);
    ObjectEntry<uint32_t> entry_x1F57sub2 = ObjectEntry<uint32_t>(&data.x1F57[1], 1);
    ObjectEntryBase *entries_x1F57[3] = {&entry_x1F57sub0, &entry_x1F57sub1, &entry_x1F57sub2};
    ObjectEntry<uint32_t> entry_x1F80sub0 = ObjectEntry<uint32_t>(&data.x1F80, 3);
    ObjectEntryBase *entries_x1F80[1] = {&entry_x1F80sub0};
    ObjectEntry<int16_t> entry_x6007sub0 = ObjectEntry<int16_t>(&data.x6007, 3);
    ObjectEntryBase *entries_x6007[1] = {&entry_x6007sub0};
    ObjectEntry<uint16_t> entry_x603Fsub0 = ObjectEntry<uint16_t>(&data.x603F, 5);
    ObjectEntryBase *entries_x603F[1] = {&entry_x603Fsub0};
    ObjectEntry<uint16_t> entry_x6040sub0 = ObjectEntry<uint16_t>(&data.x6040, 7);
    ObjectEntryBase *entries_x6040[1] = {&entry_x6040sub0};
    ObjectEntry<uint16_t> entry_x6041sub0 = ObjectEntry<uint16_t>(&data.x6041, 5);
    ObjectEntryBase *entries_x6041[1] = {&entry_x6041sub0};
    ObjectEntry<int16_t> entry_x6042sub0 = ObjectEntry<int16_t>(&data.x6042, 7);
    ObjectEntryBase *entries_x6042[1] = {&entry_x6042sub0};
    ObjectEntry<int16_t> entry_x6043sub0 = ObjectEntry<int16_t>(&data.x6043, 5);
    ObjectEntryBase *entries_x6043[1] = {&entry_x6043sub0};
    ObjectEntry<int16_t> entry_x6044sub0 = ObjectEntry<int16_t>(&data.x6044, 5);
    ObjectEntryBase *entries_x6044[1] = {&entry_x6044sub0};
    ObjectEntry<uint8_t> entry_x6046sub0 = ObjectEntry<uint8_t>(&data.x6046sub0, 1);
    ObjectEntry<uint32_t> entry_x6046sub1 = ObjectEntry<uint32_t>(&data.x6046[0], 7);
    ObjectEntry<uint32_t> entry_x6046sub2 = ObjectEntry<uint32_t>(&data.x6046[1], 7);
    ObjectEntryBase *entries_x6046[3] = {&entry_x6046sub0, &entry_x6046sub1, &entry_x6046sub2};
    ObjectEntry<uint8_t> entry_x6048sub0 = ObjectEntry<uint8_t>(&data.x6048.sub0, 1);
    ObjectEntry<double> entry_x6048sub1 = ObjectEntry<double>(&data.x6048.sub1, 7);
    ObjectEntry<float> entry_x6048sub2 = ObjectEntry<float>(&data.x6048.sub2, 7);
    ObjectEntryBase *entries_x6048[3] = {&entry_x6048sub0, &entry_x6048sub1, &entry_x6048sub2};
    ObjectEntry<uint8_t> entry_x6049sub0 = ObjectEntry<uint8_t>(&data.x6049.sub0, 1);
    ObjectEntry<uint32_t> entry_x6049sub1 = ObjectEntry<uint32_t>(&data.x6049.sub1, 5);
    ObjectEntry<uint16_t> entry_x6049sub2 = ObjectEntry<uint16_t>(&data.x6049.sub2, 6);
    ObjectEntryBase *entries_x6049[3] = {&entry_x6049sub0, &entry_x6049sub1, &entry_x6049sub2};
    ObjectEntry<uint8_t> entry_x604Asub0 = ObjectEntry<uint8_t>(&data.x604A.sub0, 1);
    ObjectEntry<uint64_t> entry_x604Asub1 = ObjectEntry<uint64_t>(&data.x604A.sub1, 7);
    ObjectEntry<uint64_t> entry_x604Asub2 = ObjectEntry<uint64_t>(&data.x604A.sub2, 7);
    ObjectEntryBase *entries_x604A[3] = {&entry_x604Asub0, &entry_x604Asub1, &entry_x604Asub2};
    ObjectEntry<int8_t> entry_x6060sub0 = ObjectEntry<int8_t>(&data.x6060, 7);
    ObjectEntryBase *entries_x6060[1] = {&entry_x6060sub0};
    ObjectEntry<int8_t> entry_x6061sub0 = ObjectEntry<int8_t>(&data.x6061, 5);
    ObjectEntryBase *entries_x6061[1] = {&entry_x6061sub0};
    ObjectEntry<int32_t> entry_x6064sub0 = ObjectEntry<int32_t>(&data.x6064, 5);
    ObjectEntryBase *entries_x6064[1] = {&entry_x6064sub0};
    LimitedObjectEntry<int32_t> entry_x607Asub0 = LimitedObjectEntry<int32_t>(&data.x607A, 7, -5000, 5000);
    ObjectEntryBase *entries_x607A[1] = {&entry_x607Asub0};
    ObjectEntry<uint32_t> entry_x6081sub0 = ObjectEntry<uint32_t>(&data.x6081, 7);
    ObjectEntryBase *entries_x6081[1] = {&entry_x6081sub0};
    ObjectEntry<uint32_t> entry_x60FDsub0 = ObjectEntry<uint32_t>(&data.x60FD, 5);
    ObjectEntryBase *entries_x60FD[1] = {&entry_x60FDsub0};
    ObjectEntry<uint8_t> entry_x60FEsub0 = ObjectEntry<uint8_t>(&data.x60FEsub0, 1);
    ObjectEntry<uint32_t> entry_x60FEsub1 = ObjectEntry<uint32_t>(&data.x60FE[0], 7);
    ObjectEntryBase *entries_x60FE[2] = {&entry_x60FEsub0, &entry_x60FEsub1};
    Object object_x2 = Object(OD_OBJECT_2, 2, 1, entries_x2);
    Object object_x3 = Object(OD_OBJECT_3, 3, 1, entries_x3);
    Object object_x4 = Object(OD_OBJECT_4, 4, 1, entries_x4);
    Object object_x5 = Object(OD_OBJECT_5, 5, 1, entries_x5);
    Object object_x6 = Object(OD_OBJECT_6, 6, 1, entries_x6);
    Object object_x7 = Object(OD_OBJECT_7, 7, 1, entries_x7);
    Object object_x1000 = Object(OD_OBJECT_1000, 4096, 1, entries_x1000);
    Object1001 object_x1001 = Object1001(OD_OBJECT_1001, 4097, 1, entries_x1001);
    Object1003 object_x1003 = Object1003(OD_OBJECT_1003, 4099, 9, entries_x1003);
    Object object_x1005 = Object(OD_OBJECT_1005, 4101, 1, entries_x1005);
    Object object_x1006 = Object(OD_OBJECT_1006, 4102, 1, entries_x1006);
    Object object_x1007 = Object(OD_OBJECT_1007, 4103, 1, entries_x1007);
    Object object_x1008 = Object(OD_OBJECT_1008, 4104, 1, entries_x1008);
    Object object_x1009 = Object(OD_OBJECT_1009, 4105, 1, entries_x1009);
    Object object_x100A = Object(OD_OBJECT_100A, 4106, 1, entries_x100A);
    Object object_x100C = Object(OD_OBJECT_100C, 4108, 1, entries_x100C);
    Object object_x100D = Object(OD_OBJECT_100D, 4109, 1, entries_x100D);
    Object1010 object_x1010 = Object1010(OD_OBJECT_1010, 4112, 14, entries_x1010);
    Object1011 object_x1011 = Object1011(OD_OBJECT_1011, 4113, 14, entries_x1011);
    Object object_x1014 = Object(OD_OBJECT_1014, 4116, 1, entries_x1014);
    Object object_x1016 = Object(OD_OBJECT_1016, 4118, 2, entries_x1016);
    Object object_x1017 = Object(OD_OBJECT_1017, 4119, 1, entries_x1017);
    Object object_x1018 = Object(OD_OBJECT_1018, 4120, 5, entries_x1018);
    Object1019 object_x1019 = Object1019(OD_OBJECT_1019, 4121, 1, entries_x1019);
    Object object_x1020 = Object(OD_OBJECT_1020, 4128, 3, entries_x1020);
    Object object_x1029 = Object(OD_OBJECT_1029, 4137, 3, entries_x1029);
    Object1400 object_x1400 = Object1400(OD_OBJECT_1400, 5120, 6, entries_x1400);
    Object1400 object_x1401 = Object1400(OD_OBJECT_1401, 5121, 6, entries_x1401);
    Object1400 object_x1402 = Object1400(OD_OBJECT_1402, 5122, 6, entries_x1402);
    Object1400 object_x1403 = Object1400(OD_OBJECT_1403, 5123, 6, entries_x1403);
    Object1600 object_x1600 = Object1600(OD_OBJECT_1600, 5632, 9, entries_x1600);
    Object1600 object_x1601 = Object1600(OD_OBJECT_1601, 5633, 9, entries_x1601);
    Object1600 object_x1602 = Object1600(OD_OBJECT_1602, 5634, 9, entries_x1602);
    Object1600 object_x1603 = Object1600(OD_OBJECT_1603, 5635, 9, entries_x1603);
    Object1800 object_x1800 = Object1800(OD_OBJECT_1800, 6144, 7, entries_x1800);
    Object1800 object_x1801 = Object1800(OD_OBJECT_1801, 6145, 7, entries_x1801);
    Object1800 object_x1802 = Object1800(OD_OBJECT_1802, 6146, 7, entries_x1802);
    Object1800 object_x1803 = Object1800(OD_OBJECT_1803, 6147, 7, entries_x1803);
    Object1A00 object_x1A00 = Object1A00(OD_OBJECT_1A00, 6656, 9, entries_x1A00);
    Object1A00 object_x1A01 = Object1A00(OD_OBJECT_1A01, 6657, 9, entries_x1A01);
    Object1A00 object_x1A02 = Object1A00(OD_OBJECT_1A02, 6658, 9, entries_x1A02);
    Object1A00 object_x1A03 = Object1A00(OD_OBJECT_1A03, 6659, 9, entries_x1A03);
    Object object_x1F51 = Object(OD_OBJECT_1F51, 8017, 1, entries_x1F51);
    Object object_x1F57 = Object(OD_OBJECT_1F57, 8023, 3, entries_x1F57);
    Object object_x1F80 = Object(OD_OBJECT_1F80, 8064, 1, entries_x1F80);
    Object object_x6007 = Object(OD_OBJECT_6007, 24583, 1, entries_x6007);
    Object object_x603F = Object(OD_OBJECT_603F, 24639, 1, entries_x603F);
    Object object_x6040 = Object(OD_OBJECT_6040, 24640, 1, entries_x6040);
    Object object_x6041 = Object(OD_OBJECT_6041, 24641, 1, entries_x6041);
    Object object_x6042 = Object(OD_OBJECT_6042, 24642, 1, entries_x6042);
    Object object_x6043 = Object(OD_OBJECT_6043, 24643, 1, entries_x6043);
    Object object_x6044 = Object(OD_OBJECT_6044, 24644, 1, entries_x6044);
    Object object_x6046 = Object(OD_OBJECT_6046, 24646, 3, entries_x6046);
    Object object_x6048 = Object(OD_OBJECT_6048, 24648, 3, entries_x6048);
    Object object_x6049 = Object(OD_OBJECT_6049, 24649, 3, entries_x6049);
    Object object_x604A = Object(OD_OBJECT_604A, 24650, 3, entries_x604A);
    Object object_x6060 = Object(OD_OBJECT_6060, 24672, 1, entries_x6060);
    Object object_x6061 = Object(OD_OBJECT_6061, 24673, 1, entries_x6061);
    Object object_x6064 = Object(OD_OBJECT_6064, 24676, 1, entries_x6064);
    Object object_x607A = Object(OD_OBJECT_607A, 24698, 1, entries_x607A);
    Object object_x6081 = Object(OD_OBJECT_6081, 24705, 1, entries_x6081);
    Object object_x60FD = Object(OD_OBJECT_60FD, 24829, 1, entries_x60FD);
    Object object_x60FE = Object(OD_OBJECT_60FE, 24830, 2, entries_x60FE);
    Object *objectsArray[OD_OBJECTS_COUNT] = {&object_x2, &object_x3, &object_x4, &object_x5, &object_x6, &object_x7, &object_x1000, &object_x1001, &object_x1003, &object_x1005, &object_x1006, &object_x1007, &object_x1008, &object_x1009, &object_x100A, &object_x100C, &object_x100D, &object_x1010, &object_x1011, &object_x1014, &object_x1016, &object_x1017, &object_x1018, &object_x1019, &object_x1020, &object_x1029, &object_x1400, &object_x1401, &object_x1402, &object_x1403, &object_x1600, &object_x1601, &object_x1602, &object_x1603, &object_x1800, &object_x1801, &object_x1802, &object_x1803, &object_x1A00, &object_x1A01, &object_x1A02, &object_x1A03, &object_x1F51, &object_x1F57, &object_x1F80, &object_x6007, &object_x603F, &object_x6040, &object_x6041, &object_x6042, &object_x6043, &object_x6044, &object_x6046, &object_x6048, &object_x6049, &object_x604A, &object_x6060, &object_x6061, &object_x6064, &object_x607A, &object_x6081, &object_x60FD, &object_x60FE};
    class Node &node;
    // @endcond
public:
    const uint16_t length = OD_OBJECTS_COUNT;

    /**
     * Constructor for the Object Dictionary.
     * @param node The parent Node reference.
     * @param data The object dictionnary data structure reference.
     */
    ObjectDictionnary(class Node &node, ObjectDictionnaryData &data);

    /**
     * Find an object in the dictionary.
     * @param index The hex index of the object to find.
     * @return Pointer to the found object, or nullptr if not found.
     */
    Object *findObject(uint16_t index);

    /**
     * Access an object in the dictionary by array index.
     * @param index The array index of the object.
     * This parameter should always use one of the generated defines.
     * @return Pointer to the accessed object, or nullptr if not found.
     */
    Object *at(uint16_t index);

    /**
     * Access an object in the dictionary by array index using the subscript operator.
     * Under the hood, this method uses the at() method.
     * @param index The array index of the object to access.
     * @return Pointer to the accessed object, or nullptr if not found.
     */
    Object *operator[](uint16_t index);

    /**
     * Save data to non-volatile storage for a specified parameter group.
     * This method is called by object 0x1010.
     * @param parameterGroup The parameter group number.
     * @return True if saving was successful, false otherwise.
     */
    bool saveData(uint8_t parameterGroup);

    /**
     * Load data from non-volatile storage for a specified parameter group.
     * This method should be called on application startup to load the object dictionnary.
     * @param parameterGroup The parameter group number.
     * @return True if loading was successful, false otherwise.
     */
    bool loadData(uint8_t parameterGroup);

    /**
     * Restore data to default for a specified parameter group.
     * This method is called by object 0x1011.
     * @param parameterGroup The parameter group number.
     * @return True if restoring was successful, false otherwise.
     */
    bool restoreData(uint8_t parameterGroup);
};
}
#endif // _OD_DEFINES_ONLY
