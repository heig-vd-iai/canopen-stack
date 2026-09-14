#pragma once
#include <stdint.h>

namespace CANopen {

struct CycleTime {
    uint32_t cycleTime;
    uint32_t cycleTimeMax;
    uint32_t cycleTimeMin;
};

class Monitoring {
    CycleTime hardware, hb, sdo, pdo;
    uint32_t cpuLoad;
    uint32_t cpuLoadMax;
    uint32_t cpuLoadMin;

   public:
    Monitoring();
    void setCycleTimeHardware(uint32_t time);
    void setCycleTimeHb(uint32_t time);
    void setCycleTimeSdo(uint32_t time);
    void setCycleTimePdo(uint32_t time);

    uint32_t getCycleTimeHardware() { return hardware.cycleTime; }
    uint32_t getCycleTimeMaxHardware() { return hardware.cycleTimeMax; }
    uint32_t getCycleTimeMinHardware() { return hardware.cycleTimeMin; }
    uint32_t getCycleTimeHb() { return hb.cycleTime; }
    uint32_t getCycleTimeMaxHb() { return hb.cycleTimeMax; }
    uint32_t getCycleTimeMinHb() { return hb.cycleTimeMin; }
    uint32_t getCycleTimePdo() { return pdo.cycleTime; }
    uint32_t getCycleTimeMaxPdo() { return pdo.cycleTimeMax; }
    uint32_t getCycleTimeMinPdo() { return pdo.cycleTimeMin; }
    uint32_t getCycleTimeSdo() { return sdo.cycleTime; }
    uint32_t getCycleTimeMaxSdo() { return sdo.cycleTimeMax; }
    uint32_t getCycleTimeMinSdo() { return sdo.cycleTimeMin; }
};
}  // namespace CANopen
