#pragma once
#include "driverlib_cm.h"

#include "hardwareInterface.hpp"
#include "ipcCommunication.hpp"
#include "monitoring.hpp"
#include "od.hpp"


#define CMBANK0_SECTOR10_ORIGIN 0x00270000
#define CMBANK0_SECTOR10_LENGTH 0x00004000
#define PARAMETER_GROUP_COMMUNICATION_ORIGIN CMBANK0_SECTOR10_ORIGIN
#define PARAMETER_GROUP_COMMUNICATION_LENGTH CMBANK0_SECTOR10_LENGTH

#define CMBANK0_SECTOR11_ORIGIN 0x00274000
#define CMBANK0_SECTOR11_LENGTH 0x00004000
#define PARAMETER_GROUP_APPLICATION_ORIGIN CMBANK0_SECTOR11_ORIGIN
#define PARAMETER_GROUP_APPLICATION_LENGTH CMBANK0_SECTOR11_LENGTH

#define CMBANK0_SECTOR12_ORIGIN 0x00278000
#define CMBANK0_SECTOR12_LENGTH 0x00004000
#define PARAMETER_GROUP_MANUFACTURER_A_ORIGIN CMBANK0_SECTOR12_ORIGIN
#define PARAMETER_GROUP_MANUFACTURER_A_LENGTH CMBANK0_SECTOR12_LENGTH

#define CMBANK0_SECTOR13_ORIGIN 0x0027C000
#define CMBANK0_SECTOR13_LENGTH 0x00003FF0
#define PARAMETER_GROUP_MANUFACTURER_B_ORIGIN CMBANK0_SECTOR13_ORIGIN
#define PARAMETER_GROUP_MANUFACTURER_B_LENGTH CMBANK0_SECTOR13_LENGTH

#define CMBANK0_SECTOR14_ORIGIN 0x0027FFF0
#define CMBANK0_SECTOR14_LENGTH 0x00000010
#define SIGNATURE_ORIGIN CMBANK0_SECTOR14_ORIGIN
#define SIGNATURE_LENGTH CMBANK0_SECTOR14_LENGTH

#define RXFIFO_LEN 16
#define TXFIFO_LEN 16


extern IPCStruct localIPC;

constexpr uint16_t filters[] = {
    0,                   // NMT
    0x80,                // SYNC
    0x80 + OD_NODE_ID,   // EMCY
    0x100,               // TIME
    0x180 + OD_NODE_ID,  // TPDO1
    0x200 + OD_NODE_ID,  // RPDO1
    0x280 + OD_NODE_ID,  // TPDO2
    0x300 + OD_NODE_ID,  // RPDO2
    0x380 + OD_NODE_ID,  // TPDO3
    0x400 + OD_NODE_ID,  // RPDO3
    0x480 + OD_NODE_ID,  // TPDO4
    0x500 + OD_NODE_ID,  // RPDO4
    0x580 + OD_NODE_ID,  // TSDO
    0x600 + OD_NODE_ID,  // RSDO
    0x700 + OD_NODE_ID   // HEARTBEAT
};
constexpr unsigned filtersLen = sizeof(filters) / sizeof(filters[0]);

namespace CANopen {

bool rebootApplication();

class C2000Hardware : public HardwareInterface {
   private:
    MCAN_RxFIFOStatus rxStatus = {.num = MCAN_RX_FIFO_NUM_0};
    MCAN_RxBufElement msg;
    uint16_t errorCode;
    uint16_t manufacturerCode;
    uint16_t test;
    std::pair<int32_t, int16_t> tpdoIndex[PDO_MAX];
    std::pair<int32_t, int16_t> rpdoIndex[PDO_MAX];
    Monitoring monitoring;

   public:
    IPC ipc;
    void init() override;
    void update() override;
    void sendFrame(const Frame &frame) override;
    uint32_t getTime_us() override;
    void updateError() override;
    void resetRemote() override;
    int8_t getRemoteData(Data &data, int32_t id,
                         SDOAbortCodes &abortCode) override;
    int8_t getRemoteDomain(Data &data, int32_t id,
                           SDOAbortCodes &abortCode);
    int8_t setRemoteData(const Data &data, int32_t id,
                         SDOAbortCodes &abortCode) override;
    void configRemoteRPDO(uint16_t pdoIndex, int32_t odIDs[]) override;
    void configRemoteTPDO(uint16_t pdoIndex, int32_t odIDS[]) override;
    void enablePDO() override;
    void disablePDO() override;
    void getRemoteTPDO(uint16_t pdoIndex, Data data[]) override;
    void setRemoteRPDO(uint16_t pdoIndex, Data data[]) override;
    bool saveDataToFlash(uint8_t parameterGroup) override;
    bool loadDataFromFlash(uint8_t parameterGroup) override;
    bool saveSignatureToFlash(uint64_t signature) override;
    bool loadSignatureFromFlash(uint64_t &signature) override;
    void initFlash();
    bool eraseFlash(uint8_t ParameterGroup);
    bool writeFlash(const uint8_t *src, size_t len, uint8_t parameterGroup);
    uint32_t getFlashSector(uint8_t parameterGroup);
    uint32_t getFlashLength(uint8_t parameterGroup);
    bool reboot();
};

}  // namespace CANopen
