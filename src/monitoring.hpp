#pragma once
// #include "ipcCommunication.hpp"  TODO: Why this include? Where is the file?
#include <stdint.h>




struct CycleTime {
    float cycleTime;
    float cycleTimeMax;
    float cycleTimeMin;
};

class Monitoring {
   private:
    CycleTime hardware, hb, sdo, pdo;
    float cpuLoad;
    float cpuLoadMax;
    float cpuLoadMin;

   public:

    Monitoring();
    void setCycleTimeHardware(uint32_t time);
    void setCycleTimeHb(uint32_t time);
    void setCycleTimeSdo(uint32_t time);
    void setCycleTimePdo(uint32_t time);
    float getCycleTimeHardware();
    float getCycleTimeMaxHardware();
    float getCycleTimeMinHardware();
    float getCycleTimeHb();
    float getCycleTimeMaxHb();
    float getCycleTimeMinHb();
    float getCycleTimeSdo();
    float getCycleTimeMaxSdo();
    float getCycleTimeMinSdo();
    float getCycleTimePdo();
    float getCycleTimeMaxPdo();
    float getCycleTimeMinPdo();
};
