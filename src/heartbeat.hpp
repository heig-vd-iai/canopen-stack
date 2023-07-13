#pragma once
#include "enums.hpp"
#include "frame.hpp"

class CANopen_HB
{
private:
    class CANopen_Node &node;

public:
    CANopen_HB(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void publishState(NMTStates state);
    void update();
};
