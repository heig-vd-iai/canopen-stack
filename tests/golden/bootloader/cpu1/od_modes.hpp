//NOTICE: Auto-generated from the device's yaml file, do not edit manually!
#pragma once
#include "od_enum.hpp"


struct Modes {

    Modes(OperationStateMachine &sm) :

    OperationMode *getMode(OperationModes mode) {
        switch (mode) {
            default:
                return &none;
        }
    }
};