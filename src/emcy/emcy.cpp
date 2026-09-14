#include "emcy/emcy.hpp"

#include "emcy/frames.hpp"
#include "od_common.hpp"

using namespace CANopen;

namespace {

EMCY *boundEmcy = nullptr;

int8_t missingEmcy(SDOAbortCodes &abortCode) {
    abortCode = SDOAbortCode_ObjectNonExistent;
    return -1;
}

int8_t fail(SDOAbortCodes &abortCode, SDOAbortCodes code) {
    abortCode = code;
    return -1;
}

struct CodeClass {
    uint16_t first;
    uint16_t last;
    ErrorRegisterBits bit;
};

/** Error register bit set by each range of error codes. CiA301:2011§7.2.7.1 */
constexpr CodeClass CODE_CLASSES[] = {
    {0x2000, 0x2FFF, ErrorRegisterBit_Current},
    {0x3000, 0x3FFF, ErrorRegisterBit_Voltage},
    {0x4000, 0x4FFF, ErrorRegisterBit_Temperature},
    {emcy::COMMUNICATION_CODES_FIRST, emcy::COMMUNICATION_CODES_LAST,
     ErrorRegisterBit_Communication},
    {0xF000, 0xFEFF, ErrorRegisterBit_Manufacturer},
    {0xFF00, 0xFFFF, ErrorRegisterBit_DeviceProfile},
};

ErrorRegisterBits bitOf(uint16_t errorCode) {
    for (const CodeClass &codeClass : CODE_CLASSES) {
        if (errorCode >= codeClass.first && errorCode <= codeClass.last)
            return codeClass.bit;
    }
    return ErrorRegisterBit_Generic;
}

bool isCommunicationError(uint16_t errorCode) {
    return errorCode >= emcy::COMMUNICATION_CODES_FIRST &&
           errorCode <= emcy::COMMUNICATION_CODES_LAST;
}

}  // namespace

void CANopen::bindEmergency(EMCY &emcy) { boundEmcy = &emcy; }

int8_t emcyGetErrorRegister(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundEmcy == nullptr) return missingEmcy(abortCode);
    return boundEmcy->readRegister(data, id, abortCode);
}

int8_t emcyGetErrorField(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundEmcy == nullptr) return missingEmcy(abortCode);
    return boundEmcy->readHistory(data, id, abortCode);
}

int8_t emcySetErrorField(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode) {
    if (boundEmcy == nullptr) return missingEmcy(abortCode);
    return boundEmcy->writeHistory(data, id, abortCode);
}

int8_t emcyGetErrorBehavior(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (boundEmcy == nullptr) return missingEmcy(abortCode);
    return boundEmcy->readBehavior(data, id, abortCode);
}

int8_t emcySetErrorBehavior(const Data &data, int32_t id,
                            SDOAbortCodes &abortCode) {
    if (boundEmcy == nullptr) return missingEmcy(abortCode);
    return boundEmcy->writeBehavior(data, id, abortCode);
}

void ErrorHistory::clear() {
    for (unsigned i = 0; i < emcy::HISTORY_SIZE; i++) errors[i] = 0;
    size = 0;
}

void ErrorHistory::push(uint32_t error) {
    if (size > 0 && errors[0] == error) return;
    for (unsigned i = emcy::HISTORY_SIZE - 1; i > 0; i--)
        errors[i] = errors[i - 1];
    errors[0] = error;
    if (size < emcy::HISTORY_SIZE) size++;
}

EMCY::EMCY(ObjectDictionnary &od, CanTransport &transport, NMT &nmt,
           uint8_t nodeId)
    : od(od), transport(transport), nmt(nmt), nodeId(nodeId) {}

void EMCY::init() {
    registerId = od.findObject(emcy::ERROR_REGISTER_INDEX);
    historyId = od.findObject(emcy::ERROR_HISTORY_INDEX);
    behaviorId = od.findObject(emcy::ERROR_BEHAVIOR_INDEX);
    if (behaviorId >= 0) {
        communicationBehavior = static_cast<ErrorBehaviorValue>(
            defaultOf(behaviorId + emcy::BEHAVIOR_COMMUNICATION).u8);
        deviceBehavior = static_cast<ErrorBehaviorValue>(
            defaultOf(behaviorId + emcy::BEHAVIOR_DEVICE).u8);
    }
}

Data EMCY::defaultOf(int32_t id) const {
    Metadata *metadata = od.getMetadata(id);
    Data data;
    data.u64 = 0;
    if (metadata != nullptr) data = metadata->getDefaultValue();
    return data;
}

void EMCY::onNmtState(NMTStates state) {
    enabled = state == NMTState_PreOperational || state == NMTState_Operational;
}

void EMCY::setErrorBehavior(ErrorBehaviorValue onCommunicationError,
                            ErrorBehaviorValue onDeviceError) {
    communicationBehavior = onCommunicationError;
    deviceBehavior = onDeviceError;
}

void EMCY::send(uint16_t errorCode, uint32_t manufacturerCode) {
    transport.sendFrame(
        emcy::makeMessage(nodeId, errorCode, errorRegister, manufacturerCode));
}

void EMCY::raiseError(uint16_t errorCode, uint16_t manufacturerCode) {
    if (!enabled) return;
    errorRegister |= emcy::registerMask(bitOf(errorCode)) |
                     emcy::registerMask(ErrorRegisterBit_Generic);
    errors.push(emcy::historyEntry(errorCode, manufacturerCode));
    send(errorCode, manufacturerCode);
    applyBehavior(errorCode);
}

void EMCY::applyBehavior(uint16_t errorCode) {
    const ErrorBehaviorValue behavior = isCommunicationError(errorCode)
                                            ? communicationBehavior
                                            : deviceBehavior;
    switch (behavior) {
        case ErrorBehaviorValue_PreOperational:
            if (nmt.getState() == NMTState_Operational)
                nmt.setTransition(NMTServiceCommand_EnterPreOperational);
            break;
        case ErrorBehaviorValue_Stop:
            nmt.setTransition(NMTServiceCommand_Stop);
            break;
        case ErrorBehaviorValue_None:
            break;
    }
}

void EMCY::clearErrorBit(ErrorRegisterBits bit) {
    const uint8_t before = errorRegister;
    const uint8_t generic = emcy::registerMask(ErrorRegisterBit_Generic);
    if (bit != ErrorRegisterBit_Generic)
        errorRegister &= static_cast<uint8_t>(~emcy::registerMask(bit));
    if ((errorRegister & static_cast<uint8_t>(~generic)) == 0)
        errorRegister = 0;
    if (before != 0 && errorRegister == 0) send(EMCYErrorCode_Reset, 0);
}

void EMCY::clearErrorBit(EMCYErrorCodes code) {
    clearErrorBit(bitOf(static_cast<uint16_t>(code)));
}

int8_t EMCY::readRegister(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (id != registerId)
        return fail(abortCode, SDOAbortCode_ObjectNonExistent);
    abortCode = SDOAbortCode_OK;
    data.u8 = errorRegister;
    return 0;
}

int8_t EMCY::readHistory(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (historyId < 0 || id < historyId || id > historyId + emcy::HISTORY_SIZE)
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    if (id == historyId) {
        data.u8 = errors.count();
        return 0;
    }
    const unsigned position = static_cast<unsigned>(id - historyId - 1);
    if (position >= errors.count())
        return fail(abortCode, SDOAbortCode_NoDataAvailable);
    data.u32 = errors.at(position);
    return 0;
}

int8_t EMCY::writeHistory(const Data &data, int32_t id,
                          SDOAbortCodes &abortCode) {
    if (historyId < 0 || id != historyId)
        return fail(abortCode, SDOAbortCode_AttemptWriteOnReadOnly);
    if (data.u8 != 0)
        return fail(abortCode, SDOAbortCode_InvalidDownloadParameterValue);
    abortCode = SDOAbortCode_OK;
    errors.clear();
    return 0;
}

int8_t EMCY::readBehavior(Data &data, int32_t id, SDOAbortCodes &abortCode) {
    if (behaviorId < 0 || id < behaviorId ||
        id > behaviorId + emcy::BEHAVIOR_ENTRIES)
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    abortCode = SDOAbortCode_OK;
    switch (id - behaviorId) {
        case emcy::BEHAVIOR_COUNT:
            data.u8 = emcy::BEHAVIOR_ENTRIES;
            return 0;
        case emcy::BEHAVIOR_COMMUNICATION:
            data.u8 = static_cast<uint8_t>(communicationBehavior);
            return 0;
        default:
            data.u8 = static_cast<uint8_t>(deviceBehavior);
            return 0;
    }
}

int8_t EMCY::writeBehavior(const Data &data, int32_t id,
                           SDOAbortCodes &abortCode) {
    if (behaviorId < 0 || id <= behaviorId ||
        id > behaviorId + emcy::BEHAVIOR_ENTRIES)
        return fail(abortCode, SDOAbortCode_SubindexNonExistent);
    if (data.u8 > ErrorBehaviorValue_Stop)
        return fail(abortCode, SDOAbortCode_InvalidDownloadParameterValue);
    abortCode = SDOAbortCode_OK;
    const ErrorBehaviorValue value = static_cast<ErrorBehaviorValue>(data.u8);
    if (id - behaviorId == emcy::BEHAVIOR_COMMUNICATION)
        communicationBehavior = value;
    else
        deviceBehavior = value;
    return 0;
}
