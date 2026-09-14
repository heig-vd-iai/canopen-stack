/**
 * This file contains the remote getter and setter functions for the device.
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once

inline void emptyGetter(Data &data) {}

inline void emptySetter(const Data &data) {}


void (* const remoteGetter[16])(Data &data) = {
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
};

void (*remoteSetter[16])(const Data &data) = {
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
};

// Object is a 24 bit number: (object << 8 | subobject)
const int32_t remoteGetterBSTKeys[] = {
};

const RemoteGetter remoteGetterBSTMap[] = {
};


const RemoteType remoteTypeBSTMap[] = {
};