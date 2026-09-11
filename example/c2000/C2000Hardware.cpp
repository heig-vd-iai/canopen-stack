#include "C2000Hardware.hpp"

extern "C" {
#include "F021_F2838x_CM.h"
#include "cm.h"
}
#include "frame.hpp"
#include "node.hpp"
#include "od.hpp"

using namespace CANopen;

IPCStruct *remoteIPC;

void CANopen::C2000Hardware::init() {
    SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_MCAN_A);

    MCAN_InitParams initParams;
    memset(&initParams, 0, sizeof(initParams));
    initParams.fdMode = 0;
    initParams.brsEnable = 0;
    initParams.emulationEnable = 1;

    MCAN_BitTimingParams bitTimes;
    memset(&bitTimes, 0, sizeof(bitTimes));
    // Prescaler = 25 MHz aux clock / DIV5 / 125 kHz bitrate => 40
    bitTimes.nomRatePrescalar = bitTimes.dataRatePrescalar = 9;
    bitTimes.nomTimeSeg1 = bitTimes.dataTimeSeg1 = 1;
    bitTimes.nomTimeSeg2 = bitTimes.dataTimeSeg2 = 1;
    bitTimes.nomSynchJumpWidth = bitTimes.dataSynchJumpWidth = 0;

    MCAN_MsgRAMConfigParams msgRAMConfigParams;
    memset(&msgRAMConfigParams, 0, sizeof(msgRAMConfigParams));
    msgRAMConfigParams.lss = filtersLen;
    msgRAMConfigParams.rxFIFO0startAddr =
        msgRAMConfigParams.flssa +
        (msgRAMConfigParams.lss * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4);
    msgRAMConfigParams.rxFIFO0size = RXFIFO_LEN;
    msgRAMConfigParams.rxFIFO0OpMode = 0;
    msgRAMConfigParams.rxFIFO0ElemSize = MCAN_ELEM_SIZE_8BYTES;

    msgRAMConfigParams.txStartAddr =
        msgRAMConfigParams.rxFIFO0startAddr +
        (MCAN_getMsgObjSize(msgRAMConfigParams.rxFIFO0ElemSize) *
         msgRAMConfigParams.rxFIFO0size * 4);
    msgRAMConfigParams.txFIFOSize = TXFIFO_LEN;
    msgRAMConfigParams.txBufMode = 0;
    msgRAMConfigParams.txBufElemSize = MCAN_ELEM_SIZE_8BYTES;

    MCAN_ConfigParams configParams;
    memset(&configParams, 0, sizeof(configParams));
    configParams.filterConfig.rrfe = 1;
    configParams.filterConfig.anfs = 2;

    while (!MCAN_isMemInitDone(MCAN0_BASE)) {
    }

    MCAN_setOpMode(MCAN0_BASE, MCAN_OPERATION_MODE_SW_INIT);
    while (MCAN_getOpMode(MCAN0_BASE) != MCAN_OPERATION_MODE_SW_INIT) {
    }
    MCAN_init(MCAN0_BASE, &initParams);
    MCAN_setBitTime(MCAN0_BASE, &bitTimes);
    MCAN_msgRAMConfig(MCAN0_BASE, &msgRAMConfigParams);
    MCAN_config(MCAN0_BASE, &configParams);
    for (unsigned i = 0; i < filtersLen; i++) {
        MCAN_StdMsgIDFilterElement stdFilter;
        memset(&stdFilter, 0, sizeof(stdFilter));
        stdFilter.sfid1 = filters[i];
        stdFilter.sfid2 = 0x7FF;
        stdFilter.sfec = MCAN_STDFILTEC_FIFO0;
        stdFilter.sft = MCAN_STDFILT_CLASSIC;
        MCAN_addStdMsgIDFilter(MCAN0_BASE, i, &stdFilter);
    }
    MCAN_setOpMode(MCAN0_BASE, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_getOpMode(MCAN0_BASE) != MCAN_OPERATION_MODE_NORMAL) {
    }

    // CPU timer
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPreScaler(CPUTIMER0_BASE, CM_CLK_FREQ / 1000000U - 1);
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // Flash
    Flash_initModule(FLASH0CTRL_BASE, FLASH0ECC_BASE, 2);
    Flash_claimPumpSemaphore(FLASH_CM_WRAPPER);
    initFlash();

    // IPC communication
    uint32_t dummy;
    IPC_init(IPC_CM_L_CPU1_R);

    IPC_waitForFlag(IPC_CM_L_CPU1_R, IPC_FLAG31);
    IPC_readCommand(IPC_CM_L_CPU1_R, IPC_FLAG31, true, &dummy,
                    (uint32_t *)&remoteIPC, &dummy);
    IPC_ackFlagRtoL(IPC_CM_L_CPU1_R, IPC_FLAG31);

    IPC_sendCommand(IPC_CM_L_CPU1_R, IPC_FLAG31, true, 0, (uint32_t)&localIPC,
                    0);
    IPC_waitForAck(IPC_CM_L_CPU1_R, IPC_FLAG31);

    ipc.setRemoteIPC(remoteIPC);
    ipc.setReady(true);

    node.nmt().onReset = &rebootApplication;
    DEVICE_DELAY_US(1000);
}

void C2000Hardware::update() {
    MCAN_getRxFIFOStatus(MCAN0_BASE, &rxStatus);
    if (rxStatus.fillLvl) {
        MCAN_readMsgRam(MCAN0_BASE, MCAN_MEM_TYPE_FIFO, 0, MCAN_RX_FIFO_NUM_0,
                        &msg);
        // Frame frame((uint16_t)((msg.id >> 1) & 0x7FF));
        Frame frame = Frame::fromCobId((msg.id >> 1) & 0x7FF);
        frame.dlc = msg.dlc;
        frame.rtr = msg.rtr;
        for (unsigned i = 0; i < frame.dlc; i++) frame.data[i] = msg.data[i];
        MCAN_writeRxFIFOAck(MCAN0_BASE, MCAN_RX_FIFO_NUM_0, rxStatus.getIdx);
        node.receiveFrame(frame);
    }
    //    if (ipc.getState() != IPCState_idle) {
    //        ipc.update();
    //        return;
    //    }
    if (ipc.isRemoteReady()) updateError();
}

void C2000Hardware::sendFrame(const Frame &frame) {
    // GPIO_writePin(DEVICE_GPIO_PIN_LED2, 0);
    MCAN_TxBufElement sendmsg;
    MCAN_TxFIFOStatus status;
    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.id = ((uint32_t)frame.getCobID()) << 18;
    sendmsg.dlc = frame.dlc;
    sendmsg.rtr = frame.rtr;
    for (unsigned i = 0; i < frame.dlc; i++) sendmsg.data[i] = frame.data[i];
    MCAN_getTxFIFOQueStatus(MCAN0_BASE, &status);
    MCAN_writeMsgRam(MCAN0_BASE, MCAN_MEM_TYPE_FIFO, status.putIdx, &sendmsg);
    MCAN_txBufAddReq(MCAN0_BASE, status.putIdx);
    // GPIO_writePin(DEVICE_GPIO_PIN_LED2, 1);
}

uint32_t C2000Hardware::getTime_us() {
    return 0xFFFFFFFF - CPUTimer_getTimerCount(CPUTIMER0_BASE);
}

void C2000Hardware::updateError() {
    uint16_t errorCode, manufacturerCode;
    if (ipc.fetchError(errorCode, manufacturerCode)) {
        node.emcy().raiseError(errorCode, manufacturerCode);
        ipc.update();
    }
    if (ipc.clearErrorResponse()) {
        node.emcy().clearErrorBit(ErrorRegisterBit_Current);
        node.emcy().clearErrorBit(ErrorRegisterBit_Voltage);
        node.emcy().clearErrorBit(ErrorRegisterBit_Temperature);
        node.emcy().clearErrorBit(ErrorRegisterBit_Generic);
        if (node.emcy().getErrorRegister() == 0) {
            node.emcy().clearHistory();
        }
        ipc.update();
    }
    if (ipc.getState() == IPCState_pendingErrorResponse ||
        ipc.getState() == IPCState_pendingClearResponse) {
        ipc.update();
    }
}

void C2000Hardware::resetRemote() { ipc.update(); }

int8_t C2000Hardware::getRemoteData(
    Data &data, int32_t id,      // FIXME int_least8_t
    SDOAbortCodes &abortCode) {  // TODO: test if pending
                                 // response at new request
    uint32_t time = node.hardware().getTime_us();
    if (__builtin_expect(
            node.od().getMetadata(id)->dataType == DataType::DOMAIN, false)) {
        return getRemoteDomain(data, id, abortCode);
    }
    IPCState state = ipc.getState();
    IPCState remoteState = ipc.getRemoteState();
    abortCode = SDOAbortCode_OK;
    if (state == IPCState_idle) {
        ipc.getRequest(id);
    } else if (state == IPCState_pendingGetRequest &&
               remoteState == IPCState_pendingGetResponse) {
        ipc.retriveRemoteRequest(data);
        ipc.update();
        monitoring.setCycleTimeHardware(getTime_us() - time);
        return 0;
    }
    monitoring.setCycleTimeHardware(getTime_us() - time);
    return 1;
}

int8_t C2000Hardware::getRemoteDomain(Data &data, int32_t id,
                                      SDOAbortCodes &abortCode) {
    IPCState state = ipc.getState();
    IPCState remoteState = ipc.getRemoteState();
    abortCode = SDOAbortCode_OK;
    if (state == IPCState_idle) {
        ipc.getDomainRequest(id);
    } else if (state == IPCState_pendingGetDomainRequest &&
               remoteState == IPCState_pendingGetDomainResponse) {
        ipc.retriveRemoteDomainRequest(data);
        ipc.update();
        return 0;
    }
    return 1;
}

int8_t C2000Hardware::setRemoteData(const Data &data, int32_t id,
                                    SDOAbortCodes &abortCode) {
    IPCState state = ipc.getState();
    IPCState remoteState = ipc.getRemoteState();
    abortCode = SDOAbortCode_OK;
    if (state == IPCState_idle) {
        ipc.setRequest(data, id);
    } else if (state == IPCState_pendingSetRequest &&
               remoteState == IPCState_pendingSetResponse) {
        ipc.update();
        return 0;
    }
    return 1;
}

void C2000Hardware::configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) {
    int j = 0;
    for (int i = 0; i < PDO_MAX_SIZE; i++) {
        if (odIDs[i] != 0) {
            rpdoIndex[i + pdoIndex * PDO_MAX_SIZE].first = odIDs[i];
        } else {
            rpdoIndex[i + pdoIndex * PDO_MAX_SIZE].first = -1;
        }
    }
    memset(ipc.localIPCStruct->rpdosOdID, -1,
           sizeof(ipc.localIPCStruct->rpdosOdID));
    for (int i = 0; i < PDO_MAX; i++) {
        if (rpdoIndex[i].first != -1) {
            ipc.localIPCStruct->rpdosOdID[j] = rpdoIndex[i].first;
            rpdoIndex[i].second = j;
            j++;
        } else {
            rpdoIndex[i].second = -1;
        }
    }
    ipc.configPDO();
    while (!ipc.getRemotePDOState().responsePending) {
    }  // TODO: add timeout
    ipc.ackPDOConfig();
}

void C2000Hardware::configRemoteTPDO(uint16_t pdoIndex, int32_t odIDs[]) {
    int j = 0;
    for (int i = 0; i < PDO_MAX_SIZE; i++) {
        if (odIDs[i] != 0) {
            tpdoIndex[i + pdoIndex * PDO_MAX_SIZE].first = odIDs[i];
        } else {
            tpdoIndex[i + pdoIndex * PDO_MAX_SIZE].first = -1;
        }
    }
    memset(ipc.localIPCStruct->tpdosOdID, -1,
           sizeof(ipc.localIPCStruct->tpdosOdID));
    for (int i = 0; i < PDO_MAX; i++) {
        if (tpdoIndex[i].first != -1) {
            ipc.localIPCStruct->tpdosOdID[j] = tpdoIndex[i].first;
            tpdoIndex[i].second = j;
            j++;
        } else {
            tpdoIndex[i].second = -1;
        }
    }
    ipc.configPDO();
    while (!ipc.getRemotePDOState().responsePending) {
    }  // TODO: add timeout
    ipc.ackPDOConfig();
}

void C2000Hardware::disablePDO() { ipc.disablePDO(); }

void C2000Hardware::enablePDO() { ipc.enablePDO(); }

void C2000Hardware::getRemoteTPDO(uint16_t pdoIndex, Data data[]) {
    Data temp[PDO_MAX];
    ipc.getRemoteTPDO(temp);
    for (int i = 0; i < PDO_MAX_SIZE; i++) {
        if (tpdoIndex[i + pdoIndex * PDO_MAX_SIZE].second == -1) break;
        data[i] = temp[tpdoIndex[i + pdoIndex * PDO_MAX_SIZE].second];
    }
}

void C2000Hardware::setRemoteRPDO(uint16_t pdoIndex, Data data[]) {
    Data temp[PDO_MAX];
    ipc.getRPDO(temp);
    for (int i = 0; i < PDO_MAX_SIZE; i++) {
        temp[rpdoIndex[i + pdoIndex * PDO_MAX_SIZE].second] = data[i];
    }
    ipc.setRPDO(temp);
}

void C2000Hardware::initFlash() {
    Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, CM_CLK_FREQ / 1000000U);
    Fapi_setActiveFlashBank(Fapi_FlashBank0);
}

bool C2000Hardware::eraseFlash(uint8_t parameterGroup) {
    uint32_t origin = getFlashSector(parameterGroup);
    uint32_t length = getFlashLength(parameterGroup);
    Fapi_StatusType status =
        Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)origin);
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady) {
    }
    if (status != Fapi_Status_Success || Fapi_getFsmStatus() != 0) return false;
    Fapi_FlashStatusWordType flashStatus;
    status = Fapi_doBlankCheck((uint32_t *)origin, length / 4, &flashStatus);
    return status == Fapi_Status_Success;
}

bool C2000Hardware::writeFlash(const uint8_t *src, size_t len,
                               uint8_t parameterGroup) {
    uint32_t origin = getFlashSector(parameterGroup);
    const unsigned step = 8;
    uint32_t flashAddr, i;
    for (i = 0, flashAddr = origin; flashAddr < origin + len;
         i += step, flashAddr += step) {
        uint8_t buffsize = len - i > step ? step : len - i;
        Fapi_StatusType status = Fapi_issueProgrammingCommand(
            (uint32_t *)flashAddr, (uint8_t *)src + i, buffsize, 0, 0,
            Fapi_AutoEccGeneration);
        while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy) {
        }
        if (status != Fapi_Status_Success || Fapi_getFsmStatus() != 0)
            return false;
    }
    return true;
}

bool C2000Hardware::saveDataToFlash(uint8_t parameterGroup) {
    uint_least16_t minIndex = 0x0000;
    uint_least16_t maxIndex = 0xFFFF;

    switch ((ParameterGroups)parameterGroup) {
        case ParameterGroup_All:
            return false;
        case ParameterGroup_Communication:
            minIndex = 0x1000;
            maxIndex = 0x1FFF;
            break;
        case ParameterGroup_Application:
            minIndex = 0x6000;
            maxIndex = 0x9FFF;
            break;
        case ParameterGroup_ManufacturerA:
            minIndex = 0x2000;
            maxIndex = 0x4FFF;
            break;
    }
    eraseFlash(parameterGroup);
    uint32_t origin = getFlashSector(parameterGroup);
    uint32_t flashPtr = origin;
    int count;
    int16_t size;
    SDOAbortCodes abortCode;
    for (int i = 0; i < node.od().length; i++) {
        if (CANopenOD::objectIndexTable[i].first >= minIndex &&
            CANopenOD::objectIndexTable[i].first <= maxIndex) {
            size = node.od().getSize(i);
            if (i == 261) { //TODO: remove
                int a = 42;
            }
            Data tmp;
            int8_t ret;
            count = 0;
            while (node.od().readData(tmp, i, abortCode) == 1) {
                count++;
                if (count > MAX_WRITE_TRY) {
                    break;
                }
            }
            Fapi_StatusType status = Fapi_issueProgrammingCommand(
                (uint32_t *)(flashPtr), (uint8_t *)&tmp.u64, 8, 0, 0,
                Fapi_AutoEccGeneration);
            flashPtr += 8;
            while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy) {
            }
            Fapi_FlashStatusWordType oFlashStatusWord;
            if (status != Fapi_Status_Success) return false;
            DEVICE_DELAY_US(100);
            Fapi_FlashStatusType statuss = Fapi_getFsmStatus();
            if (statuss != 0)
                int a = 42;
            //            if(Fapi_doVerifyByByte((uint8_t *)flashPtr, 16,
            //            //FIXME (uint8_t *)&tmp,
            //                                    &oFlashStatusWord) !=
            //                                    Fapi_Status_Success) {
            //                return false;
            //            }
        }
    }
    return true;
}

bool C2000Hardware::saveSignatureToFlash(uint64_t signature) {
    uint32_t origin = SIGNATURE_ORIGIN;
    uint32_t length = SIGNATURE_LENGTH;
    Fapi_StatusType status =
        Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32_t *)origin);
    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady) {
    }
    DEVICE_DELAY_US(100);
    if (status != Fapi_Status_Success || Fapi_getFsmStatus() != 0) return false;
    Fapi_FlashStatusWordType flashStatus;
    status = Fapi_doBlankCheck((uint32_t *)origin, length / 4, &flashStatus);
    uint32_t flashPtr = origin;
    status = Fapi_issueProgrammingCommand((uint32_t *)(flashPtr),
                                          (uint8_t *)&signature, 8, 0, 0,
                                          Fapi_AutoEccGeneration);
    while (Fapi_checkFsmForReady() == Fapi_Status_FsmBusy) {
    }
    Fapi_FlashStatusWordType oFlashStatusWord;
    if (status != Fapi_Status_Success) return false;
    DEVICE_DELAY_US(100);
    Fapi_FlashStatusType statuss = Fapi_getFsmStatus();
                if (statuss != 0)
                    int a = 42;
    //            if(Fapi_doVerifyByByte((uint8_t *)flashPtr, 16,
    //            (uint8_t *)&tmp,
    //                                    &oFlashStatusWord) !=
    //                                    Fapi_Status_Success) {
    //                return false;
    //            }
}

bool C2000Hardware::loadSignatureFromFlash(uint64_t &signature) {
    uint32_t origin = SIGNATURE_ORIGIN;
    Fapi_FlashStatusWordType flashStatus;
    Fapi_StatusType status =
        Fapi_doBlankCheck((uint32_t *)origin, 1, &flashStatus);
    uint32_t flashPtr = origin;
    if (status == Fapi_Error_Fail) {
        memcpy((uint8_t *)&signature, (uint8_t *)flashPtr, 8);
        return true;
    }
    return false;
}

bool C2000Hardware::loadDataFromFlash(uint8_t parameterGroup) {
    uint32_t origin = getFlashSector(parameterGroup);
    uint32_t length = getFlashLength(parameterGroup);
    uint_least16_t minIndex = 0x0000;
    uint_least16_t maxIndex = 0xFFFF;

    switch ((ParameterGroups)parameterGroup) {
        case ParameterGroup_All:
            return false;
        case ParameterGroup_Communication:
            minIndex = 0x1000;
            maxIndex = 0x1FFF;
            break;
        case ParameterGroup_Application:
            minIndex = 0x6000;
            maxIndex = 0x9FFF;
            break;
        case ParameterGroup_ManufacturerA:
            minIndex = 0x2000;
            maxIndex = 0x4FFF;  // 5000 to 5FFF is reserved for domain
            break;
    }

    Fapi_FlashStatusWordType flashStatus;
    Fapi_StatusType status = Fapi_doBlankCheck(
        (uint32_t *)origin, node.od().length / 4, &flashStatus);
    uint32_t flashPtr = origin;
    if (status == Fapi_Error_Fail) {
        int count = 0;
        for (int i = 0; i < node.od().length; i++) {
            if (CANopenOD::objectIndexTable[i].first >= minIndex &&
                CANopenOD::objectIndexTable[i].first <= maxIndex) {
                count = 0;
                Data tmp;
                SDOAbortCodes abortCode;
                memcpy((uint8_t *)&tmp, (uint8_t *)flashPtr, 8);
                flashPtr += 8;
                while (node.od().writeData(tmp, i, abortCode) == 1) {
                    if (count > MAX_WRITE_TRY) {
                        return false;
                    }
                    count++;
                }
                DEVICE_DELAY_US(200);
            }
        }
        return true;
    }else{
        return false;
    }
}

uint32_t C2000Hardware::getFlashSector(uint8_t parameterGroup) {
    switch (parameterGroup) {
        case ParameterGroup_All:
            return 0;
        case ParameterGroup_Communication:
            return PARAMETER_GROUP_COMMUNICATION_ORIGIN;
        case ParameterGroup_Application:
            return PARAMETER_GROUP_APPLICATION_ORIGIN;
        case ParameterGroup_ManufacturerA:
            return PARAMETER_GROUP_MANUFACTURER_A_ORIGIN;
            //        case ParameterGroup_ManufacturerB:
            //            return PARAMETER_GROUP_MANUFACTURER_B_ORIGIN;
    }
}

uint32_t C2000Hardware::getFlashLength(uint8_t parameterGroup) {
    switch (parameterGroup) {
        case ParameterGroup_All:
            return 0;
        case ParameterGroup_Communication:
            return PARAMETER_GROUP_COMMUNICATION_LENGTH;
        case ParameterGroup_Application:
            return PARAMETER_GROUP_APPLICATION_LENGTH;
        case ParameterGroup_ManufacturerA:
            return PARAMETER_GROUP_MANUFACTURER_A_LENGTH;
            //        case ParameterGroup_ManufacturerB:
            //            return PARAMETER_GROUP_MANUFACTURER_B_ORIGIN;
    }
}

bool C2000Hardware::reboot() {
    ipc.rebootApplication();
    while (ipc.getRemoteState() != IPCState_rebooting) {
    }
    ipc.update();
    return true;
}

bool CANopen::rebootApplication() {  // TODO: add timeout and error handling
    return node.hardware().reboot();
}
