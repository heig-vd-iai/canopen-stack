#pragma once
#include <cstdint>
#include "frame.hpp"
#include "nmt.hpp"
#include "heartbeat.hpp"
#include "sdo.hpp"
#include "od.hpp"
#include "pdo.hpp"
#include "sync.hpp"

class CANopen_Node
{
    friend CANopen_NMT;
    friend CANopen_HB;
    friend CANopen_SDO;
    friend CANopen_PDO;
    friend CANopen_SYNC;

private:
    ObjectDictionnary od;
    CANopen_NMT nmt;
    CANopen_HB hb;
    CANopen_SDO sdo;
    CANopen_PDO pdo;
    CANopen_SYNC sync;

    void sendFrame(CANopen_Frame frame);
    uint32_t getTime_us();

public:
    const uint8_t nodeId;

    CANopen_Node(uint8_t id);
    void receiveFrame(CANopen_Frame frame);
    void transmitPDO(unsigned index);
    void reloadTPDO();
    void update();
    void saveOD();
    void loadOD();
    Object *findObject(uint16_t index);
    void setNmtTransition(NMTServiceCommands command);
    NMTStates getNmtState();
};
