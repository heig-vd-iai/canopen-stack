#pragma once
#include "frame.hpp"
#include "nmt.hpp"
#include "heartbeat.hpp"

class CANopen_Node
{
private:
    int nodeId;
    CANopen_NMT nmt;
    CANopen_HB hb;

public:
    friend CANopen_NMT;
    friend CANopen_HB;
    CANopen_Node(int id);
    void receiveFrame(CANopen_Frame frame);
    void sendFrame(CANopen_Frame frame);
    void update();
};
