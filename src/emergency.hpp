/**
 * Declaration of the EMCY class.
 */
#pragma once

#include <cstdint>

#include "enums.hpp"
#include "unions.hpp"

#define EMCY_DLC 8
#define EMCY_ERRCODE_OFFSET 0
#define EMCY_ERRREG_OFFSET 2
#define EMCY_MANUFACTURER_OFFSET 3
#define ERROR_REGISTER_INDEX 0x1001
#define PREDEFINED_ERROR_FIELD_SIZE 8
#define PREDEFINED_ERROR_FIELD_INDEX 0x1003
#define ERROR_BEHAVIOR_SIZE 2
#define ERROR_BEHAVIOR_INDEX 0x1029

namespace CANopen {
/**
 * Emergency object.
 * It handles the emission of emergency messages, as well as the pre-defined
 * error field and error register. See CiA301:2011§7.2.7 (p. 64)
 */

class ErrorRegister {
    int32_t odID;
    uint8_t value;

   public:
    ErrorRegister();
    void init();
    uint8_t getValue();
    void setErrorBit(unsigned bit);
    void clearErrorBit(unsigned bit);
    bool isErrorfree();
    void reset();
    int8_t getData(Data &data, int32_t id, SDOAbortCodes &abortCode);
    int8_t setData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
};

class PreDefinesErrorField {
    int32_t odID;
    uint8_t errorsNumber;
    uint32_t errorsField[PREDEFINED_ERROR_FIELD_SIZE];

   public:
    void init();
    PreDefinesErrorField();
    void shiftErrors();
    void pushError(uint16_t errorCode, uint32_t manufacturerCode);
    void clearErrors();
    int8_t getData(Data &data, int32_t id, SDOAbortCodes &abortCode);
    int8_t setData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
};

class ErrorBehavior {
    int32_t odID;
    uint8_t numberOfEntries;
    ErrorBehaviorValue communicationError;
    ErrorBehaviorValue internalDeviceError;

   public:
    void init();
    ErrorBehavior();
    ErrorBehaviorValue getCommunicationError();
    ErrorBehaviorValue getInternalDeviceError();
    int8_t getData(Data &data, int32_t id, SDOAbortCodes &abortCode);
    int8_t setData(const Data &data, int32_t id, SDOAbortCodes &abortCode);
};

class EMCY {
   public:
    ErrorRegister errorRegister;
    PreDefinesErrorField preDefinedErrorField;
    ErrorBehavior errorBehavior;

   private:
    bool enabled = false;
    bool errorFree = true;
    // class Object1001 *errorRegisterObject;

    /**
     * Enable EMCY functionality, should only be called internally by NMT class.
     */
    void enable();

    /**
     * Disable EMCY functionality, should only be called internally by NMT
     * class.
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

    void init();

    /**
     * Constructor for EMCY class.
     * @param node The parent Node reference.
     */
    EMCY();

    /**
     * Sends an emergency message on the bus, and automatically sets the
     * corresponding bits in the error register. The generic error bit is always
     * set when using this method.
     * @param errorCode The error code to send.
     * @param manufacturerCode The optional manufacturer-specific error code.
     */
    void raiseError(uint16_t errorCode, uint16_t manufacturerCode = 0);

    /**
     * Clear a specific error bit from the error register.
     * The generic error bit can only be cleared if all other bits are already
     * cleared.
     * @param bit The index of the error bit to clear.
     */
    void clearErrorBit(unsigned bit);
    void clearErrorBit(EMCYErrorCodes code);

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
}  // namespace CANopen
