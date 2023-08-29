#pragma once
#include <cstdint>
#include "enums.hpp"
#define HEARTBEAT_DLC 1
#define HEARTBEAT_STATE_OFFSET 0

namespace CANopen
{
    /**
     * @brief This class represents the Heartbeat object.
     * It automatically handles the emission of heartbeat messages.
     * See p. 76 of CIA301 for more details.
     */
    class HB
    {
    private:
        class Node &node;
        uint32_t lastPublish = 0;

        /**
         * @brief Internal method to publish the NMT state.
         * @param state NMT state to publish.
         */
        void publishState(NMTStates state);

        /**
         * @brief Update this object.
         * If producer heartbeat time (0x1017) is configured, the state will be published at said rate.
         * @param timestamp_us Current timestamp in microseconds.
         */
        void update(uint32_t timestamp_us);

    public:
        friend class NMT;
        friend class Node;
        /**
         * @brief Constructor for the HB class.
         * @param node The parent Node reference.
         */
        HB(class Node &node);
    };
}
