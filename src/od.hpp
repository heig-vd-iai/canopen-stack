#pragma once
#include <cstdint>

union AccessType
{
    uint8_t value;
    struct 
    {
        uint8_t r: 1; 
        uint8_t w: 1; 
        uint8_t constant: 1; 
    } bits;
};

#define OD_OBJECT_COUNT 56
#define OD_RPDO_COUNT 4
#define OD_TPDO_COUNT 4
#define OD_TPDO_LONGEST_MAP 2
#define OD_RPDO_COMMUNICATION_INDEX 0x1400
#define OD_RPDO_MAPPING_INDEX 0x1600
#define OD_TPDO_COMMUNICATION_INDEX 0x1800
#define OD_TPDO_MAPPING_INDEX 0x1A00


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
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4;} x1018;
    uint8_t x1019;
    uint8_t x1020sub0;
    uint32_t x1020[2];
    uint8_t x1029sub0;
    uint8_t x1029[2];
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2;} x1400;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2;} x1401;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2;} x1402;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2;} x1403;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1600;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1601;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1602;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1603;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2; uint16_t sub3; uint8_t sub4; uint16_t sub5; uint8_t sub6;} x1800;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2; uint16_t sub3; uint8_t sub4; uint16_t sub5; uint8_t sub6;} x1801;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2; uint16_t sub3; uint8_t sub4; uint16_t sub5; uint8_t sub6;} x1802;
    struct {uint8_t sub0; uint32_t sub1; uint8_t sub2; uint16_t sub3; uint8_t sub4; uint16_t sub5; uint8_t sub6;} x1803;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1A00;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1A01;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1A02;
    struct {uint8_t sub0; uint32_t sub1; uint32_t sub2; uint32_t sub3; uint32_t sub4; uint32_t sub5; uint32_t sub6; uint32_t sub7; uint32_t sub8;} x1A03;
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
    struct {uint8_t sub0; double sub1; float sub2;} x6048;
    struct {uint8_t sub0; uint32_t sub1; uint16_t sub2;} x6049;
    struct {uint8_t sub0; uint64_t sub1; uint64_t sub2;} x604A;
    int8_t x6061;
    int32_t x6064;
    uint32_t x60FD;

    OD_Data() : x2(0), x3(0), x4(0), x5(0), x6(0), x7(0), x1000(131474), x1001(0), x1003sub0(9), x1003{0, 0, 0, 0, 0, 0, 0, 0}, x1005(128), x1006(0), x1007(0), x100C(0), x100D(0), x1010sub0(14), x1010{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, x1011sub0(14), x1011{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, x1014(129), x1016sub0(2), x1016{0}, x1017(0), x1018{4, 620, 70, 145031168, 0}, x1019(0), x1020sub0(3), x1020{0, 0}, x1029sub0(3), x1029{0, 1}, x1400{2, 513, 255}, x1401{2, 769, 255}, x1402{2, 1025, 255}, x1403{2, 1281, 255}, x1600{2, 1614807056, 1616904200, 0, 0, 0, 0, 0, 0}, x1601{2, 1618608160, 1619066912, 0, 0, 0, 0, 0, 0}, x1602{1, 1614938128, 0, 0, 0, 0, 0, 0, 0}, x1603{1, 1627259168, 0, 0, 0, 0, 0, 0, 0}, x1800{6, 385, 255, 100, 0, 0, 0}, x1801{6, 641, 255, 100, 0, 0, 0}, x1802{6, 897, 255, 100, 0, 0, 0}, x1803{6, 1153, 255, 100, 0, 0, 0}, x1A00{2, 1614872592, 1616969736, 0, 0, 0, 0, 0, 0}, x1A01{1, 1617166368, 0, 0, 0, 0, 0, 0, 0}, x1A02{1, 1615069200, 0, 0, 0, 0, 0, 0, 0}, x1A03{1, 1627193376, 0, 0, 0, 0, 0, 0, 0}, x1F51sub0(3), x1F51{0, 0}, x1F57sub0(3), x1F57{0, 0}, x1F80(0), x6007(1), x603F(0), x6040(0), x6041(0), x6042(200), x6043(0), x6044(0), x6046sub0(3), x6046{0, 30000}, x6048{2, 500.0, 1.0}, x6049{2, 500, 1}, x604A{2, 5000, 1}, x6061(0), x6064(0), x60FD(0){}
    void save();
    void load();
};

struct OD_Object
{
    const AccessType accessType;
    const uint8_t dataType;
    const uint8_t size;
    const void *valueSrc;

    OD_Object(uint8_t accessType, uint8_t dataType, uint8_t size, void *valueSrc) : accessType{accessType}, dataType(dataType), size(size), valueSrc(valueSrc) {}
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
    OD_Object obj_x1600[9];
    OD_Object obj_x1601[9];
    OD_Object obj_x1602[9];
    OD_Object obj_x1603[9];
    OD_Object obj_x1800[7];
    OD_Object obj_x1801[7];
    OD_Object obj_x1802[7];
    OD_Object obj_x1803[7];
    OD_Object obj_x1A00[9];
    OD_Object obj_x1A01[9];
    OD_Object obj_x1A02[9];
    OD_Object obj_x1A03[9];
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
    OD_Object obj_x6061[1];
    OD_Object obj_x6064[1];
    OD_Object obj_x60FD[1];

    OD_Objects(OD_Data &data) : obj_x2{OD_Object(5, 2, sizeof(data.x2), &data.x2)}, obj_x3{OD_Object(5, 3, sizeof(data.x3), &data.x3)}, obj_x4{OD_Object(5, 4, sizeof(data.x4), &data.x4)}, obj_x5{OD_Object(5, 5, sizeof(data.x5), &data.x5)}, obj_x6{OD_Object(5, 6, sizeof(data.x6), &data.x6)}, obj_x7{OD_Object(5, 7, sizeof(data.x7), &data.x7)}, obj_x1000{OD_Object(1, 7, sizeof(data.x1000), &data.x1000)}, obj_x1001{OD_Object(1, 5, sizeof(data.x1001), &data.x1001)}, obj_x1003{OD_Object(3, 5, sizeof(data.x1003sub0), &data.x1003sub0), OD_Object(1, 7, sizeof(data.x1003[0]), &data.x1003[0]), OD_Object(1, 7, sizeof(data.x1003[1]), &data.x1003[1]), OD_Object(1, 7, sizeof(data.x1003[2]), &data.x1003[2]), OD_Object(1, 7, sizeof(data.x1003[3]), &data.x1003[3]), OD_Object(1, 7, sizeof(data.x1003[4]), &data.x1003[4]), OD_Object(1, 7, sizeof(data.x1003[5]), &data.x1003[5]), OD_Object(1, 7, sizeof(data.x1003[6]), &data.x1003[6]), OD_Object(1, 7, sizeof(data.x1003[7]), &data.x1003[7])}, obj_x1005{OD_Object(3, 7, sizeof(data.x1005), &data.x1005)}, obj_x1006{OD_Object(3, 7, sizeof(data.x1006), &data.x1006)}, obj_x1007{OD_Object(3, 7, sizeof(data.x1007), &data.x1007)}, obj_x100C{OD_Object(3, 6, sizeof(data.x100C), &data.x100C)}, obj_x100D{OD_Object(3, 5, sizeof(data.x100D), &data.x100D)}, obj_x1010{OD_Object(1, 5, sizeof(data.x1010sub0), &data.x1010sub0), OD_Object(3, 7, sizeof(data.x1010[0]), &data.x1010[0]), OD_Object(3, 7, sizeof(data.x1010[1]), &data.x1010[1]), OD_Object(3, 7, sizeof(data.x1010[2]), &data.x1010[2]), OD_Object(3, 7, sizeof(data.x1010[3]), &data.x1010[3]), OD_Object(3, 7, sizeof(data.x1010[4]), &data.x1010[4]), OD_Object(3, 7, sizeof(data.x1010[5]), &data.x1010[5]), OD_Object(3, 7, sizeof(data.x1010[6]), &data.x1010[6]), OD_Object(3, 7, sizeof(data.x1010[7]), &data.x1010[7]), OD_Object(3, 7, sizeof(data.x1010[8]), &data.x1010[8]), OD_Object(3, 7, sizeof(data.x1010[9]), &data.x1010[9]), OD_Object(3, 7, sizeof(data.x1010[10]), &data.x1010[10]), OD_Object(3, 7, sizeof(data.x1010[11]), &data.x1010[11]), OD_Object(3, 7, sizeof(data.x1010[12]), &data.x1010[12])}, obj_x1011{OD_Object(1, 5, sizeof(data.x1011sub0), &data.x1011sub0), OD_Object(3, 7, sizeof(data.x1011[0]), &data.x1011[0]), OD_Object(3, 7, sizeof(data.x1011[1]), &data.x1011[1]), OD_Object(3, 7, sizeof(data.x1011[2]), &data.x1011[2]), OD_Object(3, 7, sizeof(data.x1011[3]), &data.x1011[3]), OD_Object(3, 7, sizeof(data.x1011[4]), &data.x1011[4]), OD_Object(3, 7, sizeof(data.x1011[5]), &data.x1011[5]), OD_Object(3, 7, sizeof(data.x1011[6]), &data.x1011[6]), OD_Object(3, 7, sizeof(data.x1011[7]), &data.x1011[7]), OD_Object(3, 7, sizeof(data.x1011[8]), &data.x1011[8]), OD_Object(3, 7, sizeof(data.x1011[9]), &data.x1011[9]), OD_Object(3, 7, sizeof(data.x1011[10]), &data.x1011[10]), OD_Object(3, 7, sizeof(data.x1011[11]), &data.x1011[11]), OD_Object(3, 7, sizeof(data.x1011[12]), &data.x1011[12])}, obj_x1014{OD_Object(3, 7, sizeof(data.x1014), &data.x1014)}, obj_x1016{OD_Object(1, 5, sizeof(data.x1016sub0), &data.x1016sub0), OD_Object(3, 7, sizeof(data.x1016[0]), &data.x1016[0])}, obj_x1017{OD_Object(3, 6, sizeof(data.x1017), &data.x1017)}, obj_x1018{OD_Object(1, 5, sizeof(data.x1018.sub0), &data.x1018.sub0), OD_Object(1, 7, sizeof(data.x1018.sub1), &data.x1018.sub1), OD_Object(1, 7, sizeof(data.x1018.sub2), &data.x1018.sub2), OD_Object(1, 7, sizeof(data.x1018.sub3), &data.x1018.sub3), OD_Object(1, 7, sizeof(data.x1018.sub4), &data.x1018.sub4)}, obj_x1019{OD_Object(3, 5, sizeof(data.x1019), &data.x1019)}, obj_x1020{OD_Object(1, 5, sizeof(data.x1020sub0), &data.x1020sub0), OD_Object(3, 7, sizeof(data.x1020[0]), &data.x1020[0]), OD_Object(3, 7, sizeof(data.x1020[1]), &data.x1020[1])}, obj_x1029{OD_Object(1, 5, sizeof(data.x1029sub0), &data.x1029sub0), OD_Object(3, 5, sizeof(data.x1029[0]), &data.x1029[0]), OD_Object(3, 5, sizeof(data.x1029[1]), &data.x1029[1])}, obj_x1400{OD_Object(1, 5, sizeof(data.x1400.sub0), &data.x1400.sub0), OD_Object(3, 7, sizeof(data.x1400.sub1), &data.x1400.sub1), OD_Object(3, 5, sizeof(data.x1400.sub2), &data.x1400.sub2)}, obj_x1401{OD_Object(1, 5, sizeof(data.x1401.sub0), &data.x1401.sub0), OD_Object(3, 7, sizeof(data.x1401.sub1), &data.x1401.sub1), OD_Object(3, 5, sizeof(data.x1401.sub2), &data.x1401.sub2)}, obj_x1402{OD_Object(1, 5, sizeof(data.x1402.sub0), &data.x1402.sub0), OD_Object(3, 7, sizeof(data.x1402.sub1), &data.x1402.sub1), OD_Object(3, 5, sizeof(data.x1402.sub2), &data.x1402.sub2)}, obj_x1403{OD_Object(1, 5, sizeof(data.x1403.sub0), &data.x1403.sub0), OD_Object(3, 7, sizeof(data.x1403.sub1), &data.x1403.sub1), OD_Object(3, 5, sizeof(data.x1403.sub2), &data.x1403.sub2)}, obj_x1600{OD_Object(3, 5, sizeof(data.x1600.sub0), &data.x1600.sub0), OD_Object(3, 7, sizeof(data.x1600.sub1), &data.x1600.sub1), OD_Object(3, 7, sizeof(data.x1600.sub2), &data.x1600.sub2), OD_Object(3, 7, sizeof(data.x1600.sub3), &data.x1600.sub3), OD_Object(3, 7, sizeof(data.x1600.sub4), &data.x1600.sub4), OD_Object(3, 7, sizeof(data.x1600.sub5), &data.x1600.sub5), OD_Object(3, 7, sizeof(data.x1600.sub6), &data.x1600.sub6), OD_Object(3, 7, sizeof(data.x1600.sub7), &data.x1600.sub7), OD_Object(3, 7, sizeof(data.x1600.sub8), &data.x1600.sub8)}, obj_x1601{OD_Object(3, 5, sizeof(data.x1601.sub0), &data.x1601.sub0), OD_Object(3, 7, sizeof(data.x1601.sub1), &data.x1601.sub1), OD_Object(3, 7, sizeof(data.x1601.sub2), &data.x1601.sub2), OD_Object(3, 7, sizeof(data.x1601.sub3), &data.x1601.sub3), OD_Object(3, 7, sizeof(data.x1601.sub4), &data.x1601.sub4), OD_Object(3, 7, sizeof(data.x1601.sub5), &data.x1601.sub5), OD_Object(3, 7, sizeof(data.x1601.sub6), &data.x1601.sub6), OD_Object(3, 7, sizeof(data.x1601.sub7), &data.x1601.sub7), OD_Object(3, 7, sizeof(data.x1601.sub8), &data.x1601.sub8)}, obj_x1602{OD_Object(3, 5, sizeof(data.x1602.sub0), &data.x1602.sub0), OD_Object(3, 7, sizeof(data.x1602.sub1), &data.x1602.sub1), OD_Object(3, 7, sizeof(data.x1602.sub2), &data.x1602.sub2), OD_Object(3, 7, sizeof(data.x1602.sub3), &data.x1602.sub3), OD_Object(3, 7, sizeof(data.x1602.sub4), &data.x1602.sub4), OD_Object(3, 7, sizeof(data.x1602.sub5), &data.x1602.sub5), OD_Object(3, 7, sizeof(data.x1602.sub6), &data.x1602.sub6), OD_Object(3, 7, sizeof(data.x1602.sub7), &data.x1602.sub7), OD_Object(3, 7, sizeof(data.x1602.sub8), &data.x1602.sub8)}, obj_x1603{OD_Object(3, 5, sizeof(data.x1603.sub0), &data.x1603.sub0), OD_Object(3, 7, sizeof(data.x1603.sub1), &data.x1603.sub1), OD_Object(3, 7, sizeof(data.x1603.sub2), &data.x1603.sub2), OD_Object(3, 7, sizeof(data.x1603.sub3), &data.x1603.sub3), OD_Object(3, 7, sizeof(data.x1603.sub4), &data.x1603.sub4), OD_Object(3, 7, sizeof(data.x1603.sub5), &data.x1603.sub5), OD_Object(3, 7, sizeof(data.x1603.sub6), &data.x1603.sub6), OD_Object(3, 7, sizeof(data.x1603.sub7), &data.x1603.sub7), OD_Object(3, 7, sizeof(data.x1603.sub8), &data.x1603.sub8)}, obj_x1800{OD_Object(1, 5, sizeof(data.x1800.sub0), &data.x1800.sub0), OD_Object(3, 7, sizeof(data.x1800.sub1), &data.x1800.sub1), OD_Object(3, 5, sizeof(data.x1800.sub2), &data.x1800.sub2), OD_Object(3, 6, sizeof(data.x1800.sub3), &data.x1800.sub3), OD_Object(3, 5, sizeof(data.x1800.sub4), &data.x1800.sub4), OD_Object(3, 6, sizeof(data.x1800.sub5), &data.x1800.sub5), OD_Object(3, 5, sizeof(data.x1800.sub6), &data.x1800.sub6)}, obj_x1801{OD_Object(1, 5, sizeof(data.x1801.sub0), &data.x1801.sub0), OD_Object(3, 7, sizeof(data.x1801.sub1), &data.x1801.sub1), OD_Object(3, 5, sizeof(data.x1801.sub2), &data.x1801.sub2), OD_Object(3, 6, sizeof(data.x1801.sub3), &data.x1801.sub3), OD_Object(3, 5, sizeof(data.x1801.sub4), &data.x1801.sub4), OD_Object(3, 6, sizeof(data.x1801.sub5), &data.x1801.sub5), OD_Object(3, 5, sizeof(data.x1801.sub6), &data.x1801.sub6)}, obj_x1802{OD_Object(1, 5, sizeof(data.x1802.sub0), &data.x1802.sub0), OD_Object(3, 7, sizeof(data.x1802.sub1), &data.x1802.sub1), OD_Object(3, 5, sizeof(data.x1802.sub2), &data.x1802.sub2), OD_Object(3, 6, sizeof(data.x1802.sub3), &data.x1802.sub3), OD_Object(3, 5, sizeof(data.x1802.sub4), &data.x1802.sub4), OD_Object(3, 6, sizeof(data.x1802.sub5), &data.x1802.sub5), OD_Object(3, 5, sizeof(data.x1802.sub6), &data.x1802.sub6)}, obj_x1803{OD_Object(1, 5, sizeof(data.x1803.sub0), &data.x1803.sub0), OD_Object(3, 7, sizeof(data.x1803.sub1), &data.x1803.sub1), OD_Object(3, 5, sizeof(data.x1803.sub2), &data.x1803.sub2), OD_Object(3, 6, sizeof(data.x1803.sub3), &data.x1803.sub3), OD_Object(3, 5, sizeof(data.x1803.sub4), &data.x1803.sub4), OD_Object(3, 6, sizeof(data.x1803.sub5), &data.x1803.sub5), OD_Object(3, 5, sizeof(data.x1803.sub6), &data.x1803.sub6)}, obj_x1A00{OD_Object(3, 5, sizeof(data.x1A00.sub0), &data.x1A00.sub0), OD_Object(3, 7, sizeof(data.x1A00.sub1), &data.x1A00.sub1), OD_Object(3, 7, sizeof(data.x1A00.sub2), &data.x1A00.sub2), OD_Object(3, 7, sizeof(data.x1A00.sub3), &data.x1A00.sub3), OD_Object(3, 7, sizeof(data.x1A00.sub4), &data.x1A00.sub4), OD_Object(3, 7, sizeof(data.x1A00.sub5), &data.x1A00.sub5), OD_Object(3, 7, sizeof(data.x1A00.sub6), &data.x1A00.sub6), OD_Object(3, 7, sizeof(data.x1A00.sub7), &data.x1A00.sub7), OD_Object(3, 7, sizeof(data.x1A00.sub8), &data.x1A00.sub8)}, obj_x1A01{OD_Object(3, 5, sizeof(data.x1A01.sub0), &data.x1A01.sub0), OD_Object(3, 7, sizeof(data.x1A01.sub1), &data.x1A01.sub1), OD_Object(3, 7, sizeof(data.x1A01.sub2), &data.x1A01.sub2), OD_Object(3, 7, sizeof(data.x1A01.sub3), &data.x1A01.sub3), OD_Object(3, 7, sizeof(data.x1A01.sub4), &data.x1A01.sub4), OD_Object(3, 7, sizeof(data.x1A01.sub5), &data.x1A01.sub5), OD_Object(3, 7, sizeof(data.x1A01.sub6), &data.x1A01.sub6), OD_Object(3, 7, sizeof(data.x1A01.sub7), &data.x1A01.sub7), OD_Object(3, 7, sizeof(data.x1A01.sub8), &data.x1A01.sub8)}, obj_x1A02{OD_Object(3, 5, sizeof(data.x1A02.sub0), &data.x1A02.sub0), OD_Object(3, 7, sizeof(data.x1A02.sub1), &data.x1A02.sub1), OD_Object(3, 7, sizeof(data.x1A02.sub2), &data.x1A02.sub2), OD_Object(3, 7, sizeof(data.x1A02.sub3), &data.x1A02.sub3), OD_Object(3, 7, sizeof(data.x1A02.sub4), &data.x1A02.sub4), OD_Object(3, 7, sizeof(data.x1A02.sub5), &data.x1A02.sub5), OD_Object(3, 7, sizeof(data.x1A02.sub6), &data.x1A02.sub6), OD_Object(3, 7, sizeof(data.x1A02.sub7), &data.x1A02.sub7), OD_Object(3, 7, sizeof(data.x1A02.sub8), &data.x1A02.sub8)}, obj_x1A03{OD_Object(3, 5, sizeof(data.x1A03.sub0), &data.x1A03.sub0), OD_Object(3, 7, sizeof(data.x1A03.sub1), &data.x1A03.sub1), OD_Object(3, 7, sizeof(data.x1A03.sub2), &data.x1A03.sub2), OD_Object(3, 7, sizeof(data.x1A03.sub3), &data.x1A03.sub3), OD_Object(3, 7, sizeof(data.x1A03.sub4), &data.x1A03.sub4), OD_Object(3, 7, sizeof(data.x1A03.sub5), &data.x1A03.sub5), OD_Object(3, 7, sizeof(data.x1A03.sub6), &data.x1A03.sub6), OD_Object(3, 7, sizeof(data.x1A03.sub7), &data.x1A03.sub7), OD_Object(3, 7, sizeof(data.x1A03.sub8), &data.x1A03.sub8)}, obj_x1F51{OD_Object(1, 5, sizeof(data.x1F51sub0), &data.x1F51sub0), OD_Object(3, 5, sizeof(data.x1F51[0]), &data.x1F51[0]), OD_Object(3, 5, sizeof(data.x1F51[1]), &data.x1F51[1])}, obj_x1F57{OD_Object(1, 5, sizeof(data.x1F57sub0), &data.x1F57sub0), OD_Object(1, 7, sizeof(data.x1F57[0]), &data.x1F57[0]), OD_Object(1, 7, sizeof(data.x1F57[1]), &data.x1F57[1])}, obj_x1F80{OD_Object(3, 7, sizeof(data.x1F80), &data.x1F80)}, obj_x6007{OD_Object(3, 3, sizeof(data.x6007), &data.x6007)}, obj_x603F{OD_Object(1, 6, sizeof(data.x603F), &data.x603F)}, obj_x6040{OD_Object(3, 6, sizeof(data.x6040), &data.x6040)}, obj_x6041{OD_Object(1, 6, sizeof(data.x6041), &data.x6041)}, obj_x6042{OD_Object(3, 3, sizeof(data.x6042), &data.x6042)}, obj_x6043{OD_Object(1, 3, sizeof(data.x6043), &data.x6043)}, obj_x6044{OD_Object(1, 3, sizeof(data.x6044), &data.x6044)}, obj_x6046{OD_Object(5, 5, sizeof(data.x6046sub0), &data.x6046sub0), OD_Object(3, 7, sizeof(data.x6046[0]), &data.x6046[0]), OD_Object(3, 7, sizeof(data.x6046[1]), &data.x6046[1])}, obj_x6048{OD_Object(5, 5, sizeof(data.x6048.sub0), &data.x6048.sub0), OD_Object(3, 17, sizeof(data.x6048.sub1), &data.x6048.sub1), OD_Object(3, 8, sizeof(data.x6048.sub2), &data.x6048.sub2)}, obj_x6049{OD_Object(5, 5, sizeof(data.x6049.sub0), &data.x6049.sub0), OD_Object(1, 7, sizeof(data.x6049.sub1), &data.x6049.sub1), OD_Object(2, 6, sizeof(data.x6049.sub2), &data.x6049.sub2)}, obj_x604A{OD_Object(5, 5, sizeof(data.x604A.sub0), &data.x604A.sub0), OD_Object(3, 27, sizeof(data.x604A.sub1), &data.x604A.sub1), OD_Object(3, 27, sizeof(data.x604A.sub2), &data.x604A.sub2)}, obj_x6061{OD_Object(1, 2, sizeof(data.x6061), &data.x6061)}, obj_x6064{OD_Object(1, 4, sizeof(data.x6064), &data.x6064)}, obj_x60FD{OD_Object(1, 7, sizeof(data.x60FD), &data.x60FD)}{}
};

struct OD_ObjectEntry // Struct representing a dictionnary entry
{
    const uint16_t index;
    const uint16_t objectType;
    const uint8_t subNumber;
    const OD_Object *objects;

    OD_ObjectEntry(uint16_t index, uint16_t objectType, uint8_t subNumber, OD_Object *objects) : index(index), objectType(objectType), subNumber(subNumber), objects(objects) {}
};

class OD_ObjectDictionnary // Object dictionnary class that is usable for the user
{
private:
    const unsigned size;
    OD_Data data;
    OD_Objects objects;
    OD_ObjectEntry entries[OD_OBJECT_COUNT];

public:
    OD_ObjectDictionnary() : size(OD_OBJECT_COUNT), objects(data), entries{OD_ObjectEntry(2, 7, 1, objects.obj_x2), OD_ObjectEntry(3, 7, 1, objects.obj_x3), OD_ObjectEntry(4, 7, 1, objects.obj_x4), OD_ObjectEntry(5, 7, 1, objects.obj_x5), OD_ObjectEntry(6, 7, 1, objects.obj_x6), OD_ObjectEntry(7, 7, 1, objects.obj_x7), OD_ObjectEntry(4096, 7, 1, objects.obj_x1000), OD_ObjectEntry(4097, 7, 1, objects.obj_x1001), OD_ObjectEntry(4099, 8, 9, objects.obj_x1003), OD_ObjectEntry(4101, 7, 1, objects.obj_x1005), OD_ObjectEntry(4102, 7, 1, objects.obj_x1006), OD_ObjectEntry(4103, 7, 1, objects.obj_x1007), OD_ObjectEntry(4108, 7, 1, objects.obj_x100C), OD_ObjectEntry(4109, 7, 1, objects.obj_x100D), OD_ObjectEntry(4112, 8, 14, objects.obj_x1010), OD_ObjectEntry(4113, 8, 14, objects.obj_x1011), OD_ObjectEntry(4116, 7, 1, objects.obj_x1014), OD_ObjectEntry(4118, 8, 2, objects.obj_x1016), OD_ObjectEntry(4119, 7, 1, objects.obj_x1017), OD_ObjectEntry(4120, 9, 5, objects.obj_x1018), OD_ObjectEntry(4121, 7, 1, objects.obj_x1019), OD_ObjectEntry(4128, 8, 3, objects.obj_x1020), OD_ObjectEntry(4137, 8, 3, objects.obj_x1029), OD_ObjectEntry(5120, 9, 3, objects.obj_x1400), OD_ObjectEntry(5121, 9, 3, objects.obj_x1401), OD_ObjectEntry(5122, 9, 3, objects.obj_x1402), OD_ObjectEntry(5123, 9, 3, objects.obj_x1403), OD_ObjectEntry(5632, 9, 9, objects.obj_x1600), OD_ObjectEntry(5633, 9, 9, objects.obj_x1601), OD_ObjectEntry(5634, 9, 9, objects.obj_x1602), OD_ObjectEntry(5635, 9, 9, objects.obj_x1603), OD_ObjectEntry(6144, 9, 7, objects.obj_x1800), OD_ObjectEntry(6145, 9, 7, objects.obj_x1801), OD_ObjectEntry(6146, 9, 7, objects.obj_x1802), OD_ObjectEntry(6147, 9, 7, objects.obj_x1803), OD_ObjectEntry(6656, 9, 9, objects.obj_x1A00), OD_ObjectEntry(6657, 9, 9, objects.obj_x1A01), OD_ObjectEntry(6658, 9, 9, objects.obj_x1A02), OD_ObjectEntry(6659, 9, 9, objects.obj_x1A03), OD_ObjectEntry(8017, 8, 3, objects.obj_x1F51), OD_ObjectEntry(8023, 8, 3, objects.obj_x1F57), OD_ObjectEntry(8064, 7, 1, objects.obj_x1F80), OD_ObjectEntry(24583, 7, 1, objects.obj_x6007), OD_ObjectEntry(24639, 7, 1, objects.obj_x603F), OD_ObjectEntry(24640, 7, 1, objects.obj_x6040), OD_ObjectEntry(24641, 7, 1, objects.obj_x6041), OD_ObjectEntry(24642, 7, 1, objects.obj_x6042), OD_ObjectEntry(24643, 7, 1, objects.obj_x6043), OD_ObjectEntry(24644, 7, 1, objects.obj_x6044), OD_ObjectEntry(24646, 8, 3, objects.obj_x6046), OD_ObjectEntry(24648, 9, 3, objects.obj_x6048), OD_ObjectEntry(24649, 9, 3, objects.obj_x6049), OD_ObjectEntry(24650, 9, 3, objects.obj_x604A), OD_ObjectEntry(24673, 7, 1, objects.obj_x6061), OD_ObjectEntry(24676, 7, 1, objects.obj_x6064), OD_ObjectEntry(24829, 7, 1, objects.obj_x60FD)}{}
    inline void save() { return data.save(); }
    inline void load() { return data.load(); }
    inline OD_Objects &getObjects() { return objects; }
    OD_ObjectEntry *findEntry(uint16_t index);
};