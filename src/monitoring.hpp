#pragma once
#include <stdint.h>

struct CycleTime {
    float cycleTime;
    float cycleTimeMax;
    float cycleTimeMin;
};

class Monitoring {
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

    float getCycleTimeHardware() { return hardware.cycleTime; }
    float getCycleTimeMaxHardware() { return hardware.cycleTimeMax; }
    float getCycleTimeMinHardware() { return hardware.cycleTimeMin; }
    float getCycleTimeHb() { return hb.cycleTime; }
    float getCycleTimeMaxHb() { return hb.cycleTimeMax; }
    float getCycleTimeMinHb() { return hb.cycleTimeMin; }
    float getCycleTimePdo() { return pdo.cycleTime; }
    float getCycleTimeMaxPdo() { return pdo.cycleTimeMax; }
    float getCycleTimeMinPdo() { return pdo.cycleTimeMin; }
    float getCycleTimeSdo() { return sdo.cycleTime; }
    float getCycleTimeMaxSdo() { return sdo.cycleTimeMax; }
    float getCycleTimeMinSdo() { return sdo.cycleTimeMin; }
};
