#pragma once
#include "frame.hpp"
#include "nmt.hpp"

class CANopen_Node
{
private:
    int nodeId;
    CANopen_NMT nmt;

public:
    friend CANopen_NMT;
    CANopen_Node(int id);
    void receiveFrame(CANopen_Frame frame);
    void sendFrame(CANopen_Frame frame);
    void update();
};
