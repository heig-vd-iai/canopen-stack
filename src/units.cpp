#include "units.hpp"

#include "node.hpp"
#include "od.hpp"

using namespace CANopen;

Units::Units() {
    positionUnitID = node.od().findObject(POSITION_SI_UNIT_INDEX);
    velocityUnitID = node.od().findObject(VELOCITY_SI_UNIT_INDEX);
    accelerationUnitID = node.od().findObject(ACCELERATION_SI_UNIT_INDEX);
    jerkUnitID = node.od().findObject(JERK_SI_UNIT_INDEX);
    Data tmp;
    SDOAbortCodes abortCode;
    if (positionUnitID != -1) {
        getLocalData_uint32_t(tmp, positionUnitID, abortCode);
        positionUnit = tmp.u32;
    }
    if (velocityUnitID != -1) {
        getLocalData_uint32_t(tmp, velocityUnitID, abortCode);
        velocityUnit = tmp.u32;
    }
    if (accelerationUnitID != -1) {
        getLocalData_uint32_t(tmp, accelerationUnitID, abortCode);
        accelerationUnit = tmp.u32;
    }
}
