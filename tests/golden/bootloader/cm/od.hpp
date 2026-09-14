/**
 * This file contains the declaration of the ObjectDictionnary class.
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once

#include "od_common.hpp"
#include "od/metadata.hpp"

#define OD_NODE_ID 1

#define OD_OBJECTS_COUNT 8
#define OD_TPDO_COUNT 0
#define OD_RPDO_COUNT 0
#define OD_LENGTH 16

/* Number of entries per typed data table, 0 when the table does not exist */
#define OD_BOOL_COUNT 0
#define OD_I8_COUNT 0
#define OD_I16_COUNT 0
#define OD_I32_COUNT 0
#define OD_I64_COUNT 0
#define OD_U8_COUNT 7
#define OD_U16_COUNT 1
#define OD_U32_COUNT 7
#define OD_U64_COUNT 0
#define OD_F32_COUNT 0
#define OD_F64_COUNT 0
#define OD_STR_COUNT 0

/* These defines map an object to its dictionnary array index */
#define OD_OBJECT_1000_SUB0 0
#define OD_OBJECT_1001_SUB0 1
#define OD_OBJECT_1017_SUB0 2
#define OD_OBJECT_1018_SUB0 3
#define OD_OBJECT_1018_SUB1 4
#define OD_OBJECT_1018_SUB2 5
#define OD_OBJECT_1018_SUB3 6
#define OD_OBJECT_1018_SUB4 7
#define OD_OBJECT_1f50_SUB0 8
#define OD_OBJECT_1f50_SUB1 9
#define OD_OBJECT_1f51_SUB0 10
#define OD_OBJECT_1f51_SUB1 11
#define OD_OBJECT_1f56_SUB0 12
#define OD_OBJECT_1f56_SUB1 13
#define OD_OBJECT_1f57_SUB0 14
#define OD_OBJECT_1f57_SUB1 15

extern const uint64_t od_signature;

namespace CANopenOD {
// INDEX
// This table is used to find an object by its index.
constexpr std::pair<uint16_t, uint8_t> objectIndexTable[OD_LENGTH] = {
    /* 0000 */ std::make_pair(0x1000, 0),
    /* 0001 */ std::make_pair(0x1001, 0),
    /* 0002 */ std::make_pair(0x1017, 0),
    /* 0003 */ std::make_pair(0x1018, 0),
    /* 0004 */ std::make_pair(0x1018, 1),
    /* 0005 */ std::make_pair(0x1018, 2),
    /* 0006 */ std::make_pair(0x1018, 3),
    /* 0007 */ std::make_pair(0x1018, 4),
    /* 0008 */ std::make_pair(0x1f50, 0),
    /* 0009 */ std::make_pair(0x1f50, 1),
    /* 0010 */ std::make_pair(0x1f51, 0),
    /* 0011 */ std::make_pair(0x1f51, 1),
    /* 0012 */ std::make_pair(0x1f56, 0),
    /* 0013 */ std::make_pair(0x1f56, 1),
    /* 0014 */ std::make_pair(0x1f57, 0),
    /* 0015 */ std::make_pair(0x1f57, 1),
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
    static uint8_t u8Table[7];
    static uint16_t u16Table[1];
    static uint32_t u32Table[7];


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
