#pragma once
#include <cstdint>
#include <cstdio>

#define OD_OBJECTS_COUNT 69 // Number of objects in the dictionnary

struct OD_Data // Struct containing all of the raw data that can be saved and loaded
{
    int8_t x2;
    int16_t x3;
    int32_t x4;
    uint8_t x5;
    uint16_t x6;
    uint32_t x7;
    uint32_t x1000;
    uint8_t x1001;
    uint8_t x1003sub0;
    uint32_t x1003[8];
    uint32_t x1005;
    uint32_t x1006;
    uint32_t x1007;
    uint16_t x100C;
    uint8_t x100D;
    uint8_t x1010sub0;
    uint32_t x1010[13];
    uint8_t x1011sub0;
    uint32_t x1011[13];
    uint32_t x1014;
    uint8_t x1016sub0;
    uint32_t x1016[1];
    uint16_t x1017;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
    } x1018;
    uint8_t x1019;
    uint8_t x1020sub0;
    uint32_t x1020[2];
    uint8_t x1029sub0;
    uint8_t x1029[2];
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1400;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1401;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1402;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1403;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1404;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1405;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1406;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
    } x1407;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1600;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1601;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1602;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1603;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1604;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1605;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1606;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1607;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1800;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1801;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1802;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1803;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1804;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1805;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1806;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint8_t sub2;
        uint16_t sub3;
        uint8_t sub4;
        uint16_t sub5;
        uint8_t sub6;
    } x1807;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A00;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A01;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A02;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A03;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A04;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A05;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A06;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint32_t sub2;
        uint32_t sub3;
        uint32_t sub4;
        uint32_t sub5;
        uint32_t sub6;
        uint32_t sub7;
        uint32_t sub8;
    } x1A07;
    uint8_t x1F51sub0;
    uint8_t x1F51[2];
    uint8_t x1F57sub0;
    uint32_t x1F57[2];
    uint32_t x1F80;
    int16_t x6007;
    uint16_t x603F;
    uint16_t x6040;
    uint16_t x6041;
    int16_t x6042;
    int16_t x6043;
    int16_t x6044;
    uint8_t x6046sub0;
    uint32_t x6046[2];
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint16_t sub2;
    } x6048;
    struct
    {
        uint8_t sub0;
        uint32_t sub1;
        uint16_t sub2;
    } x6049;
    struct
    {
        uint8_t sub0;
        uint64_t sub1;
        uint64_t sub2;
    } x604A;

    OD_Data() : x2(0), x3(0), x4(0), x5(0), x6(0), x7(0), x1000(131474), x1001(0), x1003sub0(9), x1003{0, 0, 0, 0, 0, 0, 0, 0}, x1005(128), x1006(0), x1007(0), x100C(0), x100D(0), x1010sub0(14), x1010{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, x1011sub0(14), x1011{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, x1014(129), x1016sub0(2), x1016{0}, x1017(0), x1018{4, 620, 70, 145031168, 0}, x1019(0), x1020sub0(3), x1020{0, 0}, x1029sub0(3), x1029{0, 1}, x1400{2, 513, 255}, x1401{2, 769, 255}, x1402{2, 1025, 255}, x1403{2, 1281, 255}, x1404{2, 2147483648, 255}, x1405{2, 2147483648, 255}, x1406{2, 2147483648, 255}, x1407{2, 2147483648, 255}, x1600{2, 1614807056, 1616904200, 0, 0, 0, 0, 0, 0}, x1601{2, 1618608160, 1619066912, 0, 0, 0, 0, 0, 0}, x1602{1, 1614938128, 0, 0, 0, 0, 0, 0, 0}, x1603{1, 1627259168, 0, 0, 0, 0, 0, 0, 0}, x1604{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1605{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1606{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1607{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1800{6, 385, 255, 100, 0, 0, 0}, x1801{6, 641, 255, 100, 0, 0, 0}, x1802{6, 897, 255, 100, 0, 0, 0}, x1803{6, 1153, 255, 100, 0, 0, 0}, x1804{6, 3221225472, 255, 100, 0, 0, 0}, x1805{6, 3221225472, 255, 100, 0, 0, 0}, x1806{6, 3221225472, 255, 100, 0, 0, 0}, x1807{6, 3221225472, 255, 100, 0, 0, 0}, x1A00{2, 1614872592, 1616969736, 0, 0, 0, 0, 0, 0}, x1A01{1, 1617166368, 0, 0, 0, 0, 0, 0, 0}, x1A02{1, 1615069200, 0, 0, 0, 0, 0, 0, 0}, x1A03{1, 1627193376, 0, 0, 0, 0, 0, 0, 0}, x1A04{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1A05{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1A06{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1A07{0, 0, 0, 0, 0, 0, 0, 0, 0}, x1F51sub0(3), x1F51{0, 0}, x1F57sub0(3), x1F57{0, 0}, x1F80(0), x6007(1), x603F(0), x6040(0), x6041(0), x6042(200), x6043(0), x6044(0), x6046sub0(3), x6046{0, 30000}, x6048{2, 500, 1}, x6049{2, 500, 1}, x604A{2, 5000, 1}
    {
        printf("[OD_Data] address of (uint64_t*)x604A.sub1: %p\n", &x604A.sub1);
    }
    void save();
    void load();
};

struct OD_Object
{
    const int accessType;
    const int dataType;
    const void *valueSrc;

    OD_Object(int accessType, int dataType, void *valueSrc) : accessType(accessType), dataType(dataType), valueSrc(valueSrc) {}
};

struct OD_Objects // Struct containing all of the dictionnary objects
{
    OD_Object obj_x2[1];
    OD_Object obj_x3[1];
    OD_Object obj_x4[1];
    OD_Object obj_x5[1];
    OD_Object obj_x6[1];
    OD_Object obj_x7[1];
    OD_Object obj_x1000[1];
    OD_Object obj_x1001[1];
    OD_Object obj_x1003[9];
    OD_Object obj_x1005[1];
    OD_Object obj_x1006[1];
    OD_Object obj_x1007[1];
    OD_Object obj_x100C[1];
    OD_Object obj_x100D[1];
    OD_Object obj_x1010[14];
    OD_Object obj_x1011[14];
    OD_Object obj_x1014[1];
    OD_Object obj_x1016[2];
    OD_Object obj_x1017[1];
    OD_Object obj_x1018[5];
    OD_Object obj_x1019[1];
    OD_Object obj_x1020[3];
    OD_Object obj_x1029[3];
    OD_Object obj_x1400[3];
    OD_Object obj_x1401[3];
    OD_Object obj_x1402[3];
    OD_Object obj_x1403[3];
    OD_Object obj_x1404[3];
    OD_Object obj_x1405[3];
    OD_Object obj_x1406[3];
    OD_Object obj_x1407[3];
    OD_Object obj_x1600[9];
    OD_Object obj_x1601[9];
    OD_Object obj_x1602[9];
    OD_Object obj_x1603[9];
    OD_Object obj_x1604[9];
    OD_Object obj_x1605[9];
    OD_Object obj_x1606[9];
    OD_Object obj_x1607[9];
    OD_Object obj_x1800[7];
    OD_Object obj_x1801[7];
    OD_Object obj_x1802[7];
    OD_Object obj_x1803[7];
    OD_Object obj_x1804[7];
    OD_Object obj_x1805[7];
    OD_Object obj_x1806[7];
    OD_Object obj_x1807[7];
    OD_Object obj_x1A00[9];
    OD_Object obj_x1A01[9];
    OD_Object obj_x1A02[9];
    OD_Object obj_x1A03[9];
    OD_Object obj_x1A04[9];
    OD_Object obj_x1A05[9];
    OD_Object obj_x1A06[9];
    OD_Object obj_x1A07[9];
    OD_Object obj_x1F51[3];
    OD_Object obj_x1F57[3];
    OD_Object obj_x1F80[1];
    OD_Object obj_x6007[1];
    OD_Object obj_x603F[1];
    OD_Object obj_x6040[1];
    OD_Object obj_x6041[1];
    OD_Object obj_x6042[1];
    OD_Object obj_x6043[1];
    OD_Object obj_x6044[1];
    OD_Object obj_x6046[3];
    OD_Object obj_x6048[3];
    OD_Object obj_x6049[3];
    OD_Object obj_x604A[3];

    OD_Objects(OD_Data &data) : obj_x2{OD_Object(0, 2, &data.x2)}, obj_x3{OD_Object(0, 3, &data.x3)}, obj_x4{OD_Object(0, 4, &data.x4)}, obj_x5{OD_Object(0, 5, &data.x5)}, obj_x6{OD_Object(0, 6, &data.x6)}, obj_x7{OD_Object(0, 7, &data.x7)}, obj_x1000{OD_Object(0, 7, &data.x1000)}, obj_x1001{OD_Object(0, 5, &data.x1001)}, obj_x1003{OD_Object(0, 5, &data.x1003sub0), OD_Object(0, 7, &data.x1003[0]), OD_Object(0, 7, &data.x1003[1]), OD_Object(0, 7, &data.x1003[2]), OD_Object(0, 7, &data.x1003[3]), OD_Object(0, 7, &data.x1003[4]), OD_Object(0, 7, &data.x1003[5]), OD_Object(0, 7, &data.x1003[6]), OD_Object(0, 7, &data.x1003[7])}, obj_x1005{OD_Object(0, 7, &data.x1005)}, obj_x1006{OD_Object(0, 7, &data.x1006)}, obj_x1007{OD_Object(0, 7, &data.x1007)}, obj_x100C{OD_Object(0, 6, &data.x100C)}, obj_x100D{OD_Object(0, 5, &data.x100D)}, obj_x1010{OD_Object(0, 5, &data.x1010sub0), OD_Object(0, 7, &data.x1010[0]), OD_Object(0, 7, &data.x1010[1]), OD_Object(0, 7, &data.x1010[2]), OD_Object(0, 7, &data.x1010[3]), OD_Object(0, 7, &data.x1010[4]), OD_Object(0, 7, &data.x1010[5]), OD_Object(0, 7, &data.x1010[6]), OD_Object(0, 7, &data.x1010[7]), OD_Object(0, 7, &data.x1010[8]), OD_Object(0, 7, &data.x1010[9]), OD_Object(0, 7, &data.x1010[10]), OD_Object(0, 7, &data.x1010[11]), OD_Object(0, 7, &data.x1010[12])}, obj_x1011{OD_Object(0, 5, &data.x1011sub0), OD_Object(0, 7, &data.x1011[0]), OD_Object(0, 7, &data.x1011[1]), OD_Object(0, 7, &data.x1011[2]), OD_Object(0, 7, &data.x1011[3]), OD_Object(0, 7, &data.x1011[4]), OD_Object(0, 7, &data.x1011[5]), OD_Object(0, 7, &data.x1011[6]), OD_Object(0, 7, &data.x1011[7]), OD_Object(0, 7, &data.x1011[8]), OD_Object(0, 7, &data.x1011[9]), OD_Object(0, 7, &data.x1011[10]), OD_Object(0, 7, &data.x1011[11]), OD_Object(0, 7, &data.x1011[12])}, obj_x1014{OD_Object(0, 7, &data.x1014)}, obj_x1016{OD_Object(0, 5, &data.x1016sub0), OD_Object(0, 7, &data.x1016[0])}, obj_x1017{OD_Object(0, 6, &data.x1017)}, obj_x1018{OD_Object(0, 5, &data.x1018.sub0), OD_Object(0, 7, &data.x1018.sub1), OD_Object(0, 7, &data.x1018.sub2), OD_Object(0, 7, &data.x1018.sub3), OD_Object(0, 7, &data.x1018.sub4)}, obj_x1019{OD_Object(0, 5, &data.x1019)}, obj_x1020{OD_Object(0, 5, &data.x1020sub0), OD_Object(0, 7, &data.x1020[0]), OD_Object(0, 7, &data.x1020[1])}, obj_x1029{OD_Object(0, 5, &data.x1029sub0), OD_Object(0, 5, &data.x1029[0]), OD_Object(0, 5, &data.x1029[1])}, obj_x1400{OD_Object(0, 5, &data.x1400.sub0), OD_Object(0, 7, &data.x1400.sub1), OD_Object(0, 5, &data.x1400.sub2)}, obj_x1401{OD_Object(0, 5, &data.x1401.sub0), OD_Object(0, 7, &data.x1401.sub1), OD_Object(0, 5, &data.x1401.sub2)}, obj_x1402{OD_Object(0, 5, &data.x1402.sub0), OD_Object(0, 7, &data.x1402.sub1), OD_Object(0, 5, &data.x1402.sub2)}, obj_x1403{OD_Object(0, 5, &data.x1403.sub0), OD_Object(0, 7, &data.x1403.sub1), OD_Object(0, 5, &data.x1403.sub2)}, obj_x1404{OD_Object(0, 5, &data.x1404.sub0), OD_Object(0, 7, &data.x1404.sub1), OD_Object(0, 5, &data.x1404.sub2)}, obj_x1405{OD_Object(0, 5, &data.x1405.sub0), OD_Object(0, 7, &data.x1405.sub1), OD_Object(0, 5, &data.x1405.sub2)}, obj_x1406{OD_Object(0, 5, &data.x1406.sub0), OD_Object(0, 7, &data.x1406.sub1), OD_Object(0, 5, &data.x1406.sub2)}, obj_x1407{OD_Object(0, 5, &data.x1407.sub0), OD_Object(0, 7, &data.x1407.sub1), OD_Object(0, 5, &data.x1407.sub2)}, obj_x1600{OD_Object(0, 5, &data.x1600.sub0), OD_Object(0, 7, &data.x1600.sub1), OD_Object(0, 7, &data.x1600.sub2), OD_Object(0, 7, &data.x1600.sub3), OD_Object(0, 7, &data.x1600.sub4), OD_Object(0, 7, &data.x1600.sub5), OD_Object(0, 7, &data.x1600.sub6), OD_Object(0, 7, &data.x1600.sub7), OD_Object(0, 7, &data.x1600.sub8)}, obj_x1601{OD_Object(0, 5, &data.x1601.sub0), OD_Object(0, 7, &data.x1601.sub1), OD_Object(0, 7, &data.x1601.sub2), OD_Object(0, 7, &data.x1601.sub3), OD_Object(0, 7, &data.x1601.sub4), OD_Object(0, 7, &data.x1601.sub5), OD_Object(0, 7, &data.x1601.sub6), OD_Object(0, 7, &data.x1601.sub7), OD_Object(0, 7, &data.x1601.sub8)}, obj_x1602{OD_Object(0, 5, &data.x1602.sub0), OD_Object(0, 7, &data.x1602.sub1), OD_Object(0, 7, &data.x1602.sub2), OD_Object(0, 7, &data.x1602.sub3), OD_Object(0, 7, &data.x1602.sub4), OD_Object(0, 7, &data.x1602.sub5), OD_Object(0, 7, &data.x1602.sub6), OD_Object(0, 7, &data.x1602.sub7), OD_Object(0, 7, &data.x1602.sub8)}, obj_x1603{OD_Object(0, 5, &data.x1603.sub0), OD_Object(0, 7, &data.x1603.sub1), OD_Object(0, 7, &data.x1603.sub2), OD_Object(0, 7, &data.x1603.sub3), OD_Object(0, 7, &data.x1603.sub4), OD_Object(0, 7, &data.x1603.sub5), OD_Object(0, 7, &data.x1603.sub6), OD_Object(0, 7, &data.x1603.sub7), OD_Object(0, 7, &data.x1603.sub8)}, obj_x1604{OD_Object(0, 5, &data.x1604.sub0), OD_Object(0, 7, &data.x1604.sub1), OD_Object(0, 7, &data.x1604.sub2), OD_Object(0, 7, &data.x1604.sub3), OD_Object(0, 7, &data.x1604.sub4), OD_Object(0, 7, &data.x1604.sub5), OD_Object(0, 7, &data.x1604.sub6), OD_Object(0, 7, &data.x1604.sub7), OD_Object(0, 7, &data.x1604.sub8)}, obj_x1605{OD_Object(0, 5, &data.x1605.sub0), OD_Object(0, 7, &data.x1605.sub1), OD_Object(0, 7, &data.x1605.sub2), OD_Object(0, 7, &data.x1605.sub3), OD_Object(0, 7, &data.x1605.sub4), OD_Object(0, 7, &data.x1605.sub5), OD_Object(0, 7, &data.x1605.sub6), OD_Object(0, 7, &data.x1605.sub7), OD_Object(0, 7, &data.x1605.sub8)}, obj_x1606{OD_Object(0, 5, &data.x1606.sub0), OD_Object(0, 7, &data.x1606.sub1), OD_Object(0, 7, &data.x1606.sub2), OD_Object(0, 7, &data.x1606.sub3), OD_Object(0, 7, &data.x1606.sub4), OD_Object(0, 7, &data.x1606.sub5), OD_Object(0, 7, &data.x1606.sub6), OD_Object(0, 7, &data.x1606.sub7), OD_Object(0, 7, &data.x1606.sub8)}, obj_x1607{OD_Object(0, 5, &data.x1607.sub0), OD_Object(0, 7, &data.x1607.sub1), OD_Object(0, 7, &data.x1607.sub2), OD_Object(0, 7, &data.x1607.sub3), OD_Object(0, 7, &data.x1607.sub4), OD_Object(0, 7, &data.x1607.sub5), OD_Object(0, 7, &data.x1607.sub6), OD_Object(0, 7, &data.x1607.sub7), OD_Object(0, 7, &data.x1607.sub8)}, obj_x1800{OD_Object(0, 5, &data.x1800.sub0), OD_Object(0, 7, &data.x1800.sub1), OD_Object(0, 5, &data.x1800.sub2), OD_Object(0, 6, &data.x1800.sub3), OD_Object(0, 5, &data.x1800.sub4), OD_Object(0, 6, &data.x1800.sub5), OD_Object(0, 5, &data.x1800.sub6)}, obj_x1801{OD_Object(0, 5, &data.x1801.sub0), OD_Object(0, 7, &data.x1801.sub1), OD_Object(0, 5, &data.x1801.sub2), OD_Object(0, 6, &data.x1801.sub3), OD_Object(0, 5, &data.x1801.sub4), OD_Object(0, 6, &data.x1801.sub5), OD_Object(0, 5, &data.x1801.sub6)}, obj_x1802{OD_Object(0, 5, &data.x1802.sub0), OD_Object(0, 7, &data.x1802.sub1), OD_Object(0, 5, &data.x1802.sub2), OD_Object(0, 6, &data.x1802.sub3), OD_Object(0, 5, &data.x1802.sub4), OD_Object(0, 6, &data.x1802.sub5), OD_Object(0, 5, &data.x1802.sub6)}, obj_x1803{OD_Object(0, 5, &data.x1803.sub0), OD_Object(0, 7, &data.x1803.sub1), OD_Object(0, 5, &data.x1803.sub2), OD_Object(0, 6, &data.x1803.sub3), OD_Object(0, 5, &data.x1803.sub4), OD_Object(0, 6, &data.x1803.sub5), OD_Object(0, 5, &data.x1803.sub6)}, obj_x1804{OD_Object(0, 5, &data.x1804.sub0), OD_Object(0, 7, &data.x1804.sub1), OD_Object(0, 5, &data.x1804.sub2), OD_Object(0, 6, &data.x1804.sub3), OD_Object(0, 5, &data.x1804.sub4), OD_Object(0, 6, &data.x1804.sub5), OD_Object(0, 5, &data.x1804.sub6)}, obj_x1805{OD_Object(0, 5, &data.x1805.sub0), OD_Object(0, 7, &data.x1805.sub1), OD_Object(0, 5, &data.x1805.sub2), OD_Object(0, 6, &data.x1805.sub3), OD_Object(0, 5, &data.x1805.sub4), OD_Object(0, 6, &data.x1805.sub5), OD_Object(0, 5, &data.x1805.sub6)}, obj_x1806{OD_Object(0, 5, &data.x1806.sub0), OD_Object(0, 7, &data.x1806.sub1), OD_Object(0, 5, &data.x1806.sub2), OD_Object(0, 6, &data.x1806.sub3), OD_Object(0, 5, &data.x1806.sub4), OD_Object(0, 6, &data.x1806.sub5), OD_Object(0, 5, &data.x1806.sub6)}, obj_x1807{OD_Object(0, 5, &data.x1807.sub0), OD_Object(0, 7, &data.x1807.sub1), OD_Object(0, 5, &data.x1807.sub2), OD_Object(0, 6, &data.x1807.sub3), OD_Object(0, 5, &data.x1807.sub4), OD_Object(0, 6, &data.x1807.sub5), OD_Object(0, 5, &data.x1807.sub6)}, obj_x1A00{OD_Object(0, 5, &data.x1A00.sub0), OD_Object(0, 7, &data.x1A00.sub1), OD_Object(0, 7, &data.x1A00.sub2), OD_Object(0, 7, &data.x1A00.sub3), OD_Object(0, 7, &data.x1A00.sub4), OD_Object(0, 7, &data.x1A00.sub5), OD_Object(0, 7, &data.x1A00.sub6), OD_Object(0, 7, &data.x1A00.sub7), OD_Object(0, 7, &data.x1A00.sub8)}, obj_x1A01{OD_Object(0, 5, &data.x1A01.sub0), OD_Object(0, 7, &data.x1A01.sub1), OD_Object(0, 7, &data.x1A01.sub2), OD_Object(0, 7, &data.x1A01.sub3), OD_Object(0, 7, &data.x1A01.sub4), OD_Object(0, 7, &data.x1A01.sub5), OD_Object(0, 7, &data.x1A01.sub6), OD_Object(0, 7, &data.x1A01.sub7), OD_Object(0, 7, &data.x1A01.sub8)}, obj_x1A02{OD_Object(0, 5, &data.x1A02.sub0), OD_Object(0, 7, &data.x1A02.sub1), OD_Object(0, 7, &data.x1A02.sub2), OD_Object(0, 7, &data.x1A02.sub3), OD_Object(0, 7, &data.x1A02.sub4), OD_Object(0, 7, &data.x1A02.sub5), OD_Object(0, 7, &data.x1A02.sub6), OD_Object(0, 7, &data.x1A02.sub7), OD_Object(0, 7, &data.x1A02.sub8)}, obj_x1A03{OD_Object(0, 5, &data.x1A03.sub0), OD_Object(0, 7, &data.x1A03.sub1), OD_Object(0, 7, &data.x1A03.sub2), OD_Object(0, 7, &data.x1A03.sub3), OD_Object(0, 7, &data.x1A03.sub4), OD_Object(0, 7, &data.x1A03.sub5), OD_Object(0, 7, &data.x1A03.sub6), OD_Object(0, 7, &data.x1A03.sub7), OD_Object(0, 7, &data.x1A03.sub8)}, obj_x1A04{OD_Object(0, 5, &data.x1A04.sub0), OD_Object(0, 7, &data.x1A04.sub1), OD_Object(0, 7, &data.x1A04.sub2), OD_Object(0, 7, &data.x1A04.sub3), OD_Object(0, 7, &data.x1A04.sub4), OD_Object(0, 7, &data.x1A04.sub5), OD_Object(0, 7, &data.x1A04.sub6), OD_Object(0, 7, &data.x1A04.sub7), OD_Object(0, 7, &data.x1A04.sub8)}, obj_x1A05{OD_Object(0, 5, &data.x1A05.sub0), OD_Object(0, 7, &data.x1A05.sub1), OD_Object(0, 7, &data.x1A05.sub2), OD_Object(0, 7, &data.x1A05.sub3), OD_Object(0, 7, &data.x1A05.sub4), OD_Object(0, 7, &data.x1A05.sub5), OD_Object(0, 7, &data.x1A05.sub6), OD_Object(0, 7, &data.x1A05.sub7), OD_Object(0, 7, &data.x1A05.sub8)}, obj_x1A06{OD_Object(0, 5, &data.x1A06.sub0), OD_Object(0, 7, &data.x1A06.sub1), OD_Object(0, 7, &data.x1A06.sub2), OD_Object(0, 7, &data.x1A06.sub3), OD_Object(0, 7, &data.x1A06.sub4), OD_Object(0, 7, &data.x1A06.sub5), OD_Object(0, 7, &data.x1A06.sub6), OD_Object(0, 7, &data.x1A06.sub7), OD_Object(0, 7, &data.x1A06.sub8)}, obj_x1A07{OD_Object(0, 5, &data.x1A07.sub0), OD_Object(0, 7, &data.x1A07.sub1), OD_Object(0, 7, &data.x1A07.sub2), OD_Object(0, 7, &data.x1A07.sub3), OD_Object(0, 7, &data.x1A07.sub4), OD_Object(0, 7, &data.x1A07.sub5), OD_Object(0, 7, &data.x1A07.sub6), OD_Object(0, 7, &data.x1A07.sub7), OD_Object(0, 7, &data.x1A07.sub8)}, obj_x1F51{OD_Object(0, 5, &data.x1F51sub0), OD_Object(0, 5, &data.x1F51[0]), OD_Object(0, 5, &data.x1F51[1])}, obj_x1F57{OD_Object(0, 5, &data.x1F57sub0), OD_Object(0, 7, &data.x1F57[0]), OD_Object(0, 7, &data.x1F57[1])}, obj_x1F80{OD_Object(0, 7, &data.x1F80)}, obj_x6007{OD_Object(0, 3, &data.x6007)}, obj_x603F{OD_Object(0, 6, &data.x603F)}, obj_x6040{OD_Object(0, 6, &data.x6040)}, obj_x6041{OD_Object(0, 6, &data.x6041)}, obj_x6042{OD_Object(0, 3, &data.x6042)}, obj_x6043{OD_Object(0, 3, &data.x6043)}, obj_x6044{OD_Object(0, 3, &data.x6044)}, obj_x6046{OD_Object(0, 5, &data.x6046sub0), OD_Object(0, 7, &data.x6046[0]), OD_Object(0, 7, &data.x6046[1])}, obj_x6048{OD_Object(0, 5, &data.x6048.sub0), OD_Object(0, 7, &data.x6048.sub1), OD_Object(0, 6, &data.x6048.sub2)}, obj_x6049{OD_Object(0, 5, &data.x6049.sub0), OD_Object(0, 7, &data.x6049.sub1), OD_Object(0, 6, &data.x6049.sub2)}, obj_x604A{OD_Object(0, 5, &data.x604A.sub0), OD_Object(0, 27, &data.x604A.sub1), OD_Object(0, 27, &data.x604A.sub2)}
    {
        printf("[OD_Object] address of (const void*)x604A.sub1: %p\n", obj_x604A[1].valueSrc);
    }
};

struct OD_ObjectEntry // Struct representing a dictionnary entry
{
    const int index;
    const int objectType;
    const int subNumber;
    const OD_Object *objects;

    OD_ObjectEntry(int index, int objectType, int subNumber, OD_Object *objects) : index(index), objectType(objectType), subNumber(subNumber), objects(objects) {}
    int getSize(uint8_t subIndex);
};

class OD_ObjectDictionnary // Object dictionnary class that is usable for the user
{
public:
    const int size;
    OD_Data data;
    OD_Objects objects;
    OD_ObjectEntry entries[OD_OBJECTS_COUNT];

public:
    OD_ObjectDictionnary() : size(OD_OBJECTS_COUNT), objects(data), entries{OD_ObjectEntry(2, 7, 1, objects.obj_x2), OD_ObjectEntry(3, 7, 1, objects.obj_x3), OD_ObjectEntry(4, 7, 1, objects.obj_x4), OD_ObjectEntry(5, 7, 1, objects.obj_x5), OD_ObjectEntry(6, 7, 1, objects.obj_x6), OD_ObjectEntry(7, 7, 1, objects.obj_x7), OD_ObjectEntry(4096, 7, 1, objects.obj_x1000), OD_ObjectEntry(4097, 7, 1, objects.obj_x1001), OD_ObjectEntry(4099, 8, 9, objects.obj_x1003), OD_ObjectEntry(4101, 7, 1, objects.obj_x1005), OD_ObjectEntry(4102, 7, 1, objects.obj_x1006), OD_ObjectEntry(4103, 7, 1, objects.obj_x1007), OD_ObjectEntry(4108, 7, 1, objects.obj_x100C), OD_ObjectEntry(4109, 7, 1, objects.obj_x100D), OD_ObjectEntry(4112, 8, 14, objects.obj_x1010), OD_ObjectEntry(4113, 8, 14, objects.obj_x1011), OD_ObjectEntry(4116, 7, 1, objects.obj_x1014), OD_ObjectEntry(4118, 8, 2, objects.obj_x1016), OD_ObjectEntry(4119, 7, 1, objects.obj_x1017), OD_ObjectEntry(4120, 9, 5, objects.obj_x1018), OD_ObjectEntry(4121, 7, 1, objects.obj_x1019), OD_ObjectEntry(4128, 8, 3, objects.obj_x1020), OD_ObjectEntry(4137, 8, 3, objects.obj_x1029), OD_ObjectEntry(5120, 9, 3, objects.obj_x1400), OD_ObjectEntry(5121, 9, 3, objects.obj_x1401), OD_ObjectEntry(5122, 9, 3, objects.obj_x1402), OD_ObjectEntry(5123, 9, 3, objects.obj_x1403), OD_ObjectEntry(5124, 9, 3, objects.obj_x1404), OD_ObjectEntry(5125, 9, 3, objects.obj_x1405), OD_ObjectEntry(5126, 9, 3, objects.obj_x1406), OD_ObjectEntry(5127, 9, 3, objects.obj_x1407), OD_ObjectEntry(5632, 9, 9, objects.obj_x1600), OD_ObjectEntry(5633, 9, 9, objects.obj_x1601), OD_ObjectEntry(5634, 9, 9, objects.obj_x1602), OD_ObjectEntry(5635, 9, 9, objects.obj_x1603), OD_ObjectEntry(5636, 9, 9, objects.obj_x1604), OD_ObjectEntry(5637, 9, 9, objects.obj_x1605), OD_ObjectEntry(5638, 9, 9, objects.obj_x1606), OD_ObjectEntry(5639, 9, 9, objects.obj_x1607), OD_ObjectEntry(6144, 9, 7, objects.obj_x1800), OD_ObjectEntry(6145, 9, 7, objects.obj_x1801), OD_ObjectEntry(6146, 9, 7, objects.obj_x1802), OD_ObjectEntry(6147, 9, 7, objects.obj_x1803), OD_ObjectEntry(6148, 9, 7, objects.obj_x1804), OD_ObjectEntry(6149, 9, 7, objects.obj_x1805), OD_ObjectEntry(6150, 9, 7, objects.obj_x1806), OD_ObjectEntry(6151, 9, 7, objects.obj_x1807), OD_ObjectEntry(6656, 9, 9, objects.obj_x1A00), OD_ObjectEntry(6657, 9, 9, objects.obj_x1A01), OD_ObjectEntry(6658, 9, 9, objects.obj_x1A02), OD_ObjectEntry(6659, 9, 9, objects.obj_x1A03), OD_ObjectEntry(6660, 9, 9, objects.obj_x1A04), OD_ObjectEntry(6661, 9, 9, objects.obj_x1A05), OD_ObjectEntry(6662, 9, 9, objects.obj_x1A06), OD_ObjectEntry(6663, 9, 9, objects.obj_x1A07), OD_ObjectEntry(8017, 8, 3, objects.obj_x1F51), OD_ObjectEntry(8023, 8, 3, objects.obj_x1F57), OD_ObjectEntry(8064, 7, 1, objects.obj_x1F80), OD_ObjectEntry(24583, 7, 1, objects.obj_x6007), OD_ObjectEntry(24639, 7, 1, objects.obj_x603F), OD_ObjectEntry(24640, 7, 1, objects.obj_x6040), OD_ObjectEntry(24641, 7, 1, objects.obj_x6041), OD_ObjectEntry(24642, 7, 1, objects.obj_x6042), OD_ObjectEntry(24643, 7, 1, objects.obj_x6043), OD_ObjectEntry(24644, 7, 1, objects.obj_x6044), OD_ObjectEntry(24646, 8, 3, objects.obj_x6046), OD_ObjectEntry(24648, 9, 3, objects.obj_x6048), OD_ObjectEntry(24649, 9, 3, objects.obj_x6049), OD_ObjectEntry(24650, 9, 3, objects.obj_x604A)} {}
    inline void save() { return data.save(); }
    inline void load() { return data.load(); }
    inline OD_Objects &getObjects() { return objects; }
    OD_ObjectEntry *findEntry(uint16_t index);
};