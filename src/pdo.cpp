#include "pdo.hpp"
#include "node.hpp"

CANopen_PDO::CANopen_PDO(CANopen_Node &node) : node(node)
{
}

void CANopen_PDO::receiveFrame(CANopen_Frame frame)
{
}

void CANopen_PDO::update(uint32_t timestamp_us)
{
}
