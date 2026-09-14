#include "C2000Persistence.hpp"

#include <cstring>

#include "driverlib_cm.h"
extern "C" {
#include "F021_F2838x_CM.h"
#include "cm.h"
}
#include "od.hpp"
#include "od/parameterGroup.hpp"
#include "od_common.hpp"
#include "od_lookup.hpp"

using namespace CANopen;

// Flash commands and their polling loops must run from RAM: the CM has a
// single flash bank and cannot fetch code from it while the FSM works on it.
#if defined(__clang__) || defined(__GNUC__)
#define RAMFUNC __attribute__((section(".TI.ramfunc")))
#else
#define RAMFUNC _Pragma("CODE_SECTION(\".TI.ramfunc\")")
#endif

namespace {
constexpr uint32_t FSM_MAX_POLLS = 1000000;
constexpr uint32_t OBJECT_IMAGE_SIZE = sizeof(uint64_t);
}  // namespace

// Sector 13 is left to the bootloader (application metadata at 0x0027FF80).
const C2000Persistence::Sector C2000Persistence::sectors[] = {
    {ParameterGroup_Communication, 0x00270000, 0x00004000},
    {ParameterGroup_Application, 0x00274000, 0x00004000},
    {ParameterGroup_ManufacturerA, 0x00260000, 0x00010000},
};

const C2000Persistence::Sector C2000Persistence::signatureSector = {
    ParameterGroup_All, 0x00278000, 0x00004000};

const C2000Persistence::Sector *C2000Persistence::sectorOf(
    uint8_t parameterGroup) {
    for (const Sector &sector : sectors) {
        if (sector.parameterGroup == parameterGroup) return &sector;
    }
    return nullptr;
}

void C2000Persistence::init() {
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 2);
    Flash_claimPumpSemaphore(FLASH_CM_WRAPPER);
    Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, CM_CLK_FREQ / 1000000U);
    Fapi_setActiveFlashBank(Fapi_FlashBank0);
}

RAMFUNC
bool C2000Persistence::waitFsmReady() {
    for (uint32_t polls = 0; polls < FSM_MAX_POLLS; polls++) {
        if (Fapi_checkFsmForReady() == Fapi_Status_FsmReady) {
            return Fapi_getFsmStatus() == 0;
        }
    }
    return false;
}

RAMFUNC
bool C2000Persistence::eraseSector(const Sector &sector) {
    const Fapi_StatusType status = Fapi_issueAsyncCommandWithAddress(
        Fapi_EraseSector, reinterpret_cast<uint32_t *>(sector.origin));
    if (!waitFsmReady() || status != Fapi_Status_Success) return false;
    return isBlank(sector.origin, sector.length / 4);
}

bool C2000Persistence::isBlank(uint32_t address, uint32_t words) {
    Fapi_FlashStatusWordType flashStatus;
    return Fapi_doBlankCheck(reinterpret_cast<uint32_t *>(address), words,
                             &flashStatus) == Fapi_Status_Success;
}

RAMFUNC
bool C2000Persistence::program(uint32_t address, const uint64_t &value) {
    const Fapi_StatusType status = Fapi_issueProgrammingCommand(
        reinterpret_cast<uint32_t *>(address),
        const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(&value)),
        OBJECT_IMAGE_SIZE, 0, 0, Fapi_AutoEccGeneration);
    return waitFsmReady() && status == Fapi_Status_Success;
}

bool C2000Persistence::saveGroup(uint8_t parameterGroup) {
    const Sector *sector = sectorOf(parameterGroup);
    if (sector == nullptr || !eraseSector(*sector)) return false;
    uint32_t address = sector->origin;
    for (int32_t id = 0; id < static_cast<int32_t>(OD_LENGTH); id++) {
        if (!inParameterGroup(CANopenOD::objectIndexTable[id].first,
                              parameterGroup))
            continue;
        if (address + OBJECT_IMAGE_SIZE > sector->origin + sector->length)
            return false;
        Data value;
        value.u64 = 0;
        SDOAbortCodes abortCode;
        readDataWait(value, id, abortCode);
        if (!program(address, value.u64)) return false;
        address += OBJECT_IMAGE_SIZE;
    }
    return true;
}

bool C2000Persistence::loadGroup(uint8_t parameterGroup) {
    const Sector *sector = sectorOf(parameterGroup);
    if (sector == nullptr) return false;
    if (isBlank(sector->origin, OBJECT_IMAGE_SIZE / 4)) return false;
    uint32_t address = sector->origin;
    for (int32_t id = 0; id < static_cast<int32_t>(OD_LENGTH); id++) {
        if (!inParameterGroup(CANopenOD::objectIndexTable[id].first,
                              parameterGroup))
            continue;
        if (address + OBJECT_IMAGE_SIZE > sector->origin + sector->length)
            return false;
        Data value;
        memcpy(&value.u64, reinterpret_cast<const void *>(address),
               OBJECT_IMAGE_SIZE);
        address += OBJECT_IMAGE_SIZE;
        SDOAbortCodes abortCode;
        writeDataWait(value, id, abortCode);
    }
    return true;
}

bool C2000Persistence::saveSignature(uint64_t signature) {
    if (!eraseSector(signatureSector)) return false;
    return program(signatureSector.origin, signature);
}

bool C2000Persistence::loadSignature(uint64_t &signature) {
    if (isBlank(signatureSector.origin, OBJECT_IMAGE_SIZE / 4)) return false;
    memcpy(&signature, reinterpret_cast<const void *>(signatureSector.origin),
           OBJECT_IMAGE_SIZE);
    return true;
}
