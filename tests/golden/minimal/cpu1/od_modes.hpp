//NOTICE: Auto-generated from the device's yaml file, do not edit manually!
#pragma once
#include "od_enum.hpp"

#include "axis/modes/mode_none.hpp"
#include "axis/modes/mode_profilePosition.hpp"

struct Modes {
    ModeNone none;
    ModeProfilePosition profilePosition;

    Modes(OperationStateMachine &sm) :
        none(sm),
        profilePosition(sm){}

    OperationMode *getMode(OperationModes mode) {
        switch (mode) {
            case OPERATION_MODE__NONE:
                return &none;
            case OPERATION_MODE__PROFILE_POSITION:
                return &profilePosition;
            default:
                return &none;
        }
    }
};