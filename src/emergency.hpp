/**
 * Declaration of the EMCY class.
 */
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
     * This class represents the Emergency object.
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
         * Enable EMCY functionality, should be used by NMT only.
         */
        void enable();

        /**
         * Disable EMCY functionality, should be used by NMT only.
         */
        void disable();

        /**
         * Internal method to send an error message.
         * @param errorCode The error code to send.
         * @param manufacturerCode The manufacturer-specific error code.
         */
        void sendError(uint16_t errorCode, uint32_t manufacturerCode);

    public:
        friend class NMT;
        /**
         * Constructor for EMCY class.
         * @param node The parent Node reference.
         */
        EMCY(class Node &node);

        /**
         * Sends an emergency message on the bus, and automatically sets the corresponding bits in the error register.
         * The generic error bit is always set when using this method.
         * @param errorCode The error code to send.
         * @param manufacturerCode The optional manufacturer-specific error code.
         */
        void raiseError(uint16_t errorCode, uint16_t manufacturerCode = 0);

        /**
         * Clear a specific error bit from the error register.
         * The generic error bit can only be cleared if all other bits are already cleared.
         * @param bit The index of the error bit to clear.
         */
        void clearErrorBit(unsigned bit);

        /**
         * Get the current error register value.
         * @return The error register value.
         */
        uint8_t getErrorRegister();

        /**
         * Clear the pre-defined error field (error history) in object 0x1003.
         */
        void clearHistory();

        /**
         * Completely reset the error register to 0.
         */
        void reset();
    };
}
