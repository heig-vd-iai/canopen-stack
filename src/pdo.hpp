/**
 * Contains the declaration of the PDO class.
 */
#pragma once
#include <cstdint>
#include <functional>

#include "od.hpp"
#define TPDO_COMMUNICATION_INDEX 0x1800
#define TPDO_MAPPING_INDEX 0x1A00
#define RPDO_COMMUNICATION_INDEX 0x1400
#define RPDO_MAPPING_INDEX 0x1600
#define PDO_DLC 8

namespace CANopen {

class MapParameter {
   private:
    IObjectDictionnary &od;
    Object odID;

   protected:
    uint8_t entriesNumber;
    uint32_t mappedObjects[OD_PDO_MAPPING_MAX];
    uint8_t getData(Data &data, uint16_t id, SDOAbortCodes &abortCode);
    uint8_t setData(Data data, uint16_t id, SDOAbortCodes &abortCode);

   public:
    friend class PDO;
    friend class IObjectDictionary;
    MapParameter(IObjectDictionnary &od, uint16_t index);
    uint8_t getCount();
    uint32_t getMappedValue(uint8_t entry);
}

/**
 * PDO object.
 * It handles the emission and reception of TPDOs and RPDOs, as well as their
 * mapping and communication parameters. See CiA301:2011§7.2.2 (p. 31)
 */
class PDO {
    /**
     * Structure to hold a pair of an object and subindex for PDO mapping.
     */
    struct PDOPair {
        class Object *object;
        uint8_t subindex;
    };

    /**
     * Structure to represent a Transmit PDO.
     */
    struct TPDO {
        class Object1800 *commObject;
        class Object1A00 *mapObject;
        PDOPair mappedEntries[OD_PDO_MAPPING_MAX];
        uint8_t count = 0;
        uint8_t size = 0;
        uint32_t timestamp_us = 0;
        bool syncFlag = false;
    };

    /**
     * Structure to represent a Receive PDO.
     */
    struct RPDO {
        class Object1400 *commObject;
        class Object1600 *mapObject;
        PDOPair mappedEntries[OD_PDO_MAPPING_MAX];
        uint8_t buffer[PDO_DLC] = {0};
        uint8_t count = 0;
        uint8_t size = 0;
        uint32_t timestamp_us = 0;
        bool syncFlag = false;
        bool watchTimeoutFlag = false;
    };

   private:
    bool enabled = false;
    class Node &node;
    TPDO tpdos[OD_TPDO_COUNT];
    RPDO rpdos[OD_RPDO_COUNT];
    std::function<void(unsigned)> onReceiveFunc;
    std::function<void(unsigned)> onTimeoutFunc;

    /**
     * Enable PDO functionality, should only be called internally by NMT class.
     */
    void enable();

    /**
     * Disable PDO functionality, should only be called internally by NMT class.
     */
    void disable();

    /**
     * Initialize the Transmit PDO at the specified index.
     * @param index Index of the TPDO to initialize.
     */
    void initTPDO(unsigned index);

    /**
     * Initialize the Receive PDO at the specified index.
     * @param index Index of the RPDO to initialize.
     */
    void initRPDO(unsigned index);

    /**
     * Remap the Transmit PDO at the specified index.
     * The object-subindex pair array will be rebuilt from the current PDO
     * mapping parameter.
     * @param index Index of the TPDO to remap.
     */
    void remapTPDO(unsigned index);

    /**
     * Remap the Receive PDO at the specified index.
     * The object-subindex pair array will be rebuilt from the current PDO
     * mapping parameter.
     * @param index Index of the RPDO to remap.
     */
    void remapRPDO(unsigned index);

    /**
     * Copy the data from mapped objects of the Transmit PDO into the buffer.
     * @param index Index of the TPDO.
     * @param buffer Pointer to the buffer to store TPDO data.
     */
    void bufferizeTPDO(unsigned index, uint8_t *buffer);

    /**
     * Copy the data from the Receive PDO buffer into each mapped object.
     * @param index Index of the RPDO.
     * @param buffer Pointer to the buffer containing RPDO data.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void unpackRPDO(unsigned index, uint8_t *buffer, uint32_t timestamp_us);

    /**
     * Internal method to send the Transmit PDO at the specified index.
     * @param index Index of the TPDO to send.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void sendTPDO(unsigned index, uint32_t timestamp_us);

    /**
     * Receive and process the Transmit PDO frame.
     * This method will only be called for an RTR frame.
     * @param frame Frame to be processed.
     * @param timestamp_us Timestamp in microseconds of the frame reception.
     */
    void receiveTPDO(class Frame &frame, uint32_t timestamp_us);

    /**
     * Receive and process the Receive PDO frame.
     * @param frame Frame to be processed.
     * @param timestamp_us Timestamp in microseconds of the frame reception.
     */
    void receiveRPDO(class Frame &frame, uint32_t timestamp_us);

    /**
     * Update this object.
     * Eligible TPDOs will be sent and RPDOs deadline timeouts will be updated.
     * @param timestamp_us Current timestamp in microseconds.
     */
    void update(uint32_t timestamp_us);

    /**
     * Callback for SYNC event, called only from SYNC class.
     * @param counter Sync counter.
     * @param timestamp_us Timestamp of the SYNC event.
     */
    void onSync(uint8_t counter, uint32_t timestamp_us);

    /**
     * Get the synchronous window length from object 0x1007.
     * @return Synchronous window length in microseconds.
     */
    uint32_t getSyncWindow_us();

   public:
    friend class SYNC;
    friend class NMT;
    friend class Node;

    /**
     * Constructor for the PDO class.
     * @param node The parent Node reference.
     */
    PDO(class Node &node);

    /**
     * Transmit the specified Transmit PDO.
     * Index start at 0 for TPDO1
     * @param index Index of the TPDO to transmit.
     */
    void transmitTPDO(unsigned index);

    /**
     * Remap all Transmit PDOs.
     */
    void reloadTPDO();

    /**
     * Remap all Receive PDOs.
     */
    void reloadRPDO();

    /**
     * Set a callback function to be called when an RPDO is received.
     * The function will receive the RPDO index as an argument.
     * Index start at 0 for RPDO1.
     * **DO NOT use time consuming calls in the provided callback.**
     * @param callback Callback function to be called on RPDO reception.
     */
    void onReceive(std::function<void(unsigned)> callback);

    /**
     * Set a callback function to be called when an RPDO timeout occurs.
     * The function will receive the RPDO index as an argument.
     * Index start at 0 for RPDO1.
     * **DO NOT use time consuming calls in the provided callback.**
     * @param callback Callback function to be called on RPDO timeout.
     */
    void onTimeout(std::function<void(unsigned)> callback);
};
}  // namespace CANopen
