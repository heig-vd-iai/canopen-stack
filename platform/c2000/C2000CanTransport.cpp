#include "C2000CanTransport.hpp"

#include <cstring>

#include "driverlib_cm.h"
extern "C" {
#include "cm.h"
}
#include "frame.hpp"

using namespace CANopen;

namespace {

constexpr uint32_t RXFIFO_LEN = 16;
constexpr uint32_t TXFIFO_LEN = 16;
constexpr uint32_t MCAN_CLK_FREQ = 25000000;
constexpr uint32_t CAN_BITRATE = 500000;
constexpr uint32_t CAN_TSEG1 = 7;
constexpr uint32_t CAN_TSEG2 = 2;
constexpr uint32_t CAN_SJW = 2;
constexpr uint32_t CAN_TQ_PER_BIT = 1 + CAN_TSEG1 + CAN_TSEG2;
constexpr uint32_t CAN_PRESCALER = MCAN_CLK_FREQ / (CAN_BITRATE * CAN_TQ_PER_BIT);
constexpr uint16_t broadcastCobIds[] = {0x000, 0x080, 0x100};
constexpr uint16_t nodeCobIdBases[] = {0x080, 0x180, 0x200, 0x280,
                                       0x300, 0x380, 0x400, 0x480,
                                       0x500, 0x580, 0x600, 0x700};
constexpr uint32_t broadcastCount =
    sizeof(broadcastCobIds) / sizeof(broadcastCobIds[0]);
constexpr uint32_t nodeCount =
    sizeof(nodeCobIdBases) / sizeof(nodeCobIdBases[0]);
constexpr uint32_t filterCount = broadcastCount + nodeCount;

void addFilter(uint32_t slot, uint16_t cobId) {
    MCAN_StdMsgIDFilterElement filter;
    memset(&filter, 0, sizeof(filter));
    filter.sfid1 = cobId;
    filter.sfid2 = 0x7FF;
    filter.sfec = MCAN_STDFILTEC_FIFO0;
    filter.sft = MCAN_STDFILT_CLASSIC;
    MCAN_addStdMsgIDFilter(MCAN0_BASE, slot, &filter);
}

}  // namespace

C2000CanTransport::C2000CanTransport(uint8_t nodeId) : nodeId(nodeId) {}

void C2000CanTransport::init() {
    initCan();
    initTimer();
}

void C2000CanTransport::initCan() {
    SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_MCAN_A);

    MCAN_InitParams initParams;
    memset(&initParams, 0, sizeof(initParams));
    initParams.fdMode = 0;
    initParams.brsEnable = 0;
    initParams.emulationEnable = 1;

    MCAN_BitTimingParams bitTimes;
    memset(&bitTimes, 0, sizeof(bitTimes));
    bitTimes.nomRatePrescalar = bitTimes.dataRatePrescalar = CAN_PRESCALER - 1;
    bitTimes.nomTimeSeg1 = bitTimes.dataTimeSeg1 = CAN_TSEG1 - 1;
    bitTimes.nomTimeSeg2 = bitTimes.dataTimeSeg2 = CAN_TSEG2 - 1;
    bitTimes.nomSynchJumpWidth = bitTimes.dataSynchJumpWidth = CAN_SJW - 1;

    MCAN_MsgRAMConfigParams msgRam;
    memset(&msgRam, 0, sizeof(msgRam));
    msgRam.lss = filterCount;
    msgRam.rxFIFO0startAddr =
        msgRam.flssa + (msgRam.lss * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4);
    msgRam.rxFIFO0size = RXFIFO_LEN;
    msgRam.rxFIFO0OpMode = 0;
    msgRam.rxFIFO0ElemSize = MCAN_ELEM_SIZE_8BYTES;
    msgRam.txStartAddr =
        msgRam.rxFIFO0startAddr +
        (MCAN_getMsgObjSize(msgRam.rxFIFO0ElemSize) * msgRam.rxFIFO0size * 4);
    msgRam.txFIFOSize = TXFIFO_LEN;
    msgRam.txBufMode = 0;
    msgRam.txBufElemSize = MCAN_ELEM_SIZE_8BYTES;

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
    MCAN_msgRAMConfig(MCAN0_BASE, &msgRam);
    MCAN_config(MCAN0_BASE, &configParams);
    addFilters();
    MCAN_setOpMode(MCAN0_BASE, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_getOpMode(MCAN0_BASE) != MCAN_OPERATION_MODE_NORMAL) {
    }
}

void C2000CanTransport::addFilters() {
    uint32_t slot = 0;
    for (uint32_t i = 0; i < broadcastCount; i++) {
        addFilter(slot++, broadcastCobIds[i]);
    }
    for (uint32_t i = 0; i < nodeCount; i++) {
        addFilter(slot++, static_cast<uint16_t>(nodeCobIdBases[i] + nodeId));
    }
}

void C2000CanTransport::initTimer() {
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPreScaler(CPUTIMER0_BASE, CM_CLK_FREQ / 1000000U - 1);
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER0_BASE);
}

void C2000CanTransport::sendFrame(const Frame &frame) {
    MCAN_TxBufElement element;
    memset(&element, 0, sizeof(element));
    element.id = static_cast<uint32_t>(frame.getCobID()) << 18;
    element.dlc = frame.dlc;
    element.rtr = frame.rtr;
    memcpy(element.data, frame.data, frame.dlc);
    MCAN_TxFIFOStatus status;
    MCAN_getTxFIFOQueStatus(MCAN0_BASE, &status);
    MCAN_writeMsgRam(MCAN0_BASE, MCAN_MEM_TYPE_FIFO, status.putIdx, &element);
    MCAN_txBufAddReq(MCAN0_BASE, status.putIdx);
}

bool C2000CanTransport::receiveFrame(Frame &frame) {
    MCAN_RxFIFOStatus status;
    memset(&status, 0, sizeof(status));
    status.num = MCAN_RX_FIFO_NUM_0;
    MCAN_getRxFIFOStatus(MCAN0_BASE, &status);
    if (status.fillLvl == 0) return false;
    MCAN_RxBufElement element;
    MCAN_readMsgRam(MCAN0_BASE, MCAN_MEM_TYPE_FIFO, 0, MCAN_RX_FIFO_NUM_0,
                    &element);
    MCAN_writeRxFIFOAck(MCAN0_BASE, MCAN_RX_FIFO_NUM_0, status.getIdx);
    frame = Frame::fromCobId(static_cast<uint16_t>((element.id >> 18) & 0x7FF));
    frame.dlc = element.dlc > CAN_DATA_LENGTH ? CAN_DATA_LENGTH : element.dlc;
    frame.rtr = element.rtr;
    memcpy(frame.data, element.data, frame.dlc);
    return true;
}

uint32_t C2000CanTransport::getTime_us() {
    return 0xFFFFFFFF - CPUTimer_getTimerCount(CPUTIMER0_BASE);
}
