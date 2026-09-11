/**
 * This file contains the remote getter and setter functions for the device.
 *
 * NOTICE: Auto-generated from the device's yaml file, do not edit manually!
 */
#pragma once

inline void emptyGetter(Data &data) {}

inline void emptySetter(const Data &data) {}

inline void getobject2005sub0(Data &data) {
    data.u32 = remote.getValue();
}
inline void setobject2005sub0(const Data &data) {
    remote.setValue(data.u32);
}
inline void setobject2006sub0(const Data &data) {
    remote.attribute = data.f32 * 0.001f;
}
inline void getobject2006sub0(Data &data) {
    data.f32 = remote.attribute * 1000;
}
inline void getobject6064sub0(Data &data) {
    data.i32 = remote.position;
}

void (* const remoteGetter[])(Data &data) = {
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
    &getobject2005sub0,
    &getobject2006sub0,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    emptyGetter,
    &getobject6064sub0,
};

void (*remoteSetter[])(const Data &data) = {
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
    &setobject2005sub0,
    &setobject2006sub0,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
    emptySetter,
};

// Object is a 24 bit number: (object << 8 | subobject)
const int32_t remoteGetterBSTKeys[] = {
    2098688,
    2098432,
    6317056,
};

const RemoteGetter remoteGetterBSTMap[] = {
    &getobject2006sub0,
    &getobject2005sub0,
    &getobject6064sub0,
};


const RemoteType remoteTypeBSTMap[] = {
    REMOTE_TYPE__FLOAT32,
    REMOTE_TYPE__UINT32,
    REMOTE_TYPE__INT32,
};