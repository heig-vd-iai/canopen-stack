#pragma once
#include "frame.hpp"

class CANopen_SDO
{
private:
    class CANopen_Node &node;

public:
    CANopen_SDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
};
