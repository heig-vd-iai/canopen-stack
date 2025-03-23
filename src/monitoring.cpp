#include "monitoring.hpp"

#include <stdint.h>

Monitoring::Monitoring() {
    hardware.cycleTime = 0;
    hardware.cycleTimeMax = 0;
    hardware.cycleTimeMin = 0xFFFFFFFF;
    hb.cycleTime = 0;
    hb.cycleTimeMax = 0;
    hb.cycleTimeMin = 0xFFFFFFFF;
    pdo.cycleTime = 0;
    pdo.cycleTimeMax = 0;
    pdo.cycleTimeMin = 0xFFFFFFFF;
    sdo.cycleTime = 0;
    sdo.cycleTimeMax = 0;
    sdo.cycleTimeMin = 0xFFFFFFFF;
    cpuLoad = 0;
    cpuLoadMax = 0;
    cpuLoadMin = 0xFFFFFFFF;
}

void Monitoring::setCycleTimeHardware(uint32_t time) {
    hardware.cycleTime = time;
    if (UNLIKELY(hardware.cycleTime > hardware.cycleTimeMax)) {
        hardware.cycleTimeMax = hardware.cycleTime;
    }
    if (UNLIKELY(hardware.cycleTime < hardware.cycleTimeMin)) {
        hardware.cycleTimeMin = hardware.cycleTime;
    }
}

void Monitoring::setCycleTimeHb(uint32_t time) {
    hb.cycleTime = time;
    if (UNLIKELY(hb.cycleTime > hb.cycleTimeMax)) {
        hb.cycleTimeMax = hb.cycleTime;
    }
    if (UNLIKELY(hb.cycleTime < hb.cycleTimeMin)) {
        hb.cycleTimeMin = hb.cycleTime;
    }
}

void Monitoring::setCycleTimePdo(uint32_t time) {
    pdo.cycleTime = time;
    if (UNLIKELY(pdo.cycleTime > pdo.cycleTimeMax)) {
        pdo.cycleTimeMax = pdo.cycleTime;
    }
    if (UNLIKELY(pdo.cycleTime < pdo.cycleTimeMin)) {
        pdo.cycleTimeMin = pdo.cycleTime;
    }
}

void Monitoring::setCycleTimeSdo(uint32_t time) {
    sdo.cycleTime = time;
    if (UNLIKELY(sdo.cycleTime > sdo.cycleTimeMax)) {
        sdo.cycleTimeMax = sdo.cycleTime;
    }
    if (UNLIKELY(sdo.cycleTime < sdo.cycleTimeMin)) {
        sdo.cycleTimeMin = sdo.cycleTime;
    }
    cpuLoad = 0;  // TODO: Implement CPU load calculation
}
