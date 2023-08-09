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
        friend NMT;
        friend HB;
        friend SDO;
        friend PDO;
        friend SYNC;
        friend EMCY;

        // private:
    public: // TODO: put back to private
        ObjectDictionnary od;
        NMT nmt;
        HB hb;
        SDO sdo;
        PDO pdo;
        SYNC sync;
        EMCY emcy;

        void sendFrame(class Frame frame);
        uint32_t getTime_us();

    public:
        const uint8_t nodeId;

        Node(uint8_t id);
        void receiveFrame(class Frame frame);
        void transmitPDO(unsigned index);
        void reloadTPDO();
        void update();
        bool saveOD(uint8_t parameterGroup = 0);
        bool loadOD(uint8_t parameterGroup = 0);
        bool restoreOD(uint8_t parameterGroup = 0);
        Object *findObject(uint16_t objectIndex);
        Object *at(uint16_t index);
        void setNmtTransition(NMTServiceCommands command);
        NMTStates getNmtState();
    };
}
