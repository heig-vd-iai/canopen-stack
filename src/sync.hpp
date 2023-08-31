/******************************************************************************
 * [Filename]:      sync.hpp
 * [Project]:       CANopen
 * [Author]:        Tristan Lieberherr
 * [Date]:          August 2023
 * [Description]:   Contains the declaration of the SYNC class.
 *****************************************************************************/
#pragma once
#include <cstdint>
#define SYNC_COUNTER_OFFSET 0

namespace CANopen
{
    /**
     * @brief This class represents the SYNC object.
     * It automatically handles the reception of SYNC events and calls associated callback for PDO class.
     * See p. 62 of CIA301 for more details.
     */
    class SYNC
    {
    private:
        bool enabled = false;
        class Node &node;
        uint8_t internalCounter = 1;
        uint8_t maxCounter;

        /**
         * @brief Enable EMCY functionality, should be used by NMT only.
         */
        void enable();

        /**
         * @brief Disable EMCY functionality, should be used by NMT only.
         */
        void disable();

        /**
         * @brief Receive and process a SYNC frame.
         * This method receives and processes a SYNC frame, updating internal state and performing actions as needed.
         * @param frame SYNCFrame to be processed.
         * @param timestamp_us Timestamp in microseconds of the frame reception.
         */
        void receiveFrame(class SYNCFrame &frame, uint32_t timestamp_us);

    public:
        friend class NMT;
        friend class Node;

        /**
         * @brief Constructor for the SYNC class.
         * @param node The parent Node reference.
         */
        SYNC(class Node &node);
    };
}
