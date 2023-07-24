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
    // private:
public:
    uint8_t nodeId;
    ObjectDictionnary od;
    CANopen_NMT nmt;
    CANopen_HB hb;
    CANopen_SDO sdo;
    CANopen_PDO pdo;
    CANopen_SYNC sync;

public:
    friend CANopen_NMT;
    friend CANopen_HB;
    friend CANopen_SDO;
    friend CANopen_PDO;
    friend CANopen_SYNC;
    CANopen_Node(uint8_t id);
    void receiveFrame(CANopen_Frame frame);
    void sendFrame(CANopen_Frame frame);
    void update();
    uint32_t getTime_us();
};
