/******************************************************************************
 * [Filename]:      object_1400.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the declaration of the "RPDO communication parameter" Object1400 class.
 *****************************************************************************/
#pragma once
#include "object_1800.hpp"
#define X1400_INDEX_COBID X1800_INDEX_COBID
#define X1400_INDEX_TRANSMISSION X1800_INDEX_TRANSMISSION
#define X1400_INDEX_INHIBIT X1800_INDEX_INHIBIT
#define X1400_INDEX_RESERVED X1800_INDEX_RESERVED
#define X1400_INDEX_EVENT X1800_INDEX_EVENT
#define X1400_INDEX_SYNC X1800_INDEX_SYNC
#define X1400_SYNC_MAX X1800_SYNC_MAX
#define X1400_EVENT1 X1800_EVENT1
#define X1400_EVENT2 X1800_EVENT2
#define X1400_COBID_VALID_MASK X1800_COBID_VALID_MASK

namespace CANopen
{
    /**
     * @brief This class represents the RPDO communication parameter (0x1400) object.
     */
    class Object1400 : public Object
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
        Object1400(uint16_t index, uint8_t subNumber, const ObjectEntryBase *entries[]) : Object(index, subNumber, entries) {}

        /**
         * @brief Get the 32 bit COB-ID entry (subindex 1).
         * This value can be casted as a PDOCobidEntry.
         * @return The COB-ID value for the RPDO.
         */
        uint32_t getCobId();

        /**
         * @brief Get the actual 11 bit COB-ID value.
         * @return The actual COB-ID value for the RPDO.
         */
        uint16_t getActualCobId();

        /**
         * @brief Get the transmission type entry (subindex 2).
         * @return The transmission type value for the RPDO.
         */
        uint8_t getTransmissionType();

        /**
         * @brief Get the inhibit time entry (subindex 3) in microseconds.
         * @return The inhibit time value for the RPDO in microseconds.
         */
        uint32_t getInhibitTime_us();

        /**
         * @brief Get the event timer entry (subindex 5) in miliseconds.
         * @return The event timer value for the RPDO in miliseconds.
         */
        uint16_t getEventTimer_ms();

        /**
         * @brief Get the event timer entry (subindex 5) in microseconds.
         * @return The event timer value for the RPDO in microseconds.
         */
        uint32_t getEventTimer_us();

        /**
         * @brief Check if the RPDO is enabled (bit 31 of subindex 1).
         * @return True if the RPDO is enabled, false otherwise.
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
