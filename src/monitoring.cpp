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
    if (__builtin_expect(hardware.cycleTime > hardware.cycleTimeMax, false)) {
        hardware.cycleTimeMax = hardware.cycleTime;
    }
    if (__builtin_expect(hardware.cycleTime < hardware.cycleTimeMin, false)) {
        hardware.cycleTimeMin = hardware.cycleTime;
    }
}

void Monitoring::setCycleTimeHb(uint32_t time) {
    hb.cycleTime = time;
    if (__builtin_expect(hb.cycleTime > hb.cycleTimeMax, false)) {
        hb.cycleTimeMax = hb.cycleTime;
    }
    if (__builtin_expect(hb.cycleTime < hb.cycleTimeMin, false)) {
        hb.cycleTimeMin = hb.cycleTime;
    }
}

void Monitoring::setCycleTimePdo(uint32_t time) {
    pdo.cycleTime = time;
    if (__builtin_expect(pdo.cycleTime > pdo.cycleTimeMax, false)) {
        pdo.cycleTimeMax = pdo.cycleTime;
    }
    if (__builtin_expect(pdo.cycleTime < pdo.cycleTimeMin, false)) {
        pdo.cycleTimeMin = pdo.cycleTime;
    }
}

void Monitoring::setCycleTimeSdo(uint32_t time) {
    sdo.cycleTime = time;
    if (__builtin_expect(sdo.cycleTime > sdo.cycleTimeMax, false)) {
        sdo.cycleTimeMax = sdo.cycleTime;
    }
    if (__builtin_expect(sdo.cycleTime < sdo.cycleTimeMin, false)) {
        sdo.cycleTimeMin = sdo.cycleTime;
    }
    cpuLoad = 0;  // TODO: Implement CPU load calculation
}

float Monitoring::getCycleTimeHardware() { return hardware.cycleTime; }

float Monitoring::getCycleTimeMaxHardware() { return hardware.cycleTimeMax; }

float Monitoring::getCycleTimeMinHardware() { return hardware.cycleTimeMin; }

float Monitoring::getCycleTimeHb() { return hb.cycleTime; }

float Monitoring::getCycleTimeMaxHb() { return hb.cycleTimeMax; }

float Monitoring::getCycleTimeMinHb() { return hb.cycleTimeMin; }

float Monitoring::getCycleTimePdo() { return pdo.cycleTime; }

float Monitoring::getCycleTimeMaxPdo() { return pdo.cycleTimeMax; }

float Monitoring::getCycleTimeMinPdo() { return pdo.cycleTimeMin; }

float Monitoring::getCycleTimeSdo() { return sdo.cycleTime; }

float Monitoring::getCycleTimeMaxSdo() { return sdo.cycleTimeMax; }

float Monitoring::getCycleTimeMinSdo() { return sdo.cycleTimeMin; }
