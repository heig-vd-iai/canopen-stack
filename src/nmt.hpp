#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"

class CANopen_NMT
{
private:
    NMTStates currentState;
    class CANopen_Node &node;

public:
    CANopen_NMT(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame, uint64_t timestamp_us);
    void setTransition(NMTServiceCommands command);
    NMTStates getState();
    void update(uint64_t timestamp_us);
};
