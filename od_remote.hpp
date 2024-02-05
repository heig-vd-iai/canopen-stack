/**
 * This file contains the remote getter and setter functions for the device.
 * Generated on February 05, 2024
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once
#include "system.hpp"

inline void emptyGetter(Data &data) {}

inline void emptySetter(const Data &data) {}

inline void getobject6040sub0(Data &data) {
    data.u16 = sys.axis->getControlword();
}
inline void setobject6040sub0(const Data &data) {
    sys.axis->setControlword(data.u16);
}
inline void getobject6060sub0(Data &data) {
    data.i8 = sys.axis->getOperationMode();
}
inline void setobject6060sub0(const Data &data) {
    sys.axis->setOperationMode(data.i8);
}
inline void getobject6071sub0(Data &data) {
    data.i64 = sys.getTest();
}
inline void setobject6071sub0(const Data &data) {
    sys.setTest(data.i64);
}

void (*remoteGetter[63])(Data &data) = {
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    &getobject6040sub0,
    emptyGetter,
    &getobject6060sub0,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    &getobject6071sub0,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
};

void (*remoteSetter[63])(const Data &data) = {
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    &setobject6040sub0,
    emptySetter,
    &setobject6060sub0,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    &setobject6071sub0,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
};