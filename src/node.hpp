#pragma once
#include "frame.hpp"
#include "nmt.hpp"
#include "heartbeat.hpp"
#include "sdo.hpp"
#include "od.hpp"
#include "pdo.hpp"
#include "sync.hpp"
#include "emergency.hpp"
#include <cstdint>
#include <cstdio>

namespace CANopen
{
    class Node
    {
        friend NMT;
        friend HB;
        friend SDO;
        friend PDO;
        friend SYNC;
        friend EMCY;

    private:
        ObjectDictionnary od;
        NMT nmt;
        HB hb;
        SDO sdo;
        PDO pdo;
        SYNC sync;
        EMCY emcy;

        void sendFrame(Frame frame);
        uint32_t getTime_us();

    public:
        const uint8_t nodeId;

        Node(uint8_t id);
        void receiveFrame(Frame frame);
        void transmitPDO(unsigned index);
        void reloadTPDO();
        void update();
        void saveOD();
        void loadOD();
        Object *findObject(uint16_t index);
        void setNmtTransition(NMTServiceCommands command);
        NMTStates getNmtState();
    };
}
