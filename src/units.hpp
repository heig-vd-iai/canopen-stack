#pragma once

#include <math.h>

#include <cstdint>

#include "enums.hpp"
#include "unions.hpp"

#define POSITION_SI_UNIT_INDEX 0x60A8
#define VELOCITY_SI_UNIT_INDEX 0x60A9
#define ACCELERATION_SI_UNIT_INDEX 0x60AA
#define JERK_SI_UNIT_INDEX 0x60AB

namespace CANopen {

class Units {
   private:
    uint32_t positionUnit;
    int32_t positionUnitID;
    uint32_t velocityUnit;
    int32_t velocityUnitID;
    uint32_t accelerationUnit;
    int32_t accelerationUnitID;
    uint32_t jerkUnit;
    int32_t jerkUnitID;

   public:
    Units();
    void convertToSI(Data &data, int32_t odID);
    int8_t getData(Data &data, int32_t odID, SDOAbortCodes &abortCode);
    int8_t setData(const Data &data, int32_t odID, SDOAbortCodes &abortCode);
};

}  // namespace CANopen
