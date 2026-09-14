/**
 * This file contains the declaration of the ObjectDictionnary class.
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once

#include "od_common.hpp"
#include "od/metadata.hpp"

#define OD_NODE_ID 1

#define OD_OBJECTS_COUNT 22
#define OD_TPDO_COUNT 1
#define OD_RPDO_COUNT 1
#define OD_LENGTH 73

/* Number of entries per typed data table, 0 when the table does not exist */
#define OD_BOOL_COUNT 1
#define OD_I8_COUNT 1
#define OD_I16_COUNT 2
#define OD_I32_COUNT 2
#define OD_I64_COUNT 1
#define OD_U8_COUNT 15
#define OD_U16_COUNT 7
#define OD_U32_COUNT 38
#define OD_U64_COUNT 1
#define OD_F32_COUNT 2
#define OD_F64_COUNT 1
#define OD_STR_COUNT 1

/* These defines map an object to its dictionnary array index */
#define OD_OBJECT_1000_SUB0 0
#define OD_OBJECT_1001_SUB0 1
#define OD_OBJECT_1010_SUB0 2
#define OD_OBJECT_1010_SUB1 3
#define OD_OBJECT_1010_SUB2 4
#define OD_OBJECT_1010_SUB3 5
#define OD_OBJECT_1010_SUB4 6
#define OD_OBJECT_1010_SUB5 7
#define OD_OBJECT_1011_SUB0 8
#define OD_OBJECT_1011_SUB1 9
#define OD_OBJECT_1011_SUB2 10
#define OD_OBJECT_1011_SUB3 11
#define OD_OBJECT_1011_SUB4 12
#define OD_OBJECT_1011_SUB5 13
#define OD_OBJECT_1017_SUB0 14
#define OD_OBJECT_1018_SUB0 15
#define OD_OBJECT_1018_SUB1 16
#define OD_OBJECT_1018_SUB2 17
#define OD_OBJECT_1018_SUB3 18
#define OD_OBJECT_1018_SUB4 19
#define OD_OBJECT_1400_SUB0 20
#define OD_OBJECT_1400_SUB1 21
#define OD_OBJECT_1400_SUB2 22
#define OD_OBJECT_1600_SUB0 23
#define OD_OBJECT_1600_SUB1 24
#define OD_OBJECT_1600_SUB2 25
#define OD_OBJECT_1600_SUB3 26
#define OD_OBJECT_1600_SUB4 27
#define OD_OBJECT_1600_SUB5 28
#define OD_OBJECT_1600_SUB6 29
#define OD_OBJECT_1600_SUB7 30
#define OD_OBJECT_1600_SUB8 31
#define OD_OBJECT_1800_SUB0 32
#define OD_OBJECT_1800_SUB1 33
#define OD_OBJECT_1800_SUB2 34
#define OD_OBJECT_1800_SUB3 35
#define OD_OBJECT_1800_SUB4 36
#define OD_OBJECT_1800_SUB5 37
#define OD_OBJECT_1800_SUB6 38
#define OD_OBJECT_1a00_SUB0 39
#define OD_OBJECT_1a00_SUB1 40
#define OD_OBJECT_1a00_SUB2 41
#define OD_OBJECT_1a00_SUB3 42
#define OD_OBJECT_1a00_SUB4 43
#define OD_OBJECT_1a00_SUB5 44
#define OD_OBJECT_1a00_SUB6 45
#define OD_OBJECT_1a00_SUB7 46
#define OD_OBJECT_1a00_SUB8 47
#define OD_OBJECT_2000_SUB0 48
#define OD_OBJECT_2000_SUB1 49
#define OD_OBJECT_2000_SUB2 50
#define OD_OBJECT_2000_SUB3 51
#define OD_OBJECT_2000_SUB4 52
#define OD_OBJECT_2000_SUB5 53
#define OD_OBJECT_2000_SUB6 54
#define OD_OBJECT_2000_SUB7 55
#define OD_OBJECT_2000_SUB8 56
#define OD_OBJECT_2000_SUB9 57
#define OD_OBJECT_2000_SUB10 58
#define OD_OBJECT_2001_SUB0 59
#define OD_OBJECT_2002_SUB0 60
#define OD_OBJECT_2003_SUB0 61
#define OD_OBJECT_2004_SUB0 62
#define OD_OBJECT_2004_SUB1 63
#define OD_OBJECT_2004_SUB2 64
#define OD_OBJECT_2004_SUB3 65
#define OD_OBJECT_2005_SUB0 66
#define OD_OBJECT_2006_SUB0 67
#define OD_OBJECT_2007_SUB0 68
#define OD_OBJECT_6040_SUB0 69
#define OD_OBJECT_6041_SUB0 70
#define OD_OBJECT_6060_SUB0 71
#define OD_OBJECT_6064_SUB0 72

extern const uint64_t od_signature;

namespace CANopenOD {
// INDEX
// This table is used to find an object by its index.
constexpr std::pair<uint16_t, uint8_t> objectIndexTable[OD_LENGTH] = {
    /* 0000 */ std::make_pair(0x1000, 0),
    /* 0001 */ std::make_pair(0x1001, 0),
    /* 0002 */ std::make_pair(0x1010, 0),
    /* 0003 */ std::make_pair(0x1010, 1),
    /* 0004 */ std::make_pair(0x1010, 2),
    /* 0005 */ std::make_pair(0x1010, 3),
    /* 0006 */ std::make_pair(0x1010, 4),
    /* 0007 */ std::make_pair(0x1010, 5),
    /* 0008 */ std::make_pair(0x1011, 0),
    /* 0009 */ std::make_pair(0x1011, 1),
    /* 0010 */ std::make_pair(0x1011, 2),
    /* 0011 */ std::make_pair(0x1011, 3),
    /* 0012 */ std::make_pair(0x1011, 4),
    /* 0013 */ std::make_pair(0x1011, 5),
    /* 0014 */ std::make_pair(0x1017, 0),
    /* 0015 */ std::make_pair(0x1018, 0),
    /* 0016 */ std::make_pair(0x1018, 1),
    /* 0017 */ std::make_pair(0x1018, 2),
    /* 0018 */ std::make_pair(0x1018, 3),
    /* 0019 */ std::make_pair(0x1018, 4),
    /* 0020 */ std::make_pair(0x1400, 0),
    /* 0021 */ std::make_pair(0x1400, 1),
    /* 0022 */ std::make_pair(0x1400, 2),
    /* 0023 */ std::make_pair(0x1600, 0),
    /* 0024 */ std::make_pair(0x1600, 1),
    /* 0025 */ std::make_pair(0x1600, 2),
    /* 0026 */ std::make_pair(0x1600, 3),
    /* 0027 */ std::make_pair(0x1600, 4),
    /* 0028 */ std::make_pair(0x1600, 5),
    /* 0029 */ std::make_pair(0x1600, 6),
    /* 0030 */ std::make_pair(0x1600, 7),
    /* 0031 */ std::make_pair(0x1600, 8),
    /* 0032 */ std::make_pair(0x1800, 0),
    /* 0033 */ std::make_pair(0x1800, 1),
    /* 0034 */ std::make_pair(0x1800, 2),
    /* 0035 */ std::make_pair(0x1800, 3),
    /* 0036 */ std::make_pair(0x1800, 4),
    /* 0037 */ std::make_pair(0x1800, 5),
    /* 0038 */ std::make_pair(0x1800, 6),
    /* 0039 */ std::make_pair(0x1a00, 0),
    /* 0040 */ std::make_pair(0x1a00, 1),
    /* 0041 */ std::make_pair(0x1a00, 2),
    /* 0042 */ std::make_pair(0x1a00, 3),
    /* 0043 */ std::make_pair(0x1a00, 4),
    /* 0044 */ std::make_pair(0x1a00, 5),
    /* 0045 */ std::make_pair(0x1a00, 6),
    /* 0046 */ std::make_pair(0x1a00, 7),
    /* 0047 */ std::make_pair(0x1a00, 8),
    /* 0048 */ std::make_pair(0x2000, 0),
    /* 0049 */ std::make_pair(0x2000, 1),
    /* 0050 */ std::make_pair(0x2000, 2),
    /* 0051 */ std::make_pair(0x2000, 3),
    /* 0052 */ std::make_pair(0x2000, 4),
    /* 0053 */ std::make_pair(0x2000, 5),
    /* 0054 */ std::make_pair(0x2000, 6),
    /* 0055 */ std::make_pair(0x2000, 7),
    /* 0056 */ std::make_pair(0x2000, 8),
    /* 0057 */ std::make_pair(0x2000, 9),
    /* 0058 */ std::make_pair(0x2000, 10),
    /* 0059 */ std::make_pair(0x2001, 0),
    /* 0060 */ std::make_pair(0x2002, 0),
    /* 0061 */ std::make_pair(0x2003, 0),
    /* 0062 */ std::make_pair(0x2004, 0),
    /* 0063 */ std::make_pair(0x2004, 1),
    /* 0064 */ std::make_pair(0x2004, 2),
    /* 0065 */ std::make_pair(0x2004, 3),
    /* 0066 */ std::make_pair(0x2005, 0),
    /* 0067 */ std::make_pair(0x2006, 0),
    /* 0068 */ std::make_pair(0x2007, 0),
    /* 0069 */ std::make_pair(0x6040, 0),
    /* 0070 */ std::make_pair(0x6041, 0),
    /* 0071 */ std::make_pair(0x6060, 0),
    /* 0072 */ std::make_pair(0x6064, 0),
};

}

class ObjectDictionnary {

public:
    const uint32_t length = OD_LENGTH;
    static const Metadata objectMetadataTable[OD_LENGTH];
    static int8_t (*objectGetterTable[OD_LENGTH])(Data &data, int32_t id, SDOAbortCodes &abortCode);
    static int8_t (*objectSetterTable[OD_LENGTH])(const Data &data, int32_t id, SDOAbortCodes &abortCode);
    static uint8_t dataIndexTable[OD_LENGTH];

    // Default values initialisation tables
    static bool boolTable[1];
    static int8_t i8Table[1];
    static int16_t i16Table[2];
    static int32_t i32Table[2];
    static int64_t i64Table[1];
    static uint8_t u8Table[15];
    static uint16_t u16Table[7];
    static uint32_t u32Table[38];
    static uint64_t u64Table[1];
    static float f32Table[2];
    static double f64Table[1];

    static char object2001sub0[16];
    static char *stringTable[1];

    /**
     * Find an object in the dictionary.
     * @param index The hex index of the object to find.
     * @return index in objectDataTable if found, -1 otherwise.
     */
    int32_t findObject(uint16_t index, uint8_t subIndex = 0);

    /**
     * Access an object in the dictionary by array index using the subscript operator.
     * Under the hood, this method uses the at() method.
     * @param index The array index of the object to access.
     * @return index in objectDataTable if found, -1 otherwise.
     */
    int32_t operator[](uint16_t index);

    /**
     * Save data to non-volatile storage for a specified parameter group.
     * This method is called by object 0x1010.
     * @param parameterGroup The parameter group number.
     * @return True if saving was successful, false otherwise.
     */
    bool saveData(uint8_t parameterGroup);

    /**
    * Setter called by the od to save data
    * @param data must contain "save"
    * @param id object id for parameter group
    * @param abortCode to set if the object is not found.
    */
    int8_t saveData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Getter called by the od to send information about the save
     * @param data to fill with the save status
     * @param id object id for parameter group
     * @param abortCode to set if the object is not found.
    */
    int8_t getSave(Data &data, int32_t id, SDOAbortCodes &abortCode);

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

    /**
    * Setter called by the od to restore data
    * @param data must contain "load"
    * @param id object id for parameter group
    * @param abortCode to set if the object is not found.
    */
    int8_t restoreData(const Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Getter called by the od to send information about the restore
     * @param data to fill with the save status
     * @param id object id for parameter group
     * @param abortCode to set if the object is not found.
    */
    int8_t getRestore(Data &data, int32_t id, SDOAbortCodes &abortCode);

    /**
     * Test if an object exists in the dictionary.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @return True if the object exists, false otherwise.
    */
    bool isSubValid(uint16_t index, uint8_t subIndex);

    /**
     * Get the value of an object in the dictionary.
     * @param data The data object to store the value in.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @param abortCode The abort code to set if the object is not found.
     * @return 0 if the object was found, 1 if a request is pending, -1 otherwise.
    */
    int8_t readData(Data &data, uint16_t index, uint8_t subIndex,
                    SDOAbortCodes &abortCode);
    int8_t readData(Data &data, int32_t id, SDOAbortCodes &abortCode);
    int8_t readData(Data &data, uint16_t index, uint8_t subIndex);
    int8_t readData(Data &data, int32_t id);

    /**
     * Set the value of an object in the dictionary.
     * @param data The data object to store the value in.
     * @param index The hex index of the object to find.
     * @param subIndex The subindex of the object to find.
     * @param abortCode The abort code to set if the object is not found.
     * @return 0 if the object was found, 1 if a request is pending, -1
     * otherwise.
     */
    int8_t writeData(const Data &data, uint16_t index, uint8_t subIndex,
                     SDOAbortCodes &abortCode);
    int8_t writeData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
    int8_t writeData(const Data &data, uint16_t index, uint8_t subIndex);
    int8_t writeData(const Data &data, int32_t id);

    /**
     * Get the metadata of an object in the dictionary.
     * @param index The index to the object
     * @param subIndex The subindex to the object
     * @return The metadata of the object.
     */
    const Metadata *getMetadata(uint16_t index, uint8_t subindex);
    const Metadata *getMetadata(int32_t id);
    uint16_t getSize(uint16_t index, uint8_t subindex);
    uint16_t getSize(int32_t id);
};
