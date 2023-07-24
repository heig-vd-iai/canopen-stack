#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"

class CANopen_SYNC
{
private:
    class CANopen_Node &node;

public:
    CANopen_SYNC(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame, uint32_t timestamp_us);
};
