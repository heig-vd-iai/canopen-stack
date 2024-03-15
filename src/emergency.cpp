/**
 * Contains the definitions of the EMCY class.
 */
#include "emergency.hpp"

#include "frame.hpp"
#include "node.hpp"

using namespace CANopen;

ErrorRegister::ErrorRegister() : value(0) {}

void ErrorRegister::init() { odID = node.od().findObject(ERROR_REGISTER_INDEX); }

uint8_t ErrorRegister::getValue() { return value; }

void ErrorRegister::setErrorBit(unsigned bit) {
    ErrorRegisterValue reg = {value};
    reg.bits.genericError = 1;
    switch ((ErrorRegisterBits)bit) {
        case ErrorRegisterBit_Current:
            reg.bits.current = 1;
            break;
        case ErrorRegisterBit_Voltage:
            reg.bits.voltage = 1;
            break;
        case ErrorRegisterBit_Temperature:
            reg.bits.temperature = 1;
            break;
        case ErrorRegisterBit_Communication:
            reg.bits.communicationError = 1;
            break;
        case ErrorRegisterBit_DeviceProfile:
            reg.bits.deviceProfileSpecific = 1;
            break;
        case ErrorRegisterBit_Manufacturer:
            reg.bits.manufacturerSpecific = 1;
            break;
        default:
            break;
    }
    value = reg.value;
}

void ErrorRegister::clearErrorBit(unsigned bit) {
    ErrorRegisterValue reg = {value};
    switch ((ErrorRegisterBits)bit) {
        case ErrorRegisterBit_Generic:
            // Only clear generic error bit is all other bits are clear
            if (reg.value == 0b00000001) reg.bits.genericError = 0;
            break;
        case ErrorRegisterBit_Current:
            reg.bits.current = 0;
            break;
        case ErrorRegisterBit_Voltage:
            reg.bits.voltage = 0;
            break;
        case ErrorRegisterBit_Temperature:
            reg.bits.temperature = 0;
            break;
        case ErrorRegisterBit_Communication:
            reg.bits.communicationError = 0;
            break;
        case ErrorRegisterBit_DeviceProfile:
            reg.bits.deviceProfileSpecific = 0;
            break;
        case ErrorRegisterBit_Manufacturer:
            reg.bits.manufacturerSpecific = 0;
            break;
        default:
            break;
    }
    value = reg.value;
}

bool ErrorRegister::isErrorfree() { return value == 0; }

void ErrorRegister::reset() { value = 0; }

int8_t ErrorRegister::getData(Data &data, int32_t odID,
                              SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID != this->odID) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    data.u8 = value;
    return 0;
}

int8_t ErrorRegister::setData(const Data &data, int32_t odID,
                              SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID != this->odID) {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    value = data.u8;
    return 0;
}

PreDefinesErrorField::PreDefinesErrorField() : errorsNumber(0) {}

void PreDefinesErrorField::init() {
    odID = node.od().findObject(PREDEFINED_ERROR_FIELD_INDEX);
    for (int i = 0; i < PREDEFINED_ERROR_FIELD_SIZE; i++) errorsField[i] = 0;
}

void PreDefinesErrorField::shiftErrors() {
    uint32_t val = 0;
    for (unsigned i = errorsNumber; i > 0; i--) {
        val = errorsField[i - 1];
        errorsField[i] = val;
    }
}

void PreDefinesErrorField::pushError(uint16_t errorCode,
                                     uint32_t manufacturerCode) {
    uint32_t newError = manufacturerCode << 16 | errorCode;
    uint32_t firstError = newError;
    firstError = errorsField[0];
    if (firstError != newError) {
        shiftErrors();
        uint8_t count = errorsNumber;
        if (count < PREDEFINED_ERROR_FIELD_SIZE) errorsNumber = ++count;
        errorsField[0] = newError;
    }
}

void PreDefinesErrorField::clearErrors() {
    errorsNumber = 0;
    for (unsigned i = 0; i < errorsNumber; i++) errorsField[i] = 0;
}

int8_t PreDefinesErrorField::getData(Data &data, int32_t odID,
                                     SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID == this->odID) {
        data.u8 = errorsNumber;
        return 0;
    }
    if (odID >= this->odID + 1 && odID < this->odID + 1 + PREDEFINED_ERROR_FIELD_SIZE) {
        data.u32 = errorsField[odID - this->odID - 1];
        return 0;
    }
}

int8_t PreDefinesErrorField::setData(const Data &data, int32_t odID,
                                     SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID == this->odID) {
        if (data.u8 == 0) {
            clearErrors();
        } else {
            abortCode = SDOAbortCode_InvalidDownloadParameterValue;
            return -1;
        }
        abortCode = SDOAbortCode_OK;
        return 0;
    }
}

ErrorBehavior::ErrorBehavior() {}

void ErrorBehavior::init() {
    odID = node.od().findObject(ERROR_BEHAVIOR_INDEX);
    communicationError = {ErrorBehaviorValue_PreOperational};
    internalDeviceError = {ErrorBehaviorValue_PreOperational};
}

ErrorBehaviorValue ErrorBehavior::getCommunicationError() {
    return communicationError;
}

ErrorBehaviorValue ErrorBehavior::getInternalDeviceError() {
    return internalDeviceError;
}

int8_t ErrorBehavior::getData(Data &data, int32_t odID,
                              SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID == this->odID) {
        data.u8 = numberOfEntries;
    }
    if (odID == this->odID + 1) {
        data.u8 = (uint8_t)communicationError;
    } else if (odID == this->odID + 2) {
        data.u8 = (uint8_t)internalDeviceError;
    } else {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return 0;
}

int8_t ErrorBehavior::setData(const Data &data, int32_t odID,
                              SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_OK;
    if (odID == this->odID + 1) {
        communicationError = (ErrorBehaviorValue)data.u8;
    } else if (odID == this->odID + 2) {
        internalDeviceError = (ErrorBehaviorValue)data.u8;
    } else {
        abortCode = SDOAbortCode_ObjectNonExistent;
        return -1;
    }
    return 0;
}

EMCY::EMCY()
    : errorRegister(),
      preDefinedErrorField(),
      errorBehavior() {}

void EMCY::init() {
    errorRegister.init();
    preDefinedErrorField.init();
    errorBehavior.init();
}

void EMCY::enable() { enabled = true; }

void EMCY::disable() { enabled = false; }

void EMCY::sendError(uint16_t errorCode, uint32_t manufacturerCode) {
    EmergencyFrame frame(node.nodeId, errorCode, errorRegister.getValue(),
                         manufacturerCode);
    node.hardware().sendFrame(frame);
}

void EMCY::raiseError(uint16_t errorCode,
                      uint16_t manufacturerCode) {
    if (!enabled) return;
    switch ((EMCYErrorCodes)errorCode) {
        case EMCYErrorCode_Generic:
        case EMCYErrorCode_DeviceHardware:
        case EMCYErrorCode_Software:
        case EMCYErrorCode_Software_Internal:
        case EMCYErrorCode_Software_User:
        case EMCYErrorCode_Software_Dataset:
        case EMCYErrorCode_Modules:
        case EMCYErrorCode_Monitoring:
        case EMCYErrorCode_Protocol:
        case EMCYErrorCode_Protocol_PDOLengthError:
        case EMCYErrorCode_Protocol_PDOLengthExceeded:
        case EMCYErrorCode_Protocol_DAMMPDODestinationNotAvailable:
        case EMCYErrorCode_Protocol_SyncDataLength:
        case EMCYErrorCode_Protocol_RPDOTimeout:
        case EMCYErrorCode_ExternalError:
            errorRegister.setErrorBit(ErrorRegisterBit_Generic);
            break;
        case EMCYErrorCode_Current:
        case EMCYErrorCode_Current_InputSide:
        case EMCYErrorCode_Current_InsideDevice:
        case EMCYErrorCode_Current_OutputSide:
            errorRegister.setErrorBit(ErrorRegisterBit_Current);
            break;
        case EMCYErrorCode_Voltage:
        case EMCYErrorCode_Voltage_Main:
        case EMCYErrorCode_Voltage_InsideDevice:
        case EMCYErrorCode_Voltage_Output:
        case EMCYErrorCode_DC_Link_Under_Voltage:
            errorRegister.setErrorBit(ErrorRegisterBit_Voltage);
            break;
        case EMCYErrorCode_Temperature:
        case EMCYErrorCode_Temperature_Ambient:
        case EMCYErrorCode_Temperature_Device:
            errorRegister.setErrorBit(ErrorRegisterBit_Temperature);
            break;
        case EMCYErrorCode_Communication:
        case EMCYErrorCode_Communication_CANOverrun:
        case EMCYErrorCode_Communication_CANErrorPassive:
        case EMCYErrorCode_Communication_HeartbeatError:
        case EMCYErrorCode_Communication_RecoveredBusOFF:
        case EMCYErrorCode_Communication_CANIDCollision:
            errorRegister.setErrorBit(ErrorRegisterBit_Communication);
            break;
        case EMCYErrorCode_DeviceSpecific:
            errorRegister.setErrorBit(ErrorRegisterBit_DeviceProfile);
            break;
        case EMCYErrorCode_AdditionalFunctions:
            errorRegister.setErrorBit(ErrorRegisterBit_Manufacturer);
            break;
        default:
            break;
    }
    preDefinedErrorField.pushError(errorCode, manufacturerCode);
    sendError(errorCode, manufacturerCode);
    NMTServiceCommands command = NMTServiceCommand_None;
    switch (errorBehavior.getCommunicationError()) {
        default:
        case ErrorBehaviorValue_PreOperational:
            if (node._nmt.getState() == NMTState_Operational)
                command = NMTServiceCommand_EnterPreOperational;
            break;
        case ErrorBehaviorValue_None:
            command = NMTServiceCommand_None;
            break;
        case ErrorBehaviorValue_Stop:
            command = NMTServiceCommand_Stop;
            break;
    }
    node._nmt.setTransition(command);
}

void EMCY::clearErrorBit(unsigned bit) {
    bool tmp = errorRegister.isErrorfree();
    errorRegister.clearErrorBit(bit);
    if (errorRegister.isErrorfree() && !tmp) sendError(EMCYErrorCode_Reset, 0);
}

void EMCY::clearErrorBit(EMCYErrorCodes code) {
    ErrorRegisterBits bit;
    switch (code) {
        case EMCYErrorCode_Generic:
        case EMCYErrorCode_DeviceHardware:
        case EMCYErrorCode_Software:
        case EMCYErrorCode_Software_Internal:
        case EMCYErrorCode_Software_User:
        case EMCYErrorCode_Software_Dataset:
        case EMCYErrorCode_Modules:
        case EMCYErrorCode_Monitoring:
        case EMCYErrorCode_Protocol:
        case EMCYErrorCode_Protocol_PDOLengthError:
        case EMCYErrorCode_Protocol_PDOLengthExceeded:
        case EMCYErrorCode_Protocol_DAMMPDODestinationNotAvailable:
        case EMCYErrorCode_Protocol_SyncDataLength:
        case EMCYErrorCode_Protocol_RPDOTimeout:
        case EMCYErrorCode_ExternalError:
            bit = ErrorRegisterBit_Generic;
            break;
        case EMCYErrorCode_Current:
        case EMCYErrorCode_Current_InputSide:
        case EMCYErrorCode_Current_InsideDevice:
        case EMCYErrorCode_Current_OutputSide:
            bit = ErrorRegisterBit_Current;
            break;
        case EMCYErrorCode_Voltage:
        case EMCYErrorCode_Voltage_Main:
        case EMCYErrorCode_Voltage_InsideDevice:
        case EMCYErrorCode_Voltage_Output:
            bit = ErrorRegisterBit_Voltage;
            break;
        case EMCYErrorCode_Temperature:
        case EMCYErrorCode_Temperature_Ambient:
        case EMCYErrorCode_Temperature_Device:
            bit = ErrorRegisterBit_Temperature;
            break;
        case EMCYErrorCode_Communication:
        case EMCYErrorCode_Communication_CANOverrun:
        case EMCYErrorCode_Communication_CANErrorPassive:
        case EMCYErrorCode_Communication_HeartbeatError:
        case EMCYErrorCode_Communication_RecoveredBusOFF:
        case EMCYErrorCode_Communication_CANIDCollision:
            bit = ErrorRegisterBit_Communication;
            break;
        case EMCYErrorCode_DeviceSpecific:
            bit = ErrorRegisterBit_DeviceProfile;
            break;
        case EMCYErrorCode_AdditionalFunctions:
            bit = ErrorRegisterBit_Manufacturer;
            break;
        default:
            break;
    }
    clearErrorBit((unsigned)bit);
}

uint8_t EMCY::getErrorRegister() { return errorRegister.getValue(); }

void EMCY::clearHistory() { preDefinedErrorField.clearErrors(); }

void EMCY::reset() { errorRegister.reset(); }
