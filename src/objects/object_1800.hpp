#pragma once
#include "../object.hpp"
#define X1800_INDEX_COBID 1
#define X1800_INDEX_TRANSMISSION 2
#define X1800_INDEX_INHIBIT 3
#define X1800_INDEX_RESERVED 4
#define X1800_INDEX_EVENT 5
#define X1800_INDEX_SYNC 6
#define X1800_ACYCLIC 0x0
#define X1800_SYNC_MAX 0xF0
#define X1800_RTR_SYNC 0xFC
#define X1800_RTR_EVENT 0xFD
#define X1800_EVENT1 0xFE
#define X1800_EVENT2 0xFF
#define X1800_COBID_VALID_MASK 0x80000000

namespace CANopen
{
    /**
     * @brief This class represents the TPDO communication parameter (0x1800) object.
     */
    class Object1800 : public Object
    {
    private:
        bool remap = false;

        SDOAbortCodes preReadBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, uint32_t offset) override;
        SDOAbortCodes preWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;
        void postWriteBytes(uint8_t subindex, uint8_t *bytes, uint32_t size, class Node &node) override;

    public:
        /**
         * @brief Constructor for the Object class.
         * @param index Index of the object.
         * @param subNumber Number of subentries in the object.
         * @param entries Array of pointers to object entries belonging to that object.
         */
        Object1800(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

        /**
         * @brief Get the 32 bit COB-ID entry (subindex 1).
         * This value can be casted as a PDOCobidEntry.
         * @return The COB-ID value for the TPDO.
         */
        uint32_t getCobId();

        /**
         * @brief Get the actual 11 bit COB-ID value.
         * @return The actual COB-ID value for the TPDO.
         */
        uint16_t getActualCobId();

        /**
         * @brief Get the transmission type entry (subindex 2).
         * @return The transmission type value for the TPDO.
         */
        uint8_t getTransmissionType();

        /**
         * @brief Get the inhibit time entry (subindex 3) in microseconds.
         * @return The inhibit time value for the TPDO in microseconds.
         */
        uint32_t getInhibitTime_us();

        /**
         * @brief Get the event timer entry (subindex 5) in miliseconds.
         * @return The event timer value for the TPDO in miliseconds.
         */
        uint16_t getEventTimer_ms();

        /**
         * @brief Get the event timer entry (subindex 5) in microseconds.
         * @return The event timer value for the TPDO in microseconds.
         */
        uint32_t getEventTimer_us();

        /**
         * @brief Get the %SYNC start value.
         * @return The %SYNC start value for the TPDO.
         */
        uint8_t getSyncStart();

        /**
         * @brief Check if the TPDO is enabled (bit 31 of subindex 1).
         * @return True if the TPDO is enabled, false otherwise.
         */
        bool isEnabled();

        /**
         * @brief Check if the inhibit time is present.
         * @return True if the inhibit time is present, false otherwise.
         */
        bool isInhibitSupported();

        /**
         * @brief Check if the event timer is present.
         * @return True if the event timer is present, false otherwise.
         */
        bool isTimerSupported();

        /**
         * @brief Check if the transmission type is synchronous.
         * @return True if synchronous, false otherwise.
         */
        bool isSynchronous();
    };
}
