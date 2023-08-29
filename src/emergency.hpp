#pragma once
#include <cstdint>
#define X1029_SUB_COMMUNICATION 1
#define X1029_BEHAVIOUR_PREOP 0x00
#define X1029_BEHAVIOUR_NONE 0x01
#define X1029_BEHAVIOUR_STOPPED 0x02
#define EMCY_DLC 8
#define EMCY_ERRCODE_OFFSET 0
#define EMCY_ERRREG_OFFSET 2
#define EMCY_MANUFACTURER_OFFSET 3

namespace CANopen
{
    /**
     * @brief This class represents the Emergency object.
     * It handles the emission of emergency messages, as well as the pre-defined error field and error register.
     * See p. 64 of CIA301 for more details.
     */
    class EMCY
    {
    private:
        bool enabled = false;
        bool errorFree = true;
        class Node &node;
        class Object1001 *errorRegisterObject;

        /**
         * @brief Enable EMCY functionality, should be used by NMT.
         */
        void enable();

        /**
         * @brief Disable EMCY functionality, should be used by NMT.
         */
        void disable();

        /**
         * @brief Internal method to send an error message.
         * @param errorCode The error code to send.
         * @param manufacturerCode The manufacturer-specific error code.
         */
        void sendError(uint16_t errorCode, uint32_t manufacturerCode);

    public:
        friend class NMT;
        /**
         * @brief Constructor for EMCY class.
         * @param node The parent Node reference.
         */
        EMCY(class Node &node);

        /**
         * @brief Sends an emergency message on the bus, and automatically sets the corresponding bits in the error register.
         * The generic error bit is always set when using this method.
         * @param errorCode The error code to send.
         * @param manufacturerCode The optional manufacturer-specific error code.
         */
        void raiseError(uint16_t errorCode, uint16_t manufacturerCode = 0);

        /**
         * @brief Clear a specific error bit from the error register.
         * The generic error bit can only be cleared if all other bits are already cleared.
         * @param bit The index of the error bit to clear.
         */
        void clearErrorBit(unsigned bit);

        /**
         * @brief Get the current error register value.
         * @return The error register value.
         */
        uint8_t getErrorRegister();

        /**
         * @brief Clear the pre-defined error field (error history) in object 0x1003.
         */
        void clearHistory();

        /**
         * @brief Completely reset the error register to 0.
         */
        void reset();
    };
}
