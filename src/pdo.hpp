#pragma once
#include <cstdint>
#include "enums.hpp"
#include "frame.hpp"

class CANopen_PDO
{
private:
    class CANopen_Node &node;

public:
    CANopen_PDO(class CANopen_Node &node);
    void receiveFrame(CANopen_Frame frame);
    void update(uint32_t timestamp_us);
};
