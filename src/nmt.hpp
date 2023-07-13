#pragma once
#include "enums.hpp"
#include "frame.hpp"


class CANopen_NMT
{
private:
    NMTStates currentState;
    class CANopen_Node &node;

public:
    CANopen_NMT(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void setTransition(NMTServiceCommands command);
    void update();
};
