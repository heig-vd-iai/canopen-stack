#pragma once
#include "nmt.hpp"
#include "heartbeat.hpp"
#include "sdo.hpp"
#include "od.hpp"
#include "pdo.hpp"
#include "sync.hpp"
#include "emergency.hpp"
#include <cstdint>

namespace CANopen
{
    class Node
    {
    private:
        ObjectDictionnary _od;
        NMT _nmt;
        HB _hb;
        SDO _sdo;
        PDO _pdo;
        SYNC _sync;
        EMCY _emcy;

        void sendFrame(class Frame &frame);
        uint32_t getTime_us();

    public:
        friend NMT;
        friend HB;
        friend SDO;
        friend PDO;
        friend SYNC;
        friend EMCY;
        const uint8_t nodeId;

        Node(uint8_t id);
        ObjectDictionnary &od();
        NMT &nmt();
        HB &hb();
        SDO &sdo();
        PDO &pdo();
        SYNC &sync();
        EMCY &emcy();
        void init();
        void receiveFrame(class Frame frame);
        void update();
    };
}
